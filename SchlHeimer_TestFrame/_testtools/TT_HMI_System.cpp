#include "ipc_connecthdl.h"
#include "cpcont_HMITestToolscbgen.h"
#include "cpcont_HMITestToolsgen.h"
#include "main_c1.h" // for IPCDispatcher

// debug
#include "ace/Log_Msg.h"

//the entry functions for the test tool
extern "C"
{
#include "TT_System.h"
}

#include "stdio.h"

void TestTool_Cyclic_10ms(void);

/* =============== initial code ====================
 * .xdh=> 'initial call;' 
 */
bool HMITestTools_Callbacks::boInitialCall()
{
   //call the init / Reset (Wakeup / RESET) method of the ST-Component HMI module
   TT_vHMIInitReset();
   
   //---------------------------------------------------------------------------------
   // set timer to 10,0ms 
   m_oSignals.boSetTimeout(CC_IPC::TI_HMITestTools_10ms, 0, 10000, true);
   //
   //---------------------------------------------------------------------------------

// 20170315 wti: NOT USED
//   //---------------------------------------------------------------------------------
//   // set timer to 100,0ms 
//   m_oSignals.boSetTimeout(CC_IPC::TI_HMITestTools_100ms, 0, 100000, true);
//   //
//   //---------------------------------------------------------------------------------     
       
   //
   return true;
}

//------------------------------------------------------------------
// general timeout callback for all timer according to ipc_idgen.h
bool HMITestTools_Callbacks::boSignalSI_IPC_Timeout(char *pcUserData, size_t tsSize)
{
#if 0 //wti
    //get the timer ID
   CC_IPC::enTimer_t enTimerId = *((CC_IPC::enTimer_t*)pcUserData);
   switch (enTimerId)
   {
   case CC_IPC::TI_HMITestTools_10ms:  TT_vHMICyclic10ms();  break; 
   case CC_IPC::TI_HMITestTools_100ms: TT_vCyclic100ms();    break; 
   }
#endif
   TestTool_Cyclic_10ms();  //20170315 wti
   return true;
}
//----------------------------------------------------------------
// handle signals by a general callback with switch case
// usually you use individual signal callbacks or this general callback
// with 'initial (currently not used)
bool HMITestTools_Callbacks::boProcessReceivedSignal(enSignalId_t enSignal, char *pcUserData, size_t tsSize)
{
    return true;
}


//----------------------------------------------------------------
// Function called every 10ms to handle the test tool tasks
void TestTool_Cyclic_10ms( void )
{
    static uint32 cycleCounter = 0;

    cycleCounter++;
    switch (cycleCounter)
    {
    case 10:
        cycleCounter = 0;
        TT_vCyclic100ms();
        // fall thru to 10ms routine
    default:
        TT_vCyclic10ms();
        break;
    }
}