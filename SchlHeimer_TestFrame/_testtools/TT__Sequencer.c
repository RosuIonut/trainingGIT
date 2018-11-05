/**
 *  \file TT__Sequencer.c
 *  \brief Brief
 */
#include "TT__Sequencer.h"    // own header
#include "TT__ExecWrapper.h"  // functionality for ExecFunction test calls
#include "SHMEM_Layout.h"     // shared memory for data exchagne with Schleißheimer tools
#include "SHMEM_Access.h"     // do get access to NO_OF_CLIENTS and sema definitions
#include "TT_Interface.h"     // do get access to TT_tenEventIds (e.g. TT_NextStepLegacy)

#include "EVHD_C1.h"          // Eventhaendler 
#include "DPOOL_C1.h"         // data pool
#include "ZpmMi_c1.h"         // ZPMMI component (module legacy)
#include <windows.h>          // Semaphore access

/* Timeout for "long press 800 ms" */
#define TT__nLongPress800ms    800
/* Timeout for "long press 2000 ms" */
#define TT__nLongPress2000ms  2000
/* Timeout for "long press 3000 ms" */
#define TT__nLongPress3000ms  3000

typedef enum {
   TT__nenKeyNotPressed,
   TT__nenKeyPressed,
   TT__nenKeyLongPress800ms,
   TT__nenKeyLongPress2000ms,
   TT__nenKeyLongPress3000ms,
} TT__tenKeyEvent;

static struct {
   bool boActive;
   bool boStepActive;
   SHMEM_tstSeqCommand *pstCommands;
   SHMEM_tstSeqCommand *pstCurrCmd;
   uint16               u16CmdCount;
   uint16               u16CurrKeyPressTime;
   TT__tenKeyEvent      enCurrKeyEvent;
} TT__stSequencer;

//----------------------------------------------------------------------------------------------------------------------------
// Initialiser utilities for MFL and FAS CAN DPOOL data
//----------------------------------------------------------------------------------------------------------------------------
/**  Initialise the DPOOL shared memory block for MFL CAN
 */
static void TT_vDPOOL_InitialisePoolData_MFLCan()
{
   //MFL CAN data structure for initialisation
   CUHIC_tMFLCANData temp;

   //set the members to zero
   temp.EventCode1   = 0;
   temp.EventCode2   = 0;
   temp.TastenCode1  = 0;
   temp.TastenCode2  = 0;
   temp.MflMarke     = 0;
   temp.MflVariante  = 0;
   
   //set the DPOOL MFL CAN data-structure to the inital values (zeros)
   DPOOL_enInitialisePoolData_DPOOL_nDataCUHIC_MFLCANData(&temp); 
}

/**  Initialise the DPOOL shared memory block for FAS CAN
 */
static void TT_vDPOOL_InitialisePoolData_FASCan()
{
   //MFL CAN data structure for initialisation
   CUHIC_tFASCANData temp;

   //set the members to zero
   temp.FASTaster        = 0;
   temp.FASTasterFehler  = 0;
   //temp.reserved         = 0; comment out here, since should not set value of reserved flag?
   
   //set the DPOOL MFL CAN data-structure to the inital values (zeros) 
   DPOOL_enInitialisePoolData_DPOOL_nDataCUHIC_FASCANData(&temp); 
}
//----------------------------------------------------------------------------------------------------------------------------
// END: Initialiser utilities for MFL and FAS CAN DPOOL data
//----------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
// Utilities for MFL and FAS CAN DPOOL data
//----------------------------------------------------------------------------------------------------------------------------

/**  Set MFL CAN EventCode1
 */
static void TT__vDPOOL_SetMFLCANData_EventCode1(const uint8 u8Value)
{
   CUHIC_tMFLCANData temp;
   
   DPOOL_enGetPoolData_DPOOL_nDataCUHIC_MFLCANData(&temp);
   temp.EventCode1 = u8Value;
   DPOOL__enSetXData(DPOOL_nDataCUHIC_MFLCANData, &temp, &DPOOL__xCUHIC_MFLCANDataData);             
}

/**  Set MFLCAN TastenCode1
 */
static void TT__vDPOOL_SetMFLCANData_TastenCode1(const uint8 u8TastenCode)
{
  CUHIC_tMFLCANData temp;
   
  DPOOL_enGetPoolData_DPOOL_nDataCUHIC_MFLCANData(&temp);
  temp.TastenCode1 = u8TastenCode;
  DPOOL__enSetXData(DPOOL_nDataCUHIC_MFLCANData, &temp, &DPOOL__xCUHIC_MFLCANDataData);          
}

/**  Set FAS CAN FASTaster
 */
static void TT__vDPOOL_SetData_FASCan_FASTaster(const uint8 u8Value)
{
   CUHIC_tFASCANData temp;
   
   DPOOL_enGetPoolData_DPOOL_nDataCUHIC_FASCANData(&temp);
   temp.FASTaster = u8Value;
   DPOOL__enSetXData(DPOOL_nDataCUHIC_FASCANData, &temp, &DPOOL__xCUHIC_FASCANDataData);             
}
//--------------------------------------------------------------------------------------------------
// END: Setter and Getter utilities for MFL and FAS CAN DPOOL data
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Implementation of TT__Sequencer.c functions
//--------------------------------------------------------------------------------------------------
/**  Init function for the sequencer
 */
void TT__vSequencerInit(void)
{
   TT__stSequencer.boActive = False;
   TT__stSequencer.boStepActive = False;
   TT__stSequencer.enCurrKeyEvent = TT__nenKeyNotPressed;   
}

/**  Intialise data structures
 */
void TT__vSequencerOnDownload(void *pvData, uint32 u32Length)
{
   SHMEM_tstSeqHeader *pstHeader = pvData;

   if (pstHeader->u16CmdCount > 0)
   {
      TT__stSequencer.pstCommands = (SHMEM_tstSeqCommand*) (pstHeader + 1);
      TT__stSequencer.pstCurrCmd = TT__stSequencer.pstCommands;
      TT__stSequencer.u16CmdCount = pstHeader->u16CmdCount;
      TT__stSequencer.boActive = True;
      TT__stSequencer.boStepActive = True;  //20170118 wti
      
      //--------------------------------------------------------------------------------------------
      //FPKE
      TT_vDPOOL_InitialisePoolData_MFLCan();
      TT_vDPOOL_InitialisePoolData_FASCan();

      //FPKS
      //CAN_vOutMFL_Marke(0);
      //CAN_vOutMFL_Variante(0);
      //CAN_vOutMFL_Tastencode_2(0);
      //CAN_vOutMFL_Eventcode_2(0);
      //CAN_vOutFAS_Taster_Fehler(0);
      //--------------------------------------------------------------------------------------------
   }
}

/**  Load queries for sequencer
 */
bool TT__boSequencerQueryUpload(void *pvData, uint32 u32Length)
{
   /* Prevent client to be "done" as long as sequencer runs */
   return !TT__stSequencer.boActive;
}

/**  Performed when the MRTS command "sleep" is executed.
 *    handles the fact that sleep times are in msec, but the 
 */
static bool TT__boSeqOnSleep(uint16 *pMilliSec)
{
   /* Sleep argument is specified in milliseconds, but our cycle time is 10ms.
    */
   // wti: changed from 10 to 20 because the cycle time will need 10ms, so wait can be 1 cycle shorter
   if ((*pMilliSec) >= 20)
   {
      (*pMilliSec) -= 10;
   }
   else
   {
      (*pMilliSec) = 0;
   }
   
   /* Return 'true' if timeout has elapsed */
   return (0 == (*pMilliSec));
}

/**  Sets the CAN signed event code, called when a key is pressed or released
 */
static void TT__vSetSignalEventcode(TT__tenKeyEvent enKeyEvent)
{
#if (IDE_nProcess == SYSCONST_nProcessLegacy)   // Lecacy only
   uint8 u8EventForCan;

   if (TT__stSequencer.enCurrKeyEvent != enKeyEvent)
   {
      /* Translate internal key event enum into signal data value */
      switch (enKeyEvent)
      {
         case TT__nenKeyPressed:          u8EventForCan = 0x01; break;
         case TT__nenKeyLongPress800ms:   u8EventForCan = 0x04; break;
         case TT__nenKeyLongPress2000ms:  u8EventForCan = 0x05; break;
         case TT__nenKeyLongPress3000ms:  u8EventForCan = 0x06; break;
         default:
         case TT__nenKeyNotPressed:       u8EventForCan = 0x00; break;
      }
      
      //--------------------------------------------------------------------------------------------
      //FPKE      
      TT__vDPOOL_SetMFLCANData_EventCode1(u8EventForCan);
      //--------------------------------------------------------------------------------------------
        
      // /* Set Eventcode */
      // CAN_vOutMFL_Eventcode_1(u8EventForCan);
      // /* Notify that MFL keys have changed */
      // EVHD_vSendEvent(EV_CanMFLKeysChanged);
      
      /* Store new state */
      TT__stSequencer.enCurrKeyEvent = enKeyEvent;
   }
#endif
}

/**  Times key pressses
 */
static void TT__vUpdateKeyPressTimer(void)
{
   uint16 u16Time;

   if (TT__stSequencer.enCurrKeyEvent != TT__nenKeyNotPressed)
   {
      /* count timer until longest longpress is reached */
      if (TT__stSequencer.u16CurrKeyPressTime < TT__nLongPress3000ms)
      {
         TT__stSequencer.u16CurrKeyPressTime += 10;
      }
      u16Time = TT__stSequencer.u16CurrKeyPressTime;
      /* 0 to 800 ms => "normal" press */
      if (u16Time > 0 && u16Time < TT__nLongPress800ms)
      {
         TT__vSetSignalEventcode(TT__nenKeyPressed);
      }
      /* 800 to 2000 ms => long press 800 ms */
      if (u16Time >= TT__nLongPress800ms && u16Time < TT__nLongPress2000ms)
      {
         TT__vSetSignalEventcode(TT__nenKeyLongPress800ms);
      }
      /* 2000 to 3000 ms => long press 2000 ms */
      if (u16Time >= TT__nLongPress2000ms && u16Time < TT__nLongPress3000ms)
      {
         TT__vSetSignalEventcode(TT__nenKeyLongPress2000ms);
      }
      /* anything above 3000 ms => long press 3000 ms */
      if (u16Time >= TT__nLongPress3000ms)
      {
         TT__vSetSignalEventcode(TT__nenKeyLongPress3000ms);
      }
   }
}

/** Called when the MRTS script contains a KeyPress command
 */
static void TT__vSeqOnKeyPress(SHMEM_tstSeqCommand *pstCmd)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // Legacy only 
    TT_vTraceEvent(TT_KeyPress, pstCmd->u16Argument);
    if (pstCmd->u16Argument == SHMEM_nSeqKey_MFL_FAS)
    {
        TT__vDPOOL_SetData_FASCan_FASTaster(1);
        EVHD_vSendEvent(EV_DPOOL_Always_CUHIC_FASCANData);   

      // /* Code for pressed key */
      // CAN_vOutFAS_Taster(1);
      // /* Notify that FAS key has changed */
      // EVHD_vSendEvent(EV_CanFASKeyChanged);
    }
    else
    {
      /* Code for pressed key */
      TT__vDPOOL_SetMFLCANData_TastenCode1( (uint8)pstCmd->u16Argument );

      /* Restart key press timer */
      TT__stSequencer.u16CurrKeyPressTime = 0;
      /* Set event code initially to "pressed" */
      TT__vSetSignalEventcode(TT__nenKeyPressed);
    }
#endif
}

/**  Called when the MRTS script contains a KeyRelease command
 */
static void TT__vSeqOnKeyRelease(SHMEM_tstSeqCommand *pstCmd)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // Legacy only 
    TT_vTraceEvent(TT_KeyRelease, pstCmd->u16Argument);
    if (pstCmd->u16Argument == SHMEM_nSeqKey_MFL_FAS)
    {
        //--------------------------------------------------------------------------------------------
        TT__vDPOOL_SetData_FASCan_FASTaster(0);
        EVHD_vSendEvent(EV_DPOOL_Always_CUHIC_FASCANData);   
          
        /* Code for pressed key */
        //CAN_vOutFAS_Taster(0);
        /* Notify that FAS key has changed */
        //EVHD_vSendEvent(EV_CanFASKeyChanged);
        //--------------------------------------------------------------------------------------------
    }
    else
    {     
        /* Set event code to "not pressed" */
        TT__vDPOOL_SetMFLCANData_TastenCode1( (uint8)pstCmd->u16Argument );
        
        /* Set event code to "not pressed" */
        TT__vSetSignalEventcode(TT__nenKeyNotPressed);
    }
#endif
}

/**  Called when the MRTS script contains a WarnRequest command
 */
static void TT__vSeqOnWarnRequest(SHMEM_tstSeqCommand *pstCmd)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // Legacy only 

   uint16 u16WarnCode = pstCmd->u16Argument;   
   uint8 my_activation[] = { 1, 0 };
   my_activation[0] = 1;
   ZPMMI_vDiaInterface(0x41, u16WarnCode >> 8, u16WarnCode & 0xff, my_activation);
   
#endif
}

/**  Called when the MRTS script contains a WarnRelease command
 */
static void TT__vSeqOnWarnRelease(SHMEM_tstSeqCommand *pstCmd)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // Legacy only 

   uint16 u16WarnCode = pstCmd->u16Argument;   
   uint8 my_activation[] = { 1, 0 };
   my_activation[0] = 0;
   ZPMMI_vDiaInterface(0x41, u16WarnCode >> 8, u16WarnCode & 0xff, my_activation);

#endif
}

/** Called when the MRTS script contains a ExecFunction command
 *  
 *  Called when the MRTS script contains a ExecFunction command. This method implements
 *  parameterless calls to TT functiona defined by the developer / tester. For further information see
 *  the file ExecWrapper in the same directory as this file.
 */
static void TT__vSeqOnExecFunction(SHMEM_tstSeqCommand *pstCmd)
{
// TT__vExecFunction(pstCmd->u16Argument); // 20170307 wti: added 2nd argument
   TT__vExecFunction( pstCmd->u16Argument, pstCmd->u16Argument2 );
}

/**  Called when the MRTS keywords "hmiwarn or hmiwarnoff" are processes by the Schleißhimer tool
 */
static void TT__vSeqOnHmiWarn(SHMEM_tstSeqCommand *pstCmd, unsigned char a_on)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // Legacy only 
//Exclude this code for HMI, otherwise get linked errors since ZPMMI_vDiaInterface not available in HMI
      
   /* Translate key code into CAN signal value */
   uint16 u16WarnCode = pstCmd->u16Argument;   
   uint8 my_activation[] = { 1, 0 };
   my_activation[0] = a_on;
   ZPMMI_vDiaInterface(0x41, u16WarnCode >> 8, u16WarnCode & 0xff, my_activation);
#endif
} 

//###+++ wti +++###########################################
/** Return if step is active and can get performed
*/
static bool TT__vStepActive( void )
{
    if (False == TT__stSequencer.boStepActive)
    {
        if (NUM_OF_CLIENTS == TT__stSequencer.pstCurrCmd->u8Processed)
        {   // All client did this step
            // Advance to next command 
            TT__stSequencer.pstCurrCmd++;

            /* Check if sequence end has been reached */
            if (TT__stSequencer.pstCurrCmd >= &TT__stSequencer.pstCommands[TT__stSequencer.u16CmdCount])
            {
                TT__stSequencer.boActive = False;
             // TT__stSequencer.boStepActive = False;  // is already false
            }
            else
            {
                TT__stSequencer.boStepActive = True;
            }
        }
    }
    
    return TT__stSequencer.boStepActive;
}

/**  Finalize step  (replaces TT_vNextStep)
 */
static void TT__vFinishStep(void)
{
    if (TT__stSequencer.pstCurrCmd != NULL)
    {
        // Mark current command as processed
        TT__stSequencer.pstCurrCmd->u8Processed++;
        // ### Currently without mutex - write access 
        // ### Could lead to an hang if operation is not atomic

        // Step is done
        TT__stSequencer.boStepActive = False;
    }
}
//###--- wti ---###########################################

/**  Entry point for the TT__Sequencer. Called every 10msec by the framwork.
*
*  Entry point for the regular polling calls to the TT__Sequencer. Called every 10msec by 
*  the framwork. This function is registered with the framework by making the entry: 
*  CAM  DEFINE ACTIVITY TT_vCyclic10ms   
*  IS OF TYPE CYCLIC EXECUTED IN Every10ms NEEDS RESOURCE STATES : {}  SEQUENCE 100 RUNTIME IS 0;
*  in the S(ystem) D(efines) H(eader) file: TT.sdh in the same directory.
*/
void TT__vSequencerCyclic10ms(void)
{
    static bool     boSleeping = False;
    static uint16   u16MilliSec;
    
    if ( TT__stSequencer.boActive && TT__vStepActive() )
    {
        TT__vUpdateKeyPressTimer();

        /* Still active and not sleeping? */
        if ( TT__stSequencer.boActive && (False == boSleeping) )
        {
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
//          TT_vTraceEvent(TT_NextStepLegacy, TT__stSequencer.pstCurrCmd->u8ActionCode);
#endif
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)   // only for Hmi
//          TT_vTraceEvent(TT_NextStepHmi, TT__stSequencer.pstCurrCmd->u8ActionCode);
#endif
            /* Now check what to do */
            switch (TT__stSequencer.pstCurrCmd->u8ActionCode)
            {
            case SHMEM_nSeqCodeStartSync:
                // No real action - only to sync with other clients
                break;
            case SHMEM_nSeqCodeSleep:
                u16MilliSec = TT__stSequencer.pstCurrCmd->u16Argument;
                boSleeping = True;                
                break;
            case SHMEM_nSeqCodeKeyPress:
                TT__vSeqOnKeyPress(TT__stSequencer.pstCurrCmd);
                break;
            case SHMEM_nSeqCodeKeyRelease:
                TT__vSeqOnKeyRelease(TT__stSequencer.pstCurrCmd);
                break;
            case SHMEM_nSeqCodeWarnRequest:
                TT__vSeqOnWarnRequest(TT__stSequencer.pstCurrCmd);
                break;
            case SHMEM_nSeqCodeWarnRelease:
                TT__vSeqOnWarnRelease(TT__stSequencer.pstCurrCmd);
                break;
            case SHMEM_nSeqCodeHmiWarn:
                TT__vSeqOnHmiWarn(TT__stSequencer.pstCurrCmd, 1);
                break;
            case SHMEM_nSeqCodeHmiWarnOff:
                TT__vSeqOnHmiWarn(TT__stSequencer.pstCurrCmd, 0);
                break;
            case SHMEM_nSeqCodeExecFunction:
                TT__vSeqOnExecFunction(TT__stSequencer.pstCurrCmd);
                break;
            }
        }
        
        /* Manage wait */
        if ( True == boSleeping )
        {
            if ( True == TT__boSeqOnSleep( &u16MilliSec ) )
            {
                boSleeping = False;
            }
        }

        if ( False == boSleeping )
        {   /* Finialize step */
            TT__vFinishStep();
        }
    }
}

//
//--------------------------------------------------------------------------------------------------
// END: TT__Sequencer.c functionality
//--------------------------------------------------------------------------------------------------
