#ifndef TT__TIMINGTEST_H
#define TT__TIMINGTEST_H

#include "cdef.h"

void TT__vTimingTestInit(void);
void TT__vTimingTestOnDownload(void *pvData, uint32 u32Length);
bool TT__boTimingTestQueryUpload(void *pvData, uint32 u32Length);
void TT__vTimingTestCyclic100ms(void);

#endif
