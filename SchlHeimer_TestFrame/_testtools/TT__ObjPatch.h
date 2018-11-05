#ifndef TT__OBJPATCH_H
#define TT__OBJPATCH_H

#include "cdef.h"
#include "SHMEM_Layout.h"

void TT__vObjPatchInit(void);
void TT__vObjPatchOnDownload(void *pvData, uint32 u32Length);
void TT__vObjPatchConfirmUpload(void);

void TT__vPatchObject(void *pvObject, SHMEM_tenObjTypes enType);

#endif
