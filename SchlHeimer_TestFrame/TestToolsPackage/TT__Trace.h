#ifndef TT__TRACE_H
#define TT__TRACE_H

#include "SHMEM_Layout.h"
#include "SHMEM_Access.h"
#include "cdef.h"

void TT__vTraceInit(SHMEM_tstAccessContext *pstContext);
void TT__vTraceOnDownload(void *pvData, uint32 u32Length);
void TT__vTraceConfirmUpload(void);
void TT__vTraceItem(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, const void *pvBlock);
uint8 TT__u8TraceGetArgCount(SHMEM_tenTraceItemIds enItemId);
bool TT__boDoesItemPassFilter(SHMEM_tenTraceItemIds enItemId, const uint32 *pu32Args, uint8 u8ArgCount);
bool TT__boIsActive(void);

#endif
