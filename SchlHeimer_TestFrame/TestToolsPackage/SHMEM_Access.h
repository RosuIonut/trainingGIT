#ifndef SHMEM_ACCESS_H
#define SHMEM_ACCESS_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//#undef On
#include <windows.h>

/* Access Synchronization
 *
 * The shared memory block can be accessed by both parties, the host software
 * (e.g. SimInterface) and the client software (e.g. TestTools in the PC
 * based test frame). To guarantee data integrity, it is necessary that each
 * one knows when it is safe to read/write the data.
 *
 * The following sequence is the common use case for the shared memory block:
 *
 * 1.  Host writes into the block
 * 2.  Host sets 'HostDone' event
 * 3.  Client sees and clears 'HostDone' event
 * 4.  Client reads from the block
 * 5.  Client writes into the block
 * 6.  Client sets 'ClientDone' event
 * 7.  Host sees and clears 'ClientDone' event
 * 8.  Host reads from the block
 * 9.  Continue with step 1.
 *
 * For accessing the three shared objects (memory block, HostDone event,
 * ClientDone event) from both processes, the following names will be used.
 */
#define SHMEM_rsNameMemoryBlock      TEXT("FPK_IntTest_SHMEM_MemoryBlock")
//#define SHMEM_rsNameHostDoneEvent    TEXT("FPK_IntTest_SHMEM_HostDoneEvent")
//#define SHMEM_rsNameClientDoneEvent  TEXT("FPK_IntTest_SHMEM_ClientDoneEvent")

#define SHMEM_rsNameClientStepActiveSema    TEXT("FPK_IntTest_SHMEM_ClientStepActiveSema")
#define SHMEM_rsNameClientStepDoneSema      TEXT("FPK_IntTest_SHMEM_ClientStepDoneSema")
#define SHMEM_rsNameClientWriteMutex        TEXT("FPK_IntTest_SHMEM_ClientWriteMutex")
#define NUM_OF_CLIENTS               2

/* Functions to provide access control */
typedef struct {
   PVOID pMemoryBlock;
   HANDLE hMemoryMapping;
//   HANDLE hHostDoneEvent;
//   HANDLE hClientDoneEvent;
   HANDLE hWriteMutex;
} SHMEM_tstAccessContext;

typedef struct {
   HANDLE hHostDoneEvent;
   HANDLE hClientDoneEvent;
} SHMEM_tstSyncContext;

typedef struct SHMEM_SyncEventsStruct tSHMEM_SyncEventsStruct;

/* Function to manage shared memory access */
BOOL SHMEM_boInitAccessContext(  SHMEM_tstAccessContext *pstContext );
void SHMEM_vDeinitAccessContext( SHMEM_tstAccessContext *pstContext );

/* Functions to manage synchronization events */
BOOL SHMEM_boInitLegacySyncContext( SHMEM_tstSyncContext *pstContext );
BOOL SHMEM_boInitHmiSyncContext(    SHMEM_tstSyncContext *pstContext );
void SHMEM_vDeinitSyncContext(      SHMEM_tstSyncContext *pstContext );


/* Functions to manage HostDone event */
void SHMEM_vSetHostDone(      SHMEM_tstSyncContext *pstContext );
void SHMEM_vClearHostDone(    SHMEM_tstSyncContext *pstContext );
BOOL SHMEM_boIsHostDone(      SHMEM_tstSyncContext *pstContext );
BOOL SHMEM_boWaitForHostDone( SHMEM_tstSyncContext *pstContext );

/* Functions to manage ClientDone event */
void SHMEM_vSetClientDone(      SHMEM_tstSyncContext *pstContext );
void SHMEM_vClearClientDone(    SHMEM_tstSyncContext *pstContext );
BOOL SHMEM_boIsClientDone(      SHMEM_tstSyncContext *pstContext );
BOOL SHMEM_boWaitForClientDone( SHMEM_tstSyncContext *pstContext );

///BOOL SHMEM_boWaitForClientsDone(const SHMEM_tstAccessContext** pstContexts);


#endif
