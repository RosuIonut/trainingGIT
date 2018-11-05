#include "TT__Trace.h"
#include "stdio.h"

static struct
{
   /* Flag if trace is currently active */
   bool boActive;
   /* Pointer to trace header in shared memory */
   SHMEM_tstTraceHeader *pstHeader;
   /* Pointer to filter section in shared memory, or NULL */
   const SHMEM_tstTraceFilterEntry *pstFilters;
   /* Pointer to trace log buffer in shared memory */
   uint8 *pu8Buffer;
   /* Pointer to last log entry of each arg trace item, or NULL */
   SHMEM_tstTraceEntryArgs *apstLastLogPtr[SHMEM_nenTraceItemCount];
   /* Mutex to manage write access to shared memory */
   HANDLE hTraceWriteMutex;
} TT__Trace = { 0 };

bool TT__boIsActive(void)
{
    return TT__Trace.boActive;
}

void TT__vTraceInit(SHMEM_tstAccessContext *pstContext)
{
   TT__Trace.boActive = False;
   TT__Trace.hTraceWriteMutex = pstContext->hWriteMutex;
}

void TT__vTraceOnDownload(void *pvData, uint32 u32Length)
{
   uint16 u16Index;

   /* Save pointer to trace header */
   TT__Trace.pstHeader = pvData;
   /* Are filters defined? */
   if (TT__Trace.pstHeader->u16FilterEntries > 0)
   {
      /* Set pointer to filter section */
      TT__Trace.pstFilters = (SHMEM_tstTraceFilterEntry*) (TT__Trace.pstHeader + 1);
      /* Buffer starts after filter section */
      TT__Trace.pu8Buffer = (uint8*) (TT__Trace.pstFilters + TT__Trace.pstHeader->u16FilterEntries);
   }
   else
   {
      /* There is no filter section */
      TT__Trace.pstFilters = NULL;
      /* Buffer starts directly after header */
      TT__Trace.pu8Buffer = (uint8*) (TT__Trace.pstHeader + 1);
   }
   /* Unset all last log pointers */
   for (u16Index = 0; u16Index < SHMEM_nenTraceItemCount; u16Index++)
      TT__Trace.apstLastLogPtr[u16Index] = NULL;
   /* Trace is active now */
   TT__Trace.boActive = True;
   
   /*wti*  printf("Download: Trace is active.\n"); */
}

void TT__vTraceConfirmUpload(void)
{
   /* Disable trace until next download */
   TT__Trace.boActive = False;

   /*wti*  printf("Upload: Trace gets inactive.\n"); */
}

/*static*/ bool TT__boDoesItemPassFilter(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, uint8 u8ArgCount)
{
   /* Default is pass, in case for filter index == 0xFFFF */
   bool boPass = True;
   /* Filter index for this trace item */
   const uint16 u16FilterIndex = TT__Trace.pstHeader->astItemCfg[enItemId].u16FilterIndex;

   if (u16FilterIndex < TT__Trace.pstHeader->u16FilterEntries)
   {
      /* Pointer to current filter entry, starting with first item of sum */
      const SHMEM_tstTraceFilterEntry *pstEntry = &TT__Trace.pstFilters[u16FilterIndex];

      /* Sum defaults to False because it is OR-ed together */
      bool boSum = False;
      bool boEndSum = False;
      while (!boEndSum)
      {
         /* Product defaults to True because it is AND-ed together */
         bool boProduct = True;
         bool boEndProduct = False;
         while (!boEndProduct)
         {
            if (pstEntry->u8Flags & SHMEM_nTraceFlagAlwaysTrue)
            {
               /* This entry evaluates to true, always:
                * boProduct = boProduct && True;
                * (which is a statement without effect)
                */
            }
            else
            {
               /* Evaluate this entry */
               const uint8 u8Index = pstEntry->u8ArgIndex;
               if (u8Index < u8ArgCount)
               {
                  /* Result of this expression's entry */
                  bool boExp = (pu32Args[u8Index] == pstEntry->u32ArgValue);
                  /* Check for negation and AND result to product */
                  if (pstEntry->u8Flags & SHMEM_nTraceFlagNot)
                     boProduct = boProduct && !boExp;
                  else
                     boProduct = boProduct && boExp;
               }
            }

            /* Save flags of current entry for loop control */
            boEndProduct = (pstEntry->u8Flags & SHMEM_nTraceFlagEndProduct) != 0;
            boEndSum = (pstEntry->u8Flags & SHMEM_nTraceFlagEndSum) != 0;
            /* Advance to next entry */
            pstEntry++;
         }
         /* OR result to sum */
         boSum = boSum || boProduct;
      }
      
      /* Overall result is the sum of all products */
      boPass = boSum;
   }

   return boPass;
}

static bool TT__boInsertTraceItemArgs(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, uint8 u8ArgCount)
{
    bool retReleaseMutex;
    DWORD retWaitMutex;
    LARGE_INTEGER largeInt;

   bool boInserted = False;
   SHMEM_tstTraceHeader *pstTrace = TT__Trace.pstHeader;

   // 20170116 wti: Added mutex to avoid access conflicts
   retWaitMutex = WaitForSingleObject( TT__Trace.hTraceWriteMutex, 50 );

   /* Does it fit in? (For simplicity we calculate with maximum argument count) */
//   if (pstTrace->u32BytesUsed + sizeof(SHMEM_tstTraceEntryArgs) < pstTrace->u32BytesMax)
   if ((pstTrace->u32BytesUsed + sizeof(SHMEM_tstTraceEntryArgs) < pstTrace->u32BytesMax)
    && (WAIT_OBJECT_0 == retWaitMutex)) // got mutex if true
   {
      /*wti*  printf("TT__boInsertTraceItemArgs( %d ), BytesUsed_0=%d", enItemId, pstTrace->u32BytesUsed); */
      uint8 u8ArgIndex;
      uint16 u16ArgSize = 0;
      uint8 *pu8BufferNext = TT__Trace.pu8Buffer + pstTrace->u32BytesUsed;
      SHMEM_tstTraceEntryArgs *pstEntry = (SHMEM_tstTraceEntryArgs*) pu8BufferNext;

      /* Insert ID and timestamp of trace item */
      pstEntry->stHeader.u16Id = (uint16) enItemId;
      QueryPerformanceCounter(&largeInt);
      pstEntry->stHeader.i64Timestamp = largeInt.QuadPart;

      /* Insert optional arguments */
      for (u8ArgIndex = 0; u8ArgIndex < u8ArgCount; u8ArgIndex++)
      {
         pstEntry->au32Args[u8ArgIndex] = pu32Args[u8ArgIndex];
         u16ArgSize += sizeof(uint32);
      }
      /* Insert size of arguments */
      pstEntry->stHeader.u16ArgSize = u16ArgSize;

      /* Adjust last log pointer */
      TT__Trace.apstLastLogPtr[enItemId] = pstEntry;

      /* Update counter of used bytes */
      pstTrace->u32BytesUsed += sizeof(SHMEM_tstTraceEntryHeader) + pstEntry->stHeader.u16ArgSize;
      
      /*wti*  printf(", BytesUsed_1=%d\n", pstTrace->u32BytesUsed); */
      
      /* Operation successful */
      boInserted = True;
   }
   else
   {
       printf("ERROR in TT__boInsertTraceItemArgs: retWaitMutex=%d\b", retWaitMutex);
   }

   // 20170116 wti: Release mutex again
   retReleaseMutex = ReleaseMutex( TT__Trace.hTraceWriteMutex );

   return boInserted;
}

static bool TT__boInsertTraceItemBlock(SHMEM_tenTraceItemIds enItemId, const uint8 *pu8Data, uint16 u16Length)
{
   bool boInserted = False;
   SHMEM_tstTraceHeader *pstTrace = TT__Trace.pstHeader;

   /* Does it fit in? (Generic header + payload) */
   if (pstTrace->u32BytesUsed + sizeof(SHMEM_tstTraceEntryHeader) + u16Length < pstTrace->u32BytesMax)
   {
      uint8 *pu8BufferNext = TT__Trace.pu8Buffer + pstTrace->u32BytesUsed;
      SHMEM_tstTraceEntryBlock *pstEntry = (SHMEM_tstTraceEntryBlock*) pu8BufferNext;
      uint16 u16BlockPos;

      /* Insert ID of trace item */
      pstEntry->stHeader.u16Id = (uint16) enItemId;
      /* Insert length of text string */
      pstEntry->stHeader.u16ArgSize = u16Length;
      /* Insert data block */
      for (u16BlockPos = 0; u16BlockPos < u16Length; u16BlockPos++)
         pstEntry->au8Data[u16BlockPos] = pu8Data[u16BlockPos];

      /* Update counter of used bytes */
      pstTrace->u32BytesUsed += sizeof(SHMEM_tstTraceEntryHeader) + pstEntry->stHeader.u16ArgSize;
      /* Operation successful */
      boInserted = True;
   }

   return boInserted;
}


static bool TT__boCheckTraceDataChanged(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, uint8 u8ArgCount)
{
   bool boChanged = False;
   const SHMEM_tstTraceEntryArgs *pstLastLog = TT__Trace.apstLastLogPtr[enItemId];

   if (pstLastLog)
   {
      /* Already been logged => compare values */
      uint8 u8Index = 0;
      while (!boChanged && u8Index < u8ArgCount)
      {
         if (pu32Args[u8Index] != pstLastLog->au32Args[u8Index])
            boChanged = True;
         u8Index++;
      }
   }
   else
   {
      /* Not yet logged => it has changed */
      boChanged = True;
   }

   return boChanged;
}

void TT__vTraceItem(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, const void *pvBlock)
{
   if (TT__Trace.boActive && enItemId < SHMEM_nenTraceItemCount)
   {
      const SHMEM_tstTraceItemCfg *pstConfig = &TT__Trace.pstHeader->astItemCfg[enItemId];

      if (pstConfig->u8Mode != 0)
      {
         bool boInsertItem = False;
         bool boInsertBlock = False;

         switch (pstConfig->u8Type)
         {
         case SHMEM_nTraceItemTypeEvent:
            /* Events will always be traced */
            boInsertItem = True;
            break;
         case SHMEM_nTraceItemTypeData:
            /* Data will be traced according its notification mode */
            switch (pstConfig->u8Mode)
            {
            case SHMEM_nTraceDataNotifyAlways:
               boInsertItem = True;
               break;
            case SHMEM_nTraceDataNotifyOnChange:
               boInsertItem = TT__boCheckTraceDataChanged(enItemId, pu32Args, pstConfig->u8ArgCount);
               break;
            default:
               /* Invalid mode or disabled */
               break;
            }
            break;
         case SHMEM_nTraceItemTypeBlock:
            /* Blocks will always be traced (if they fit into the specification) */
            if (*pu32Args <= SHMEM_nTraceMaxBlockSize)
               boInsertBlock = True;
            break;
         default:
            /* Invalid item type, do nothing */
            break;
         }

         /* Check if item should be inserted and also does pass filter */
         if (boInsertItem && TT__boDoesItemPassFilter(enItemId, pu32Args, pstConfig->u8ArgCount))
         {
            if (!TT__boInsertTraceItemArgs(enItemId, pu32Args, pstConfig->u8ArgCount))
            {
               /* Trace buffer is full, disable further tracing. */
               TT__Trace.pstHeader->u8Flags |= SHMEM_nTraceFlagBufferFull;
               TT__Trace.boActive = False;
            }
         }
         /* Check if block should be inserted (block do not support filtering yet) */
         if (boInsertBlock)
         {
            if (!TT__boInsertTraceItemBlock(enItemId, pvBlock, (uint16) pu32Args[0]))
            {
               /* Trace buffer is full, disable further tracing. */
               TT__Trace.pstHeader->u8Flags |= SHMEM_nTraceFlagBufferFull;
               TT__Trace.boActive = False;
            }
         }
      }
      else
      {
         /* Item disabled, nothing to do */
      }
   }
   else
   {
      /* Trace either not active, or item ID invalid */
   }
}

uint8 TT__u8TraceGetArgCount(SHMEM_tenTraceItemIds enItemId)
{
   uint8 u8ArgCount = 0;
   if (TT__Trace.boActive && enItemId < SHMEM_nenTraceItemCount)
   {
      u8ArgCount = TT__Trace.pstHeader->astItemCfg[enItemId].u8ArgCount;
   }
   return u8ArgCount;
}
