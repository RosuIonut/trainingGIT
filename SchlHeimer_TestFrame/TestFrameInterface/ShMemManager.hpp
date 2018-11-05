#ifndef SHMEMMANAGER_HPP
#define SHMEMMANAGER_HPP

extern "C"
{
#include "SHMEM_Access.h"
}

#include "ShMemBlock.hpp"

// The ShMemManager is a thin wrapper around the SHMEM_Access functions that
// provides send and receives functions that wrap the synchronization.

class ShMemManager
{
public:
   ShMemManager() 
   {
   }

   virtual ~ShMemManager()
   {
   }

   bool boIsConnected(void)
   {
      return m_boConnected;
   }

   bool boConnect(void);
   void vDisconnect(void);

//   bool boSendLegacyRequest(    ShMemBlock &oBlock );
//   bool boSendHMIRequest(       ShMemBlock &oBlock );
   bool boSendRequest(          ShMemBlock &oBlock );
   bool boReceiveResponse(      ShMemBlock *poBlock );
//   bool boReceiveLegacyResponse(ShMemBlock* poBlock );
//   bool boReceiveHMIResponse(   ShMemBlock* poBlock );
//   bool boWaitForResponse();

private:
//   static bool boSendRequest(ShMemBlock& oBlock, SHMEM_tstAccessContext& oAccessContext);
//   static bool boReceiveResponse(ShMemBlock* poBlock, SHMEM_tstAccessContext& oAccessContext);

   SHMEM_tstSyncContext   m_stLegacySyncContext;
   SHMEM_tstSyncContext   m_stHmiSyncContext;
   SHMEM_tstAccessContext m_stAccessContext;
   bool m_boConnected;

};

#endif
