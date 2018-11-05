#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "SHMEM_Layout.h"
#include "TT__TimingTest.h"

static struct
{
   bool boActive;
   bool boStarted;
   bool boDone;

   uint32 u32StartTime;
   uint32 u32StopTime;
   uint8 u8CyclesLeft;
} TT__stTimingTest;

void TT__vTimingTestInit(void)
{
   TT__stTimingTest.boActive = False;
}

void TT__vTimingTestOnDownload(void *pvData, uint32 u32Length)
{
   SHMEM_tstTimingTest *pstTest = pvData;

   /* Clear previous settings */
   memset(&TT__stTimingTest, 0, sizeof(TT__stTimingTest));

   /* Set cycles to run */
   TT__stTimingTest.u8CyclesLeft = pstTest->u8CycleCount100ms;

   /* Request cyclic task to star test */
   TT__stTimingTest.boActive = True;
}

bool TT__boTimingTestQueryUpload(void *pvData, uint32 u32Length)
{
   bool boUploadReady = False;

   if (TT__stTimingTest.boActive)
   {
      if (TT__stTimingTest.boDone)
      {
         SHMEM_tstTimingTest *pstTest = pvData;
         uint32 u32ElapsedTime = TT__stTimingTest.u32StopTime - TT__stTimingTest.u32StartTime;

         /* Fill in result */
         if (u32ElapsedTime > 0xFFFF)
            pstTest->u16MsNeeded = 0xFFFF;
         else
            pstTest->u16MsNeeded = (uint16) u32ElapsedTime;

         /* Job processed, not active anymore */
         TT__stTimingTest.boActive = False;
         boUploadReady = True;
      }
   }
   else
   {
      /* Test is not active, do not prevent upload */
      boUploadReady = True;
   }

   return boUploadReady;
}

void TT__vTimingTestCyclic100ms(void)
{
   /* Actions only needed if test is active */
   if (TT__stTimingTest.boActive)
   {
      /* Need to start? */
      if (!TT__stTimingTest.boStarted)
      {
         TT__stTimingTest.u32StartTime = GetTickCount();
         TT__stTimingTest.boStarted = True;
      }

      /* Allowed to stop? */
      if (!TT__stTimingTest.boDone && TT__stTimingTest.u8CyclesLeft == 0)
      {
         TT__stTimingTest.u32StopTime = GetTickCount();
         TT__stTimingTest.boDone = True;
      }

      /* Decrement counter */
      if (TT__stTimingTest.u8CyclesLeft > 0)
         TT__stTimingTest.u8CyclesLeft--;
   }
}
