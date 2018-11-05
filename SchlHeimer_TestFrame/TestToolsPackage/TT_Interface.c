#include <stdio.h>
#include <stdarg.h>

#include "TT__Trace.h"
#include "TT__ObjPatch.h"

#include "TT_Interface.h"


// Maximum length of a trace line
#define TRACE_MAX_LINE_LENGTH  1024

uint32 TT_storedHmiFrameId = 0;

void TT_vTraceFormat(const char *pcFormat, ...)
{
   uint32 u32Length;
   char acBuffer[TRACE_MAX_LINE_LENGTH];
   va_list args;

   // Create string to trace using stdio functions
   va_start(args, pcFormat);
   u32Length = vsnprintf(acBuffer, sizeof(acBuffer), pcFormat, args);
   va_end(args);

   // Note that the size must be passed as uint32 pointer, although
   // later on only an uint16 will be used to store the length.
   TT__vTraceItem(SHMEM_nenTrace_TextString, &u32Length, acBuffer);
}

void TT_vTraceEvent(TT_tenEventIds enEvent, ...)
// {
// }
// void TT_dmy1(TT_tenEventIds enEvent, ...)
{
    if (TT__boIsActive() && TT__boDoesItemPassFilter(enEvent, NULL, 0)) //20170504 wti: (pre-)Filter first to optimize runtime
    {
    va_list args;
    uint32 au32Args[SHMEM_nTraceMaxArgCount];
    uint8 u8ArgCount;
    uint8 u8CurrArg;

    va_start(args, enEvent);
    u8ArgCount = TT__u8TraceGetArgCount((SHMEM_tenTraceItemIds)enEvent);
    for (u8CurrArg = 0; u8CurrArg < u8ArgCount; u8CurrArg++)
        au32Args[u8CurrArg] = va_arg(args, uint32);
    va_end(args);

    TT__vTraceItem((SHMEM_tenTraceItemIds)enEvent, au32Args, NULL);
    }
}

void TT_vTraceData(TT_tenDataIds enData, ...)
// {
// }
// void TT_dmy2(TT_tenDataIds enData, ...)
{
   va_list args;
   uint32 au32Args[SHMEM_nTraceMaxArgCount];
   uint8 u8ArgCount;
   uint8 u8CurrArg;

   va_start(args, enData);
   u8ArgCount = TT__u8TraceGetArgCount((SHMEM_tenTraceItemIds) enData);
   for (u8CurrArg = 0; u8CurrArg < u8ArgCount; u8CurrArg++)
      au32Args[u8CurrArg] = va_arg(args, uint32);
   va_end(args);

   TT__vTraceItem((SHMEM_tenTraceItemIds) enData, au32Args, NULL);
}

void TT_vPatchObject(void *pvObject, TT_tenObjectType enType)
{
   TT__vPatchObject(pvObject, (SHMEM_tenObjTypes) enType);
}
