/*** Includes *************************************************************************************/
extern "C"
{
#include "sysconst.h"
#include "SHMEM_Layout.h"
#include "DPOOL_C1.h"
#include "ipc_idgen.h"    // for the rx signal ids
#include "dpool__gen2ct.h"
#include "crhdl_genct.h"
#include "crhdl1c1.h"
#include "API_c1.h"
#include "smgjc2.h"

// Sound test
//#include "MMi_c1.h"

/* Integration test code      w.ti*/
#include "TT_C1.h"          /*w.ti*/

// Own header file
#include "TT__ExecWrapper.h"
}

#include "TT_ExecFunctions.hpp"



#if (defined SYSCONST_nProcessBaseSystem)
#if (IDE_nProcess == SYSCONST_nProcessHMI)    // only for HMI
    #include "WAS_Service.hpp"
    //#include "cpcont_hmi_smshgen.h"
    //#include "cpcont_hmi_smcbgen.h"
    #include "cpcont_hmi_compcbgen.h"
#endif

#if (IDE_nProcess == SYSCONST_nProcessLegacy)    // only for Legacy
#endif
#endif

/*** Definitions **********************************************************************************/
/* Our own declaration of a function that exists in SmgIc1.c.
 * Not very nice, but SimEnv does the same for the KL15 button.
 */
extern void APIM_vSystemEventOccured_internal(CC_IPC::enSignalId_t ev);
extern "C" void SMG__vSwitchKl15Tg(bool boKl15);

extern "C" void DIA__vActuatorTest(DPOOL_txMSGHdl_tstKiDiagIcomBuffer * pstReq);
extern "C" TRACE_tCmdReturnCode APIMGR_xTraceCmdSetAPIValue(uint16 u16ApifunctionID, uint8 u8TimeDomain, void* ptrApiValue, uint8 u8Len);

extern void HMIA_vChangeHmiDependencies(void);

/*** Functions ************************************************************************************/
static void TT__vDebugFunction(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 1);  

    //HMI_Comp_Callbacks::boSignalSI_EV_CRHDL_InitRES_HMI_Framwork_ANY();
    //m_oSignal.boSendSignalWithoutData(CC_IPC::SI_EV_CRHDL_InitRES_HMI_Framwork_ANY);
#endif
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)  // only for Legacy
    static BAPA_tstAUDIOCurrentVolume          AudioVol = { 0, 0, 0, 0, 0, BAPA_enFIDAvailable };
    static BAPA_tstAUDIOCurrentVolumeType      AudioVolType = { 0x0, BAPA_enFIDAvailable };   // Entertainment volume changed
    static bool                                volumeUp = true;

    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 0);

    AudioVolType.u8ChangingVolumeType = 0x2;
    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolumeType, &AudioVolType, &DPOOL__xBAPA_mAUDIOCurrentVolumeTypeData);

    if (true == volumeUp)
    {
        AudioVol.u8EntertainmentVolume++;
        if (AudioVol.u8EntertainmentVolume >= 20)  volumeUp = false;
    }
    else
    {
        AudioVol.u8EntertainmentVolume--;
        if (AudioVol.u8EntertainmentVolume == 0)    volumeUp = true;
    }

    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolume, &AudioVol, &DPOOL__xBAPA_mAUDIOCurrentVolumeData);

    AudioVolType.u8ChangingVolumeType = 0x1;
    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolumeType, &AudioVolType, &DPOOL__xBAPA_mAUDIOCurrentVolumeTypeData);

    //BAPAPI_AUDIO__vSendSystemEvent(SYSTEM_VOLUME_TYPE_CHANGED);
    EVHD_vSendEvent(EV_APIM_SysEv_SYSTEM_VOLUME_TYPE_CHANGED);
#endif
}

static void TT__vExecKL15On(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, On);
    SMG__vSwitchKl15(On);
#endif
}

static void TT__vExecKL15Off(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, Off);
    SMG__vSwitchKl15(Off);
#endif
}

static void TT__vFillMessageQueue(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 0);
    for (int i = 0; i < 915; i++)               // should be at least 90%
     // HMI::WAS::Framework::PostMessage(HMI::WRS::LAPTIMER_HOUR_MODE_REACHED, HMI::WCS::enMessageType::GLOBAL_BROADCAST_MESSAGE);
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);
#endif
}

static void TT__vAddMessageQueueDelayItems(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 1);
    for (int i = 0; i < 55; i++)                // more than 3%
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_AUDIO_INFO_STATE_CHANGED);
#endif
}

static void TT__vAddMessageQueueDiscardItems(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 2);
    for (int i = 0; i < 55; i++)                // more than 3%
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_EVENT_IP_CHANGED);
        /* APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_MFA_IDLE_TO_MENU_TRIGGERED); */
#endif
}

static void TT__vAddMessageQueuePassItems(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, 3);
    for (int i = 0; i < 55; i++)                // more than 3%
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);
#endif
}

static void TT__vAddMessageQueueKeyItems1(uint16 u16FuncId, uint16 dummyEntries)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, dummyEntries);
    
    for (int i = 0; i < dummyEntries; i++)       // add messages to queue before hard key events
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_BACK_LONG);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_BACK_LONG_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_DOWN_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_DOWN_SHORT_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_UP_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_UP_LONG);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_UP_LONG_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_MENU_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_MENU_SHORT_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_OK_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_OK_SHORT_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);
#endif
}

static void TT__vAddMessageQueueKeyItems2(uint16 u16FuncId, uint16 dummyEntries)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, dummyEntries);
    
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_DOWN_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_DOWN_LONG);

    for (int i = 0; i < dummyEntries; i++)       // add messages to queue before hard key events
        APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_DOWN_LONG_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);
    
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_UP_SHORT);
    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_HK_UP_SHORT_RELEASE);

    APIM_vSystemEventOccured_internal(CC_IPC::SI_EV_APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED);
#endif
}

static void TT__vIncrementTageswegstrecke(uint16 u16FuncId)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI  
    static uint32 u32Scratch = 0;
    
    u32Scratch++;
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u32Scratch);    
    APIMGR_xTraceCmdSetAPIValue(APIM_nFId_hmi_tageswegstrecke, 0, &u32Scratch, 4);
#endif
}

static void TT__vDisplayUpDown(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    SMG__tstKL15Info  stKL15Info;
    
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    DPOOL_enGetPoolData(DPOOL_nDataSMG_stKl15Info,  (SMG__tstKL15Info*)&stKL15Info );
    if ( SHMEM_nSeqParam_DisplayUp == u16Param )
        stKL15Info.s_klemme_15 = 1;
    else if ( SHMEM_nSeqParam_DisplayDown == u16Param )
        stKL15Info.s_klemme_15 = 0;
    else
        return;
    
    DPOOL__enSetXData(DPOOL_nDataSMG_stKl15Info, &stKL15Info, &DPOOL__xSMG_stKl15InfoData);
    //DPOOL_enSetPoolData(DPOOL_nDataSMG_stKl15Info,  (SMG__tstKL15Info*)&stKL15Info );
    SMG_vControllerSync( EV_DPOOL_Always_SMG_stKl15Info_SMG_vControllerSync );
#endif
}

static void TT__vHmiEnvironment(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    if ( SHMEM_nSeqParam_Disable == u16Param )
    {   // Disable HMI environment
        CRHDL_vReqHdlClearRequest(CRHDL_nReqSMRES_HoldDisplayByMEX);
        CRHDL_vReqHdlClearRequest(CRHDL_nReqSMRES_HoldDisplayBySMG);
        CRHDL_vReqHdlClearRequest(CRHDL_nReqSMRES_HMI_EnvironmentBySMG);
    }
    else if ( SHMEM_nSeqParam_Enable == u16Param )
    {   // Enable HMI environment
        CRHDL_enReqHdlSetRequest(CRHDL_nReqSMRES_HMI_EnvironmentBySMG);
    }
    else
    {
        return;
    }
#endif
}

static void TT__vOverVoltage(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    POW_tstVoltageStatus SetVoltageStatus;
  
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    if ( SHMEM_nSeqParam_Clear == u16Param )
    {
      SetVoltageStatus.POW_biOk = 1;
      SetVoltageStatus.POW_biOvervolt = 0;
    }
    else if ( SHMEM_nSeqParam_Set == u16Param )
    {
      SetVoltageStatus.POW_biOk = 0;
      SetVoltageStatus.POW_biOvervolt = 1;
    }
    else
    {
        return;
    }
    SetVoltageStatus.POW_biOvervoltReduced = 0;
    SetVoltageStatus.POW_biOvervoltReduced_t1 = 0;
    
    DPOOL__enSetXData(DPOOL_nDataVoltageStatus, &SetVoltageStatus, &DPOOL__xVoltageStatusData);
#endif
}

static void TT__vDiagnostics(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    DPOOL_txMSGHdl_tstKiDiagIcomBuffer  req;
    
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);
    
    if ( SHMEM_nSeqParam_Enable == u16Param )
    {   // Enable
        req.au8Data[0] = 5;     // RequestMiddleDisplay
        req.au8Data[1] = 3;     // Mode
    }
    else if ( SHMEM_nSeqParam_Disable == u16Param )
    {   // Disable
        req.au8Data[0] = 6;     // ReleaseMiddleDisplay
        req.au8Data[1] = 0;     // Mode    
    }
    else
    {
        return;
    }

    DIA__vActuatorTest( &req );
#endif
}

static void TT__vSelectLanguage(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    if (SHMEM_nSeqParam_DiagAdaption == u16Param)
    {
        EVHD_vSendEvent(EV_DiagAdaption_0_Change);
    }
    else if (SHMEM_nSeqParam_DiagCoding == u16Param)
    {
        EVHD_vSendEvent(EV_DiagCodingChange);
    }
    else if (SHMEM_nSeqParam_M_Language == u16Param)
    {
        EVHD_vSendEvent(EV_DPOOL_OnChange_M_Language);
    }
    else
    {
        return;
    }
#endif
}

static void TT__vSetLanguageInDPOOL(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)   // only for Legacy
    DPOOL_txUNIT_tstLanguage stLanguage = { 0 };

    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    stLanguage.biClientUnit = (uint8)u16Param;
    DPOOL__enSetXData(DPOOL_nDataM_Language, &stLanguage, &DPOOL__xM_LanguageData);
#endif
}

static void TT__vSetDigitalVelocity(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessHMI) && (IDE_nProcess == SYSCONST_nProcessHMI)      // only for HMI

    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    APIMGR_xTraceCmdSetAPIValue(APIM_nFId_hmi_digitalgeschwindigkeit, 0, &u16Param, 4);
#endif
}

static void TT__vVolumeChange(uint16 u16FuncId, uint16 u16Param)
{
#if (defined SYSCONST_nProcessLegacy) && (IDE_nProcess == SYSCONST_nProcessLegacy)  // only for Legacy
    static BAPA_tstAUDIOCurrentVolume          AudioVol = { 0, 0, 0, 0, 0, BAPA_enFIDAvailable };
    static BAPA_tstAUDIOCurrentVolumeType      AudioVolType = { 0x0, BAPA_enFIDAvailable };   // Entertainment volume changed
    static bool                                volumeUp = true;

    TT_vTraceEvent(TT_ExecFunction, u16FuncId, u16Param);

    AudioVolType.u8ChangingVolumeType = 0x2;
    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolumeType, &AudioVolType, &DPOOL__xBAPA_mAUDIOCurrentVolumeTypeData);

    if (true == volumeUp)
    {
        AudioVol.u8EntertainmentVolume++;
        if (AudioVol.u8EntertainmentVolume >= 20)  volumeUp = false;
    }
    else
    {
        AudioVol.u8EntertainmentVolume--;
        if (AudioVol.u8EntertainmentVolume == 0)    volumeUp = true;
    }

    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolume, &AudioVol, &DPOOL__xBAPA_mAUDIOCurrentVolumeData);

    AudioVolType.u8ChangingVolumeType = 0x1;
    DPOOL__enSetXData(DPOOL_nDataBAPA_mAUDIOCurrentVolumeType, &AudioVolType, &DPOOL__xBAPA_mAUDIOCurrentVolumeTypeData);

    //BAPAPI_AUDIO__vSendSystemEvent(SYSTEM_VOLUME_TYPE_CHANGED);
    EVHD_vSendEvent(EV_APIM_SysEv_SYSTEM_VOLUME_TYPE_CHANGED);
#endif
}

/*** Exec_Function ********************************************************************************/
void TT__vExecFunction( uint16 u16FuncId, uint16 u16Param )
{
    switch ( (uint8)(u16FuncId & 0xFF))
    {
    case SHMEM_nSeqFunc_DebugFunction:
        TT__vDebugFunction( u16FuncId );     // temp. function to develop/test exec-functions
        break;
    case SHMEM_nSeqFunc_KL15On:
        TT__vExecKL15On( u16FuncId );
        break;
    case SHMEM_nSeqFunc_KL15Off:
        TT__vExecKL15Off( u16FuncId );
        break;
    case SHMEM_nSeqFunc_FillMessageQueue:
        TT__vFillMessageQueue( u16FuncId );
        break;
    case SHMEM_nSeqFunc_AddMessageQueueDelayItems:
        TT__vAddMessageQueueDelayItems( u16FuncId );
        break;
    case SHMEM_nSeqFunc_AddMessageQueueDiscardItems:
        TT__vAddMessageQueueDiscardItems( u16FuncId );
        break;
    case SHMEM_nSeqFunc_AddMessageQueuePassItems:
        TT__vAddMessageQueuePassItems( u16FuncId );
        break;
    case SHMEM_nSeqFunc_AddMessageQueueKeyItems1:
        TT__vAddMessageQueueKeyItems1( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_AddMessageQueueKeyItems2:
        TT__vAddMessageQueueKeyItems2( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_IncrementTageswegstrecke:
        TT__vIncrementTageswegstrecke( u16FuncId );
        break;
    case SHMEM_nSeqFunc_DisplayUpDown:
        TT__vDisplayUpDown( u16FuncId, u16Param );
        break;
        
    case SHMEM_nSeqFunc_HmiEnvironment:
        TT__vHmiEnvironment( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_OverVoltage:
        TT__vOverVoltage( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_Diagnostics:
        TT__vDiagnostics( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_SelectLanguage:
        TT__vSelectLanguage( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_SetLanguageInDPOOL:
        TT__vSetLanguageInDPOOL( u16FuncId, u16Param );
        break;
    case SHMEM_nSeqFunc_VolumeChange:
        TT__vVolumeChange( u16FuncId, u16Param );
        break;
    }
}
