// This version of ShMemManager is adapted to two clients (HMI and Legacy)

extern "C"
{
#include "SHMEM_Layout.h"
#include "SHMEM_Access.h"
}

#include <lib-util.hpp>
#include "ShMemManager.hpp"

bool ShMemManager::boConnect(void)
{
///    return (SHMEM_boInitLegacyAccessContext(&m_stLegacyAccessContext) != 0) 
///        && (SHMEM_boInitHMIAccessContext(&m_stHMIAccessContext) != 0);

                    // Get SharedMemory access
    m_boConnected = (SHMEM_boInitAccessContext(&m_stAccessContext) != false)
                    // Initialize sync events
                 && (SHMEM_boInitLegacySyncContext(&m_stLegacySyncContext) != false)
                 && (SHMEM_boInitHmiSyncContext(&m_stHmiSyncContext) != false);
    return m_boConnected;
}

void ShMemManager::vDisconnect(void)
{
   if (m_boConnected)
   {
      SHMEM_vDeinitAccessContext(&m_stAccessContext);
      SHMEM_vDeinitSyncContext(&m_stLegacySyncContext);
      SHMEM_vDeinitSyncContext(&m_stHmiSyncContext);
      m_boConnected = false;
   }
}

bool ShMemManager::boSendRequest(ShMemBlock &oBlock)
{
   bool boFailed = false;

   if (m_boConnected)
   {
      ByteBuffer oBuffer;

      // 1. Get data to store
      oBlock.vSerialize(&oBuffer);

      // 2. Write data into shared memory block (if it fits)
      if (oBuffer.u32GetTotalSize() < SHMEM_nMaxSharedObjectSize)
      {
         if (oBuffer.boGetNextItem(m_stAccessContext.pMemoryBlock, oBuffer.u32GetTotalSize()) == false)
         {
            util::Log::vPrint(util::LOG_ERROR, "Error writing memory block!");
            boFailed = true;
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Maximum size of memory block exceeded!");
         boFailed = true;
      }

      // 3. Set HostDone event
      if (!boFailed)
      {
         SHMEM_vSetHostDone(&m_stLegacySyncContext);
         SHMEM_vSetHostDone(&m_stHmiSyncContext);
      }
   }
   else
   {
      boFailed = true;
   }

   return !boFailed;
}

///bool ShMemManager::boSendLegacyRequest(ShMemBlock& oBlock)
///{
///   return (m_boConnected && ShMemManager::boSendRequest(oBlock, m_stLegacyAccessContext));
///}
///
///bool ShMemManager::boSendHmiRequest(ShMemBlock& oBlock)
///{
///   return (m_boConnected && ShMemManager::boSendRequest(oBlock, m_stHmiAccessContext));
///}


///bool ShMemManager::boWaitForResponse()
///{
///   bool boFailed = false;
///
///   if (m_boConnected)
///   {
/////      // Wait until both clients (Legacy and HMI) report that they have finished processing the test commands
/////      SHMEM_boWaitForClientsDone(m_Contexts);
///
///      // Wait until clients (Legacy and HMI) report that they have finished processing the test commands
///      SHMEM_boWaitForClientDone( &m_stLegacySyncContext );
///      SHMEM_boWaitForClientDone( &m_stHmiSyncContext );
///      // Clear the client done flags
///      SHMEM_vClearClientDone( &m_stLegacySyncContext );
///      SHMEM_vClearClientDone( &m_stHmiSyncContext );
///   }
///   else
///   {
///      boFailed = true;
///   }
///
///   return !boFailed;
///}

bool ShMemManager::boReceiveResponse(ShMemBlock *poBlock)
{
   bool boFailed = false;

   if (m_boConnected)
   {
      // 1. Wait until clients (Legacy and HMI) reports done and clear flags
      SHMEM_boWaitForClientDone( &m_stLegacySyncContext );
      SHMEM_boWaitForClientDone( &m_stHmiSyncContext );
      SHMEM_vClearClientDone( &m_stLegacySyncContext );
      SHMEM_vClearClientDone( &m_stHmiSyncContext );

      // 2. Read results from memory
      ByteBuffer oBuffer;
      SHMEM_tstShMemHeader *pstHeader = (SHMEM_tstShMemHeader*) m_stAccessContext.pMemoryBlock;
      oBuffer.vAppendItem(m_stAccessContext.pMemoryBlock, pstHeader->u32ObjSize);
      poBlock->boDeserialize(&oBuffer);
   }
   else
   {
      boFailed = true;
   }

   return !boFailed;
}

///bool ShMemManager::boReceiveLegacyResponse(ShMemBlock* poBlock)
///{
///    return m_boConnected && boReceiveResponse(poBlock, m_stLegacyAccessContext);
///}
///
///bool ShMemManager::boReceiveHMIResponse(ShMemBlock* poBlock)
///{
///    return m_boConnected && boReceiveResponse(poBlock, m_stHmiAccessContext);
///}
