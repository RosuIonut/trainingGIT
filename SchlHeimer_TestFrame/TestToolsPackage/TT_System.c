#include "cdef.h"

#include "SHMEM_Access.h"
#include "SHMEM_Layout.h"

#include "TT__TimingTest.h"
#include "TT__Sequencer.h"
#include "TT__Trace.h"
#include "TT__ObjPatch.h"

#include "TT_System.h"
//#include "stdio.h"

static SHMEM_tstAccessContext   TT__stAccessContext;
static SHMEM_tstSyncContext     TT__stSyncContext;
// 20170113 wti: Needs to be FALSE even before InitReset is called for the first time!
static bool TT__boShMemInitialized = False;
static bool TT__boJobActive = False;

static void TT__vProcessDownload(void *pvMemory)
{
   SHMEM_tstShMemHeader *pstHeader = pvMemory;
   SHMEM_tstSectDescr *pstFirstSection = (SHMEM_tstSectDescr*) (pstHeader + 1);
   uint8 u8Index;

   /* Notify every handler about new job */
   for (u8Index = 0; u8Index < pstHeader->u8SectCount ; u8Index++)
   {
      SHMEM_tstSectDescr *pstSection = pstFirstSection + u8Index;
      void *pvData = ((uint8*) pvMemory) + pstSection->u32Offset;

      switch (pstSection->u8Type)
      {
      case SHMEM_nSectTimingTest:
         TT__vTimingTestOnDownload(pvData, pstSection->u32Length);
         break;
      case SHMEM_nSectSequencer:
         TT__vSequencerOnDownload(pvData, pstSection->u32Length);
         break;
      case SHMEM_nSectTraceLog:
         TT__vTraceOnDownload(pvData, pstSection->u32Length);
         break;
      case SHMEM_nSectObjectPatches:
         TT__vObjPatchOnDownload(pvData, pstSection->u32Length);
         break;
      }
   }
}

static bool TT__boProcessUpload(void *pvMemory)
{
   /* Overall result is True until one handler denies upload */
   bool boAllReady = True;
   SHMEM_tstShMemHeader *pstHeader = pvMemory;
   SHMEM_tstSectDescr *pstFirstSection = (SHMEM_tstSectDescr*) (pstHeader + 1);
   uint8 u8Index;

   /* Ask every handler for its data and upload permission */
   for (u8Index = 0; u8Index < pstHeader->u8SectCount ; u8Index++)
   {
      /* Default is True, because sections without a handler
       * should not block the compelte job (robustness).
       */
      bool boSectReady = True;
      SHMEM_tstSectDescr *pstSection = pstFirstSection + u8Index;
      void *pvData = ((uint8*) pvMemory) + pstSection->u32Offset;

      switch (pstSection->u8Type)
      {
      case SHMEM_nSectTimingTest:
         boSectReady = TT__boTimingTestQueryUpload(pvData, pstSection->u32Length);
         break;
      case SHMEM_nSectSequencer:
         boSectReady = TT__boSequencerQueryUpload(pvData, pstSection->u32Length);
         break;
      }

      /* Each handler is allowed to prevent the overall upload */
      if (!boSectReady)
      {
         boAllReady = False;
         break;
      }
   }

   /* Call handler that upload will happen (e.g. for trace to disable) */
   if (boAllReady)
   {
      TT__vTraceConfirmUpload();
      TT__vObjPatchConfirmUpload();
   }

   return boAllReady;
}


// Called every 10ms, this checks if there are tests to run and runs them if there are
void TT_vCyclic10ms(void)
{
    TT__vSequencerCyclic10ms();
}

// called every 100ms this function manages the commication between the ST package and the external 
// Schleißheimer tool
// N.B.: this function is called ST-Component when running in the Legacy and the HMI components
void TT_vCyclic100ms(void)
{
    if (TT__boShMemInitialized)
    {
        /* Download phase (get any test requests from host)*/
        if (!TT__boJobActive && SHMEM_boIsHostDone(&TT__stSyncContext))
        {
            SHMEM_vClearHostDone(&TT__stSyncContext);
            TT__vProcessDownload(TT__stAccessContext.pMemoryBlock);
            TT__boJobActive = True;
        }

        /* Cyclic processing of tests */
        if (TT__boJobActive)
        {
            TT__vTimingTestCyclic100ms();
        }

        /* Upload phase (send results back to shared memory for the external ST (Host) to analyse)*/
        if (TT__boJobActive && TT__boProcessUpload(TT__stAccessContext.pMemoryBlock))
        {
            SHMEM_vSetClientDone(&TT__stSyncContext);
            TT__boJobActive = False;
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------
// initialisation and timer functions for ST component when running in the Legacy module
// These functions are registered with the SINI / CAM by declarations in TT_Legacy.sdh
//-----------------------------------------------------------------------------------------------------------------------
//
// Called at RESET and WAKEUP
void TT_vLegacyInitReset(void)
{
   TT__boShMemInitialized = False;
   TT__boJobActive = False;

//   if (SHMEM_boInitLegacyAccessContext(&TT__stAccessContext))
//   {
//       TT__boShMemInitialized = True;
//   }
   if (  SHMEM_boInitAccessContext(&TT__stAccessContext)
      && SHMEM_boInitLegacySyncContext(&TT__stSyncContext) )
   {
       TT__boShMemInitialized = True;
   }

   TT__vTimingTestInit();
   TT__vSequencerInit();
   TT__vTraceInit(&TT__stAccessContext);    // 20170112 wti: Added parameter
   TT__vObjPatchInit();
}

//
//-----------------------------------------------------------------------------------------------------------------------
//END: initalisation and timer functions for ST compnent when running in Legacy module
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
// initialisation and timer functions for ST component when running in the HMI module
// These functions are registered with the SINI / CAM by declarations in TT_Legacy.sdh
//-----------------------------------------------------------------------------------------------------------------------
//
// Called at RESET and WAKEUP
void TT_vHMIInitReset(void)
{
    TT__boShMemInitialized = False;
    TT__boJobActive = False;
    
//    if (SHMEM_boInitHMIAccessContext(&TT__stAccessContext))
//    {
//        TT__boShMemInitialized = True;
//    }
    if (   SHMEM_boInitAccessContext(&TT__stAccessContext) 
        && SHMEM_boInitHmiSyncContext(&TT__stSyncContext) );
    {
        TT__boShMemInitialized = True;
    }

    TT__vTimingTestInit();
    TT__vSequencerInit();
    TT__vTraceInit(&TT__stAccessContext);   // 20170112 wti: Added parameter
    TT__vObjPatchInit();
}


