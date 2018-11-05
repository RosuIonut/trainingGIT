#include "SHMEM_Access.h"
#include "SHMEM_Layout.h"

/* typedef for the synchronization events for host - client communication */
typedef struct SHMEM_SyncEventsStruct {
    LPCTSTR lpcNameHostDoneEvent;       /* Event indicating that the host has completed processing */
    LPCTSTR lpcNameClientDoneEvent;     /* Event indicating that the client has completed processing */
} tSHMEM_SyncEventsStruct;

/* Definition of the synchronization events for the Legacy module */
static const tSHMEM_SyncEventsStruct SHMEM_LegacySyncEvents = { 
    TEXT("FPK_IntTest_SHMEM_LegacyHostDoneEvent"),
    TEXT("FPK_IntTest_SHMEM_LegacyDoneEvent") 
};

/* Definition of the synchronization events for the HMI module */
static const tSHMEM_SyncEventsStruct SHMEM_HmiSyncEvents = { 
    TEXT("FPK_IntTest_SHMEM_HmiHostDoneEvent"),   
    TEXT("FPK_IntTest_SHMEM_HmiDoneEvent") 
};



/** Returns TRUE if object is set 
*/
static BOOL SHMEM__boWaitForObjects(HANDLE* phHandles, DWORD dwTimeout)
{
    BOOL boResult = FALSE;
    if ((phHandles[0] != NULL) && (phHandles[1] != NULL))
    {
        if (WaitForMultipleObjects(2, phHandles, TRUE, dwTimeout) == WAIT_OBJECT_0)
        {
            boResult = TRUE;
        }
    }
    return boResult;
}

/** Returns TRUE if object is set 
*/
static BOOL SHMEM__boWaitForObject(HANDLE hHandle, DWORD dwTimeout)
{
   BOOL boResult = FALSE;

   if (hHandle != NULL)
   {
      if (WaitForSingleObject(hHandle, dwTimeout) == WAIT_OBJECT_0)
         boResult = TRUE;
   }

   return boResult;
}




/** Creates shared memory and intialises pstContext with this information 
*/
BOOL SHMEM_boInitAccessContext(SHMEM_tstAccessContext *pstContext)
{
   BOOL boFailed = FALSE;
   BOOL boFirst = FALSE;
   HANDLE hMapping = NULL;
   PVOID pView = NULL;
   HANDLE hShMemWriteMutex = NULL;

   /* Create file mapping */
   if (!boFailed)
   {
      hMapping = CreateFileMapping(
         INVALID_HANDLE_VALUE,         /* Backed by page file */
         NULL,                         /* No security attributes */
         PAGE_READWRITE,               /* Access (r/w) */
         0,                            /* High DWORD of size */
         SHMEM_nMaxSharedObjectSize,   /* Low DWORD of size */
         SHMEM_rsNameMemoryBlock);     /* Object's name */

      if (hMapping != NULL)
      {
         /* Remember if mapping is new or already existed */
         boFirst = (GetLastError() != ERROR_ALREADY_EXISTS);
      }
      else
      {
         /* Creating file mapping failed */
         boFailed = TRUE;
      }
   }

   /* Get pointer to view */
   if (!boFailed)
   {
      pView = MapViewOfFile(
         hMapping,                        /* Mapping to use */
         FILE_MAP_READ | FILE_MAP_WRITE,  /* Access (r/w) */
         0,                               /* High DWORD of offset */
         0,                               /* Low DWORD of offset */
         0);                              /* Size (complete mapping) */

      if (pView == NULL)
         boFailed = TRUE;
   }

   /* Initialize/check version header field */
   if (!boFailed)
   {
      SHMEM_tstShMemHeader *pstHeader = (SHMEM_tstShMemHeader*) pView;

      if (boFirst)
      {
         /* First user initializes */
         pstHeader->u8Version = SHMEM_nLayoutVersion;
         pstHeader->u32ObjSize = sizeof(*pstHeader);
      }
      else
      {
         /* Second user checks version */
         if (pstHeader->u8Version != SHMEM_nLayoutVersion)
            boFailed = TRUE;
      }
   }

   /* Create trace write access mutex */
   if (!boFailed)
   {
      hShMemWriteMutex = CreateMutex(
         NULL,                                  /* No security attributes */
         FALSE,                                 /* Initial owner */
         SHMEM_rsNameClientWriteMutex);         /* Object's name */

      if (hShMemWriteMutex == NULL)
         boFailed = TRUE;
    
      pstContext->hWriteMutex = hShMemWriteMutex;
      // printf("hWriteMutex = %d\n", hShMemWriteMutex);
   }

   /* Check if everything is okay */
   if (!boFailed)
   {
      /* Save context */
      pstContext->hMemoryMapping = hMapping;
      pstContext->pMemoryBlock = pView;
   }
   else
   {
      /* Clean up */
      SHMEM_vDeinitAccessContext(pstContext);
   }

   return !boFailed;
}

/** Destroy the shared memory objects
*/
void SHMEM_vDeinitAccessContext(SHMEM_tstAccessContext *pstContext)
{
   if (pstContext->pMemoryBlock != NULL)
   {
      UnmapViewOfFile(pstContext->pMemoryBlock);
      pstContext->pMemoryBlock = NULL;
   }

   if (pstContext->hMemoryMapping != NULL)
   {
      CloseHandle(pstContext->hMemoryMapping);
      pstContext->hMemoryMapping = NULL;
   }
   
   if (pstContext->hWriteMutex != NULL)
   {
      CloseHandle(pstContext->hWriteMutex);
      pstContext->hWriteMutex = NULL;
   }
}


/** Creates sychronization event objects and intialises pstContext with this information 
*/
static BOOL SHMEM_boInitSyncContext( SHMEM_tstSyncContext *pstContext, 
                               const tSHMEM_SyncEventsStruct* const pstEventNames)
{
   BOOL boFailed = FALSE;
   HANDLE hHostDoneEvent = NULL;
   HANDLE hClientDoneEvent = NULL;

   /* Create synchronization event for host */
   if (!boFailed)
   {
      hHostDoneEvent = CreateEvent(
         NULL,                                  /* No security attributes */
         TRUE,                                  /* Manual reset needed */
         FALSE,                                 /* Initial state */
         pstEventNames->lpcNameHostDoneEvent);  /* Object's name */

      if (hHostDoneEvent == NULL)
         boFailed = TRUE;
   }

   /* Create synchronization event for client */
   if (!boFailed)
   {
      hClientDoneEvent = CreateEvent(
         NULL,                                      /* No security attributes */
         TRUE,                                      /* Manual reset needed */
         FALSE,                                     /* Initial state */
         pstEventNames->lpcNameClientDoneEvent);    /* Object's name */

      if (hClientDoneEvent == NULL)
         boFailed = TRUE;
   }

   /* Check if everything is okay */
   if (!boFailed)
   {
      /* Save context */
      pstContext->hHostDoneEvent = hHostDoneEvent;
      pstContext->hClientDoneEvent = hClientDoneEvent;
   }
   else
   {
      /* Clean up */
      SHMEM_vDeinitAccessContext(pstContext);
   }

   return !boFailed;
}

/** Initialises communication events between the Host and Legacy Module
*/
BOOL SHMEM_boInitLegacySyncContext(SHMEM_tstSyncContext *pstContext)
{
    return SHMEM_boInitSyncContext(pstContext, &SHMEM_LegacySyncEvents);
}

/** Initialises communication events between the Host and HMI Module
*/
BOOL SHMEM_boInitHmiSyncContext(SHMEM_tstSyncContext *pstContext)
{
    return SHMEM_boInitSyncContext(pstContext, &SHMEM_HmiSyncEvents);
}

/** Destroy the synchronization objects
*/
void SHMEM_vDeinitSyncContext(SHMEM_tstSyncContext *pstContext)
{
   if (pstContext->hHostDoneEvent != NULL)
   {
      CloseHandle(pstContext->hHostDoneEvent);
      pstContext->hHostDoneEvent = NULL;
   }

   if (pstContext->hClientDoneEvent != NULL)
   {
      CloseHandle(pstContext->hClientDoneEvent);
      pstContext->hClientDoneEvent = NULL;
   }
}



/**
*/
void SHMEM_vSetHostDone(SHMEM_tstSyncContext *pstContext)
{
   if (pstContext->hHostDoneEvent != NULL)
      SetEvent(pstContext->hHostDoneEvent);
}

/**
*/
void SHMEM_vClearHostDone(SHMEM_tstSyncContext *pstContext)
{
   if (pstContext->hHostDoneEvent != NULL)
      ResetEvent(pstContext->hHostDoneEvent);
}

/**
*/
BOOL SHMEM_boIsHostDone(SHMEM_tstSyncContext *pstContext)
{
   return SHMEM__boWaitForObject(pstContext->hHostDoneEvent, 0);
}

/**
*/
BOOL SHMEM_boWaitForHostDone(SHMEM_tstSyncContext *pstContext)
{
   return SHMEM__boWaitForObject(pstContext->hHostDoneEvent, INFINITE);
}


/**
*/
void SHMEM_vSetClientDone(SHMEM_tstSyncContext *pstContext)
{
   if (pstContext->hClientDoneEvent != NULL)
      SetEvent(pstContext->hClientDoneEvent);
}

/**
*/
void SHMEM_vClearClientDone(SHMEM_tstSyncContext *pstContext)
{
   if (pstContext->hClientDoneEvent != NULL)
      ResetEvent(pstContext->hClientDoneEvent);
}

/**
*/
BOOL SHMEM_boIsClientDone(SHMEM_tstSyncContext *pstContext)
{
   return SHMEM__boWaitForObject(pstContext->hClientDoneEvent, 0);
}

/**
*/
BOOL SHMEM_boWaitForClientDone(SHMEM_tstSyncContext *pstContext)
{
   return SHMEM__boWaitForObject(pstContext->hClientDoneEvent, INFINITE);
}

/**
*/
///BOOL SHMEM_boWaitForClientsDone(const SHMEM_tstAccessContext** pstContexts)
///{
///    const HANDLE hClientDoneEvents[2] = { pstContexts[0]->hClientDoneEvent, pstContexts[1]->hClientDoneEvent };
///    return SHMEM__boWaitForObjects(hClientDoneEvents, INFINITE);
///}

