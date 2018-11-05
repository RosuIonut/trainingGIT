#include <lib-util.hpp>
#include <lib-names.hpp>
#include "TraceConfig.hpp"

#include <string>
#include <sstream>
#include <Windows.h>

// Project include path necessary (Q:...)
#include "..\..\adapt\gen\brutus\wrs_externaleventdefinitions.h"
#include "..\..\adapt\gen\proseco\evhd_events.h"

#include "TT_ExecFunctions.hpp"
#include "SequencerSection.hpp"

namespace TraceConfig
{
   ///////////////////////////////////////////////////////////////////////////
   // Part I
   // Data structures for static configuration of trace items

   // Prototypes for filter factory functions
   bool boFilterDefault(const std::vector<std::string> &args, std::vector<SHMEM_tstTraceFilterEntry> *filter);
   bool boFilterMessage(const std::vector<std::string> &args, std::vector<SHMEM_tstTraceFilterEntry> *filter);
   // Prototypes for args translators
   void vTrMsgId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrArgsDec(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrWidget(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrExecFunc(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrDPool(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrSState(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrValue(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrEasType(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrKey(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrSendEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrKeyFilter(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrRun(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrRecEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrTdEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrSignalId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrApimDataId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrDepType(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   void vTrSmState(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
   // Prototypes for block translators
   void vTransTextString(ByteBuffer &oData, TraceSection::txLogEntry *xLogEntry);

// Abbreviations to make table more readable
#define nEvent       SHMEM_nTraceItemTypeEvent
#define nData        SHMEM_nTraceItemTypeData
#define nBlock       SHMEM_nTraceItemTypeBlock
#define nEvtEnabled  SHMEM_nTraceEventEnabled
#define nBlkEnabled  SHMEM_nTraceBlockEnabled
#define nNever       SHMEM_nTraceDataNotifyNever
#define nAlways      SHMEM_nTraceDataNotifyAlways
#define nOnChange    SHMEM_nTraceDataNotifyOnChange

const tstItemAttr astTraceItemTable[] =
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Item ID                                          | Item Name                         |  Type  | Mode      | Arg Cnt |Filter Factory | Args Trans  | Block Trans       
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  { SHMEM_nenTrace_NextStepLegacy,                    "NextStepLegacy",                    nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_NextStepHmi,                       "NextStepHmi",                       nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_KeyPress,                          "KeyPress",                          nEvent,  nEvtEnabled, 1,       NULL,           vTrKey,       NULL             },
  { SHMEM_nenTrace_KeyRelease,                        "KeyRelease",                        nEvent,  nEvtEnabled, 1,       NULL,           vTrKey,       NULL             },
  { SHMEM_nenTrace_APIM_vTriggerStateChange,          "APIM_vTriggerStateChange",          nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_vToggleAPIMStateChange,            "vToggleAPIMStateChange",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_boIsCriticalState,                 "boIsCriticalState",                 nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Error_WAS_Exc_MessageQueuePanic,   "Error_MessageQueuePanic",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Error_WAS_Exc_MessageQueueFull,    "Error_MessageQueueFull",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SetPanicMode,                      "SetPanicMode",                      nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_SetHMIKl15,                        "SetHMIKl15",                        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SwitchKl15,                        "SwitchKl15",                        nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ExecFunction,                      "ExecFunction",                      nEvent,  nEvtEnabled, 2,       NULL,           vTrExecFunc,  NULL             },
  { SHMEM_nenTrace_SendEvent,                         "SendEvent",                         nEvent,  nEvtEnabled, 1,       NULL,           vTrSendEvent, NULL             },
  { SHMEM_nenTrace_EPFMI_vMmiKeyCallback,             "EPFMI_vMmiKeyCallback",             nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_EPFMI_vMmiDoAction,                "EPFMI_vMmiDoAction",                nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_CUH_vRegisterAppForKeys,           "CUH_vRegisterAppForKeys",           nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_CUH_vDeRegisterAppForKeys,         "CUH_vDeRegisterAppForKeys",         nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_vWAS_KeyFilterDoEventProcessing,   "vWAS_KeyFilterDoEventProcessing",   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_vActivateFilter,                   "vActivateFilter",                   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_vGetLastMessages,                  "vGetLastMessages",                  nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Filter_vActivate,                  "Filter_vActivate",                  nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_KeyFilter_operator,                "KeyFilter_operator",                nEvent,  nEvtEnabled, 2,       NULL,           vTrKeyFilter, NULL             },
  { SHMEM_nenTrace_vRun_Msg,                          "vRun_Msg",                          nEvent,  nEvtEnabled, 2,       NULL,           vTrRun,       NULL             },
  { SHMEM_nenTrace_WPAStoryBoard_vSetActive,          "WPAStoryBoard_vSetActive",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPAAnimationStartedOutEvent,       "WPAAnimationStartedOutEvent",       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_StoryBoardDeactivated,             "StoryBoardDeactivated",             nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPAAnimationFinishedOutEvent,      "WPAAnimationFinishedOutEvent",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPAStoryBoard_vTimeTick,           "WPAStoryBoard_vTimeTick",           nEvent,  nEvtEnabled, 2,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPAAnimationStarted,               "WPAAnimationStarted",               nEvent,  nEvtEnabled, 1,       NULL,           vTrWidget,    NULL             },
  { SHMEM_nenTrace_WPAAnimationStopped,               "WPAAnimationStopped",               nEvent,  nEvtEnabled, 1,       NULL,           vTrWidget,    NULL             },
  { SHMEM_nenTrace_WPAAnimationDestroyNotifcation,    "WPAAnimationDestroyNotifcation",    nEvent,  nEvtEnabled, 1,       NULL,           vTrWidget,    NULL             },
  { SHMEM_nenTrace_SelfWidgetBase_vOnAnimActive,      "SelfWidgetBase_vOnAnimActive",      nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WPASBSelf_enProcessMessage,        "WPASBSelf_enProcessMessage",        nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },
  { SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsInvert, "WPASBSelf_vUpdateAnimParamsInvert", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPASBSelf_vTimeTick,               "WPASBSelf_vTimeTick",               nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WPASBSelf_vSetActive,              "WPASBSelf_vSetActive",              nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPASBSelf_vStoreStaticValue,       "WPASBSelf_vStoreStaticValue",       nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WPAESSelf_vStoreStaticValue,       "WPAESSelf_vStoreStaticValue",       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPASBSelf_vRestoreModelValue,      "WPASBSelf_vRestoreModelValue",      nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WPAESSelf_vRestoreModelValue,      "WPAESSelf_vRestoreModelValue",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPASBSelf_vRequestUpdateAnimParams,"WPASBSelf_vRequestUpdateAnimParams",nEvent,  nEvtEnabled, 2,       NULL,           vTrEasType,   NULL             },
  { SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsSelf,   "WPASBSelf_vUpdateAnimParamsSelf",   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsSelfRuntime,"WPASBSelf_vUpdateAnimParamsSelfRuntime",  
                                                                                           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_UpdateEasingForAllScenes,          "UpdateEasingForAllScenes",          nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_UpdateDuration,                    "UpdateDuration",                    nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_UpdateAnimation,                   "UpdateAnimation",                   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_m_vOnLongPressInit,                "m_vOnLongPressInit",                nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_LongPressStarted,                  "LongPressStarted",                  nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_m_vOnLongPressRetrigger,           "m_vOnLongPressRetrigger",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_m_vOnLongPressReleased,            "m_vOnLongPressReleased",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Anima_vPropagateAnimData,          "Anima_vPropagateAnimData",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Anima_vSetActiveAnimData,          "Anima_vSetActiveAnimData",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Anima_vSetWaitingAnimData,         "Anima_vSetWaitingAnimData",         nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Anima_vSetTimeTickDirection,       "Anima_vSetTimeTickDirection",       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_Anima_vOnPreLayoutingHook,         "Anima_vOnPreLayoutingHook",         nEvent,  nEvtEnabled, 1,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vControllerSync,               "SMG_vControllerSync",               nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vInitActive15,                 "SMG_vInitActive15",                 nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vDeinitActive15,               "SMG_vDeinitActive15",               nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vInitHoldDisplay,              "SMG_vInitHoldDisplay",              nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vDeinitHoldDisplay,            "SMG_vDeinitHoldDisplay",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SMG_vDoEventProcessing,            "SMG_vDoEventProcessing",            nEvent,  nEvtEnabled, 1,       NULL,           vTrRecEvent,  NULL             },
  { SHMEM_nenTrace_ILU_vInitActiveDisplay,            "ILU_vInitActiveDisplay",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ILU_vDeinitActiveDisplay,          "ILU_vDeinitActiveDisplay",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ILU_vCPrePostRunDeinit15,          "ILU_vCPrePostRunDeinit15",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ILU_vCPrePostRunDoEventProcessing, "ILU_vCPrePostRunDoEventProcessing", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SetReq_HoldDisplayByIluDisplayUsed,"SetReq_HoldDisplayByIluDisplayUsed",nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
{ SHMEM_nenTrace_ClearReq_HoldDisplayByIluDisplayUsed,"ClearReq_HoldDisplayByIluDisplayUsed",nEvent,nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SetReq_Active15BySMG,              "SetReq_Active15BySMG",              nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ClearReq_Active15BySMG,            "ClearReq_Active15BySMG",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SetReq_HoldDisplayBySMG,           "SetReq_HoldDisplayBySMG",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ClearReq_HoldDisplayBySMG,         "ClearReq_HoldDisplayBySMG",         nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_SetReq_ActiveDisplayBySMG,         "SetReq_ActiveDisplayBySMG",         nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_ClearReq_ActiveDisplayBySMG,       "ClearReq_ActiveDisplayBySMG",       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_TIMER_vStart_nIluPrePostRunTimer,  "TIMER_vStart_nIluPrePostRunTimer",  nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_TIMER_vStart_nActiveDisplayTimeout,"TIMER_vStart_nActiveDisplayTimeout",nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_srStateHMI_Inactive__ENV_InitEvent,"srStateHMI_Inactive__ENV_InitEvent",nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_srStateHMI_Reset__FW_InitEvent,    "srStateHMI_Reset__FW_InitEvent",    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_srStateHMI_Reset__ENV_DeinitEvent, "srStateHMI_Reset__ENV_DeinitEvent", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_srStateHMI_Active__FW_DeinitEvent, "srStateHMI_Active__FW_DeinitEvent", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vReset,                        "HMI_vReset",                        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vWakeup,                       "HMI_vWakeup",                       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vSleep,                        "HMI_vSleep",                        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vDisplayOn,                    "HMI_vDisplayOn",                    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vDisplayOff,                   "HMI_vDisplayOff",                   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vSyncInit,                     "HMI_vSyncInit",                     nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_vAsyncDeInit,                  "HMI_vAsyncDeInit",                  nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_HMI_boSetDependency,               "HMI_boSetDependency",               nEvent,  nEvtEnabled, 2,       NULL,           vTrDepType,   NULL             },
  { SHMEM_nenTrace_APIM_vInitialization,              "APIM_vInitialization",              nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_APIM_vDeinitialization,            "APIM_vDeinitialization",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_APIM_Reset,                        "APIM_Reset",                        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_VCAnimationCreated,                "VCAnimationCreated",                nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_VCAnimationStarted,                "VCAnimationStarted",                nEvent, nEvtEnabled,  0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_VCAnimationStopped,                "VCAnimationStopped",                nEvent, nEvtEnabled,  0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_APIM_boSetData,                    "APIM_boSetData",                    nEvent,  nEvtEnabled, 1,       NULL,           vTrApimDataId,NULL             },
  { SHMEM_nenTrace_APIM_boGetData__APIM_nDId_ai32DP_SINT,"APIM_boGetData__APIM_nDId_ai32DP_SINT",nEvent,nEvtEnabled, 0,     NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_APIM_boGetData__APIM_nDId_aboDP_BOOL, "APIM_boGetData__APIM_nDId_aboDP_BOOL", nEvent,nEvtEnabled, 0,     NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WMMS_u8Init,                       "WMMS_u8Init",                       nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WMMS_u8DeInit,                     "WMMS_u8DeInit",                     nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_boSendSignalWithoutData,           "boSendSignalWithoutData",           nEvent,  nEvtEnabled, 1,       NULL,           vTrSignalId,  NULL             },
  { SHMEM_nenTrace_transit_CSHStateHMI_Reset,         "transit_CSHStateHMI_Reset",         nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_transit_CSHStateHMI_Active,        "transit_CSHStateHMI_Active",        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_transit_CSHStateHMI_Inactive,      "transit_CSHStateHMI_Inactive",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_API_Pool__vInitResources,          "API_Pool__vInitResources",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_API_Pool__vDeInitResources,        "API_Pool__vDeInitResources",        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_API_Timer_vInitResource,           "API_Timer_vInitResource",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_API_Timer_vDeInitResource,         "API_Timer_vDeInitResource",         nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WRM_u8Request,                     "WRM_u8Request",                     nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WRM__vCASendWarningNotification,   "WRM__vCASendWarningNotification",   nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_WRA_ActivateZWL,                   "WRA_ActivateZWL",                   nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
{ SHMEM_nenTrace_boSignalSI__InitRES_HMI_Framwork_ANY,"boSignalSI__InitRES_HMI_Framwork_ANY",nEvent,nEvtEnabled, 0,       NULL,           NULL,         NULL             },
{ SHMEM_nenTrace_boSignalSI__DeinitRES_HMI_Framwork_ANY,"boSignalSI__DeinitRES_HMI_Framwork_ANY",
                                                                                           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_CRHDL_enStateHdlGetState0,          "CRHDL_enStateHdlGetState0",        nEvent,  nEvtEnabled, 1,       NULL,           vTrSmState,   NULL             }, 
  { SHMEM_nenTrace_CRHDL_enStateHdlGetState1,          "CRHDL_enStateHdlGetState1",        nEvent,  nEvtEnabled, 1,       NULL,           vTrSmState,   NULL             },
{ SHMEM_nenTrace_boSignalSI_EV_DiagAdaption_0_Change, "boSignalSI_EV_DiagAdaption_0_Change",nEvent, nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_boSignalSI_EV_DiagCodingChange,    "boSignalSI_EV_DiagCodingChange",    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_boSignalSI_EV_DPOOL_OnChange_M_Language,"boSignalSI_EV_DPOOL_OnChange_M_Language",
                                                                                           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_GetDPOOL_M_Language,               "GetDPOOL_M_Language",               nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_MFARoleContainer_enProcessMessage, "MFARoleContainer_enProcessMessage", nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             }, 
  { SHMEM_nenTrace_MFARoleContainer_vProcessKey,      "MFARoleContainer_vProcessKey",      nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_m_vOnKeyPressed,    "SelfWidgetBase_m_vOnKeyPressed",    nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfSSMCoreCommon_m_vOnKeyPressed, "SelfSSMCoreCommon_m_vOnKeyPressed", nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfWidgetMFA_boHandleKey,         "SelfWidgetMFA_boHandleKey",         nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfWidgetList_boHandleKey,        "SelfWidgetList_boHandleKey",        nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_Framework_TriggerForcedPaint,      "Framework_TriggerForcedPaint",      nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfSSMCoreCommon_vUpdateMC,       "SelfSSMCoreCommon_vUpdateMC",       nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfWidgetWPABase_vUpdateAnimation,"SelfWidgetWPABase_vUpdateAnimation",nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_vUpdateStateSSMCore,"SelfWidgetBase_vUpdateStateSSMCore",nEvent,  nEvtEnabled, 1,       NULL,           vTrSState,    NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_stSetAnimData,      "SelfWidgetBase_stSetAnimData",      nEvent,  nEvtEnabled, 1,       NULL,           vTrSState,    NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_stGetAnimData,      "SelfWidgetBase_stGetAnimData",      nEvent,  nEvtEnabled, 1,       NULL,           vTrSState,    NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_vOnElapsed,         "SelfWidgetBase_vOnElapsed",         nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_SelfSSMCoreCommon_vOnElapsed,      "SelfSSMCoreCommon_vOnElapsed",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_SelfSSMMoveCounter_vOnElapsed,     "SelfSSMMoveCounter_vOnElapsed",     nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_vOnMFAIterationAnimationDone,      "vOnMFAIterationAnimationDone",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_SelfWidgetBase_boHandleKeySSMCore, "SelfWidgetBase_boHandleKeySSMCore", nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_vPropagateAnimDataActiveAnim,      "vPropagateAnimDataActiveAnim",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
  { SHMEM_nenTrace_vPropagateAnimDataWaitingAnim,     "vPropagateAnimDataWaitingAnim",     nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             }, 
{ SHMEM_nenTrace_vSetUpdateFreezeModeInLastBeforeDraw,"vSetUpdateFreezeModeInLastBeforeDraw",
                                                                                           nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vOnLPInit,                       "m_vOnLPInit",                       nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vActivateLPMode,                 "m_vActivateLPMode",                 nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vOnLPRetrigger,                  "m_vOnLPRetrigger",                  nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vOnLPRelease,                    "m_vOnLPRelease",                    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_m_vOnViewChange,                   "m_vOnViewChange",                   nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vFlush,                          "m_vFlush",                          nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_ControlAC_enProcessMessage,        "ControlAC_enProcessMessage",        nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_Messenger_vProcessMessage,         "Messenger_vProcessMessage",         nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             }, 
  { SHMEM_nenTrace_SelfSSMCoreCommon_vProcessElapsed, "SelfSSMCoreCommon_vProcessElapsed", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
{ SHMEM_nenTrace_ListAnimationVwFPK_enProcessMessage, "ListAnimationVwFPK_enProcessMessage",nEvent, nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },       
{ SHMEM_nenTrace_SelfSSMCoreCommon_m_vRunStateMachine,"SelfSSMCoreCommon_m_vRunStateMachine",nEvent,nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_KeyHandlerMaster_vInit,            "KeyHandlerMaster_vInit",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_KeyHandlerMaster_vDeinit,          "KeyHandlerMaster_vDeinit",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_HMI_boSendMessage,                 "HMI_boSendMessage",                 nEvent,  nEvtEnabled, 1,       NULL,           vTrMsgId,     NULL             }, 
  { SHMEM_nenTrace_WIDVWFPK_vInitGlobalData,          "WIDVWFPK_vInitGlobalData",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_UTILS_vInitGlobalData,             "UTILS_vInitGlobalData",             nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_PAINTER_vInitGlobalData,           "PAINTER_vInitGlobalData",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },       
  { SHMEM_nenTrace_CarouselContainer_enProcessMessage,"CarouselContainer_enProcessMessage",nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },   
  { SHMEM_nenTrace_SelfMainMenuRoll_boHandleKey,      "SelfMainMenuRoll_boHandleKey",      nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_m_vSetFreezeMode,                  "m_vSetFreezeMode",                  nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_KeyHandlerMaster_vUnfreeze,        "KeyHandlerMaster_vUnfreeze",        nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },    
  { SHMEM_nenTrace_vUpdateWaitingAnimation,           "vUpdateWaitingAnimation",           nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_vOnMsgAddedToKeyQueue,             "vOnMsgAddedToKeyQueue",             nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_u8AddNewKeys,                      "u8AddNewKeys",                      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_m_boNeutralizeNewKeysVirtualKeys,  "m_boNeutralizeNewKeysVirtualKeys",  nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_boInvert,                          "boInvert",                          nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },    
  { SHMEM_nenTrace_vOnLastBeforeDraw,                 "vOnLastBeforeDraw",                 nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_SelfMainMenuRoll_tCreateAnimation, "SelfMainMenuRoll_tCreateAnimation", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_KeyHandlerMaster_vFreeze,          "KeyHandlerMaster_vFreeze",          nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },    
  { SHMEM_nenTrace_vActivateStaticModeViewSkipping,   "vActivateStaticModeViewSkipping",   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_m_vRetryKeyProcessing,             "m_vRetryKeyProcessing",             nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_m_vOnViewChangeFlushKeyQueue,      "m_vOnViewChangeFlushKeyQueue",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_vFlushToSystemMsgQueue,            "vFlushToSystemMsgQueue",            nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_KeyHandlerMaster_vDeactivate,      "KeyHandlerMaster_vDeactivate",      nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_vDiscardAllMessages,               "vDiscardAllMessages",               nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_m_vRemoveAllVirtualKeysFromSSM,    "m_vRemoveAllVirtualKeysFromSSM",    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_m_vOnViewChangeDiscardKeyQueue,    "m_vOnViewChangeDiscardKeyQueue",    nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_boGetCfgDeactivateKHAtUnfreeze,    "boGetCfgDeactivateKHAtUnfreeze",    nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },    
  { SHMEM_nenTrace_KeyHandlerMaster_vActivate,        "KeyHandlerMaster_vActivate",        nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },    
  { SHMEM_nenTrace_KeyHandlerMaster_boAddMessage,     "KeyHandlerMaster_boAddMessage",     nEvent,  nEvtEnabled, 1,       NULL,           vTrMsgId,     NULL             },    
  { SHMEM_nenTrace_WarningState_enProcessMessage,     "WarningState_enProcessMessage",     nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },   
  { SHMEM_nenTrace_SelfWidgetWarningState_boHandleKey,"SelfWidgetWarningState_boHandleKey",nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_ScrollBar_enProcessMessage,        "ScrollBar_enProcessMessage",        nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },   
  { SHMEM_nenTrace_SelfOwnerPopupInvert_enProcessMessage,"SelfOwnerPopupInvert_enProcessMessage",nEvent, nEvtEnabled, 2,  NULL,           vTrWidget,    NULL             },   
  { SHMEM_nenTrace_SelfPopupInvert_boHandleKey,       "SelfPopupInvert_boHandleKey",       nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_boDisableInversion,                "boDisableInversion",                nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_CarouselContainer_start_timer,     "CarouselContainer_start_timer",     nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },    
  { SHMEM_nenTrace_LayoutController_enProcessMessage, "LayoutController_enProcessMessage", nEvent,  nEvtEnabled, 2,       NULL,           vTrWidget,    NULL             },   
  { SHMEM_nenTrace_SelfLayout_boHandleKey,            "SelfLayout_boHandleKey",            nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             }, 
  { SHMEM_nenTrace_WDOG_vEnableEntity,                "WDOG_vEnableEntity",                nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WDOG_vDisableEntity,               "WDOG_vDisableEntity",               nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_WDOG_vUpdateEntitiesList,          "WDOG_vUpdateEntitiesList",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_DispatchEvent,                     "DispatchEvent",                     nEvent,  nEvtEnabled, 1,       NULL,           vTrTdEvent,   NULL             },
  { SHMEM_nenTrace_Display_vOnLastBeforeDeactivation, "Display_vOnLastBeforeDeactivation", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_AVICORE_vTaskDeactivationCallback, "AVICORE_vTaskDeactivationCallback", nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_GRLC_vDisconnectSurfaceID,         "GRLC_vDisconnectSurfaceID",         nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vSurfaceDelete,               "GRLC_vSurfaceDelete",               nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vLayerDelete,                 "GRLC_vLayerDelete",                 nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_Display_vOnTaskActivated,          "Display_vOnTaskActivated",          nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_GRLC_vVideoOutSetLayerUpdateMode,  "GRLC_vVideoOutSetLayerUpdateMode",  nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_AVICORE_vTaskActivationCallback,   "AVICORE_vTaskActivationCallback",   nEvent,  nEvtEnabled, 0,       NULL,           NULL,         NULL             },
  { SHMEM_nenTrace_GRLC_u16LayerCreate,               "GRLC_u16LayerCreate",               nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_u16SurfaceCreate,             "GRLC_u16SurfaceCreate",             nEvent,  nEvtEnabled, 1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vConnectSurfaceID,            "GRLC_vConnectSurfaceID",            nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vLayerSetVisibility,          "GRLC_vLayerSetVisibility",          nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vLayerSetPerPixelTransparency,"GRLC_vLayerSetPerPixelTransparency",nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_EEH_u8GetPixelOffset,              "EEH_u8GetPixelOffset",              nEvent,  nEvtEnabled, 2,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_EEH_i8Getp_Verschiebung_Displayinhalte,"EEH_i8Getp_Verschiebung_Displayinhalte",nEvent,nEvtEnabled, 2, NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_GRLC_vDisplaySetOffset,            "GRLC_vDisplaySetOffset",            nEvent,  nEvtEnabled, 3,       NULL,           vTrArgsDec,   NULL             },

  // Data
  { SHMEM_nenTrace_HMIQueueLoad,                      "HMIQueueLoad",                      nData,   nOnChange,   1,       NULL,           vTrArgsDec,   NULL             },
  { SHMEM_nenTrace_SetDPOOL,                          "SetDPOOL",                          nData,   nOnChange,   2,       NULL,           vTrDPool,     NULL             },
  { SHMEM_nenTrace_GetDPOOL,                          "GetDPOOL",                          nData,   nOnChange,   2,       NULL,           vTrDPool,     NULL             },
  { SHMEM_nenTrace_SelfState_OLD,                     "SelfState_OLD",                     nData,   nOnChange,   1,       NULL,           vTrSState,    NULL             },
  { SHMEM_nenTrace_SelfState,                         "SelfState",                         nData,   nOnChange,   2,       NULL,           vTrSState,    NULL             },
  { SHMEM_nenTrace_MoveCounter,                       "MoveCounter",                       nData,   nOnChange,   2,       NULL,           vTrValue,     NULL             },
  { SHMEM_nenTrace_HmiFrameId,                        "HmiFrameId",                        nData,   nOnChange,   1,       NULL,           vTrArgsDec,   NULL             },    
  
  // Block
  { SHMEM_nenTrace_TextString,                        "ErrorDetail",                       nBlock,  nBlkEnabled, 0,       NULL,           NULL,         vTransTextString },
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

   const tstEventDef astHmiEventTable[] =
   {
       {"HK_BACK_LONG",                 HMI::APIM_SysEv_HK_BACK_LONG},
       {"HK_BACK_LONG_RELEASE",         HMI::APIM_SysEv_HK_BACK_LONG_RELEASE},
       {"HK_BACK_SHORT",                HMI::APIM_SysEv_HK_BACK_SHORT},
       {"HK_BC_DOWN_SHORT_PRESS",       HMI::APIM_SysEv_HK_BC_DOWN_SHORT_PRESS},
       {"HK_BC_RESET_LONG_PRESS",       HMI::APIM_SysEv_HK_BC_RESET_LONG_PRESS},
       {"HK_BC_RESET_SHORT_RELEASE",    HMI::APIM_SysEv_HK_BC_RESET_SHORT_RELEASE},
       {"HK_BC_UP_SHORT_PRESS",         HMI::APIM_SysEv_HK_BC_UP_SHORT_PRESS},
       {"HK_DOWN_LONG",                 HMI::APIM_SysEv_HK_DOWN_LONG},
       {"HK_DOWN_LONG_FREQUENCY",       HMI::APIM_SysEv_HK_DOWN_LONG_FREQUENCY},
       {"HK_DOWN_LONG_RELEASE",         HMI::APIM_SysEv_HK_DOWN_LONG_RELEASE},
       {"HK_DOWN_SHORT",                HMI::APIM_SysEv_HK_DOWN_SHORT},
       {"HK_DOWN_SHORT_RELEASE",        HMI::APIM_SysEv_HK_DOWN_SHORT_RELEASE},
       {"HK_HOOK_LONG",                 HMI::APIM_SysEv_HK_HOOK_LONG},
       {"HK_HOOK_LONG_RELEASE",         HMI::APIM_SysEv_HK_HOOK_LONG_RELEASE},
       {"HK_HOOK_SHORT",                HMI::APIM_SysEv_HK_HOOK_SHORT},
       {"HK_HOOK_SHORT_RELEASE",        HMI::APIM_SysEv_HK_HOOK_SHORT_RELEASE},
       {"HK_KEY_LEFT_LONG_PRESS",       HMI::APIM_SysEv_HK_KEY_LEFT_LONG_PRESS},
       {"HK_KEY_LEFT_SHORT_PRESS",      HMI::APIM_SysEv_HK_KEY_LEFT_SHORT_PRESS},
       {"HK_KEY_MENU_LONG_PRESS",       HMI::APIM_SysEv_HK_KEY_MENU_LONG_PRESS},
       {"HK_KEY_MENU_SHORT_PRESS",      HMI::APIM_SysEv_HK_KEY_MENU_SHORT_PRESS},
       {"HK_KEY_OK_LONG_PRESS",         HMI::APIM_SysEv_HK_KEY_OK_LONG_PRESS},
       {"HK_KEY_OK_SHORT_PRESS",        HMI::APIM_SysEv_HK_KEY_OK_SHORT_PRESS},
       {"HK_KEY_RIGHT_LONG_PRESS",      HMI::APIM_SysEv_HK_KEY_RIGHT_LONG_PRESS},
       {"HK_KEY_RIGHT_SHORT_PRESS",     HMI::APIM_SysEv_HK_KEY_RIGHT_SHORT_PRESS},
       {"HK_KEY_WHEEL_TICKS",           HMI::APIM_SysEv_HK_KEY_WHEEL_TICKS},
       {"HK_KOMBI_0_LONG_PRESS",        HMI::APIM_SysEv_HK_KOMBI_0_LONG_PRESS},
       {"HK_KOMBI_0_LONG_PRESS_MKB",    HMI::APIM_SysEv_HK_KOMBI_0_LONG_PRESS_MKB},
       {"HK_KOMBI_0_PRESSED_5X",        HMI::APIM_SysEv_HK_KOMBI_0_PRESSED_5X},
       {"HK_KOMBI_0_SHORT_PRESS",       HMI::APIM_SysEv_HK_KOMBI_0_SHORT_PRESS},
       {"HK_MENU_LEFT_LONG",            HMI::APIM_SysEv_HK_MENU_LEFT_LONG},
       {"HK_MENU_LEFT_LONG_RELEASE",    HMI::APIM_SysEv_HK_MENU_LEFT_LONG_RELEASE},
       {"HK_MENU_LEFT_SHORT",           HMI::APIM_SysEv_HK_MENU_LEFT_SHORT},
       {"HK_MENU_LEFT_SHORT_RELEASE",   HMI::APIM_SysEv_HK_MENU_LEFT_SHORT_RELEASE},
       {"HK_MENU_LONG",                 HMI::APIM_SysEv_HK_MENU_LONG},
       {"HK_MENU_LONG_RELEASE",         HMI::APIM_SysEv_HK_MENU_LONG_RELEASE},
       {"HK_MENU_RIGHT_LONG",           HMI::APIM_SysEv_HK_MENU_RIGHT_LONG},
       {"HK_MENU_RIGHT_LONG_RELEASE",   HMI::APIM_SysEv_HK_MENU_RIGHT_LONG_RELEASE},
       {"HK_MENU_RIGHT_SHORT",          HMI::APIM_SysEv_HK_MENU_RIGHT_SHORT},
       {"HK_MENU_RIGHT_SHORT_RELEASE",  HMI::APIM_SysEv_HK_MENU_RIGHT_SHORT_RELEASE},
       {"HK_MENU_SHORT",                HMI::APIM_SysEv_HK_MENU_SHORT},
       {"HK_MENU_SHORT_RELEASE",        HMI::APIM_SysEv_HK_MENU_SHORT_RELEASE},
       {"HK_MFL_JOKER_SHORT_PRESS",     HMI::APIM_SysEv_HK_MFL_JOKER_SHORT_PRESS},
       {"HK_OK_LONG",                   HMI::APIM_SysEv_HK_OK_LONG},
       {"HK_OK_LONG_RELEASE",           HMI::APIM_SysEv_HK_OK_LONG_RELEASE},
       {"HK_OK_SHORT",                  HMI::APIM_SysEv_HK_OK_SHORT},
       {"HK_OK_SHORT_RELEASE",          HMI::APIM_SysEv_HK_OK_SHORT_RELEASE},
       {"HK_UP_LONG",                   HMI::APIM_SysEv_HK_UP_LONG},
       {"HK_UP_LONG_FREQUENCY",         HMI::APIM_SysEv_HK_UP_LONG_FREQUENCY},
       {"HK_UP_LONG_RELEASE",           HMI::APIM_SysEv_HK_UP_LONG_RELEASE},
       {"HK_UP_SHORT",                  HMI::APIM_SysEv_HK_UP_SHORT},
       {"HK_UP_SHORT_RELEASE",          HMI::APIM_SysEv_HK_UP_SHORT_RELEASE},
       {"LAPTIMER_HOUR_MODE_REACHED",   HMI::APIM_SysEv_LAPTIMER_HOUR_MODE_REACHED},

   };
        
   const tstEventDef astSysEventTable[] =
   {
       {"KL_15_ACTIVATED",              EV_APIM_SysEv_KL_15_ACTIVATED},
       {"KL_15_DEACTIVATED",            EV_APIM_SysEv_KL_15_DEACTIVATED},
   };
        

    // wti: added names in DPOOL which are monitored by TestTool
    const char *stTraceDPoolNameTable[] =
    {
        "HMI_stExtQFillLevel",
        "SMG_T15_Status",
        "EPFMI_boModelKeyDisplaced",
        "hmi_skin_download_byGraBoot",
        "M_Language",
    };

    const char *stKeyNameTable[] =
    {
        "0?",               //00
        "MFL_MENU",         //01 //old:0x06
        "MFL_NEXT",         //02 //old:0x01
        "MFL_PREV",         //03 //old:0x02
        "MFL_UP",           //04 //old:0x03
        "MFL_DOWN",         //05 //old:0x04
        "6?",               //06
        "MFL_OK",           //07 //old:0x05
        "8?",               //08
        "9?",               //09
        "10?",              //10
        "11?",              //11
        "MFL_FAS",          //12 //old:0x07
    };
    
    const char *stEasingTypeNameTable[] =
    {
        "Constant",         // EasingTypeConstant = 0,
        "Linear",           // EasingTypeLinear, //1
        "Quadratic",        // EasingTypeQuadratic, //2
        "Cubic",            // EasingTypeCubic, //3
        "Quartic",          // EasingTypeQuartic, //4
        "Quintic",          // EasingTypeQuintic, //5
        "Exponential",      // EasingTypeExponential, //6
        "Back",             // EasingTypeBack, //7
        "Bounce",           // EasingTypeBounce, //8
        "Sinusoidal",       // EasingTypeSinusoidal, //9
        "Elastic",          // EasingTypeElastic, //10
        "Circular",         // EasingTypeCircular, //11
        "Invalid"           // EasingTypeInvalid, //12
    };
   
   const tstItemAttr *pstGetItemAttr(const std::string &sItemName)
   {
      for (int i = 0; i < ARRAY_SIZE(astTraceItemTable); i++)
      {
         if (util::sMakeLower(sItemName).compare(util::sMakeLower(astTraceItemTable[i].sItemName)) == 0)
            return &astTraceItemTable[i];
      }

      return NULL;
   }

   const tstItemAttr *pstGetItemAttr(uint16 u16ItemId)
   {
      for (int i = 0; i < ARRAY_SIZE(astTraceItemTable); i++)
      {
         if (astTraceItemTable[i].u16ItemId == u16ItemId)
            return &astTraceItemTable[i];
      }

      return NULL;
   }


   ///////////////////////////////////////////////////////////////////////////
   // Part II
   // Filter factories for item configuration

   // The default filter checks the arguments for numeric values
   bool boFilterDefault(const std::vector<std::string> &args, std::vector<SHMEM_tstTraceFilterEntry> *filter)
   {
      bool boFailed = false;

      // Iterate over all arguments
      for (int i = 1; !boFailed && i < (int) args.size(); i++)
      {
         SHMEM_tstTraceFilterEntry stEntry;
         stEntry.u8ArgIndex = i - 1;

         char *pcEnd;
         stEntry.u32ArgValue = strtoul(args[i].c_str(), &pcEnd, 0);
         if (*pcEnd != '\0')
         {
            // Not numeric data
            util::Log::vPrint(util::LOG_ERROR, "Could not translate \"%s\" into a numeric value!", args[i].c_str());
            boFailed = true;
         }

         stEntry.u8Flags = 0;
         filter->push_back(stEntry);
      }

      if (!boFailed)
      {
         // Mark last entry as end of product
         filter->back().u8Flags |= SHMEM_nTraceFlagEndProduct;
      }

      return !boFailed;
   }

   // Create filter for a single argument
   bool boFilterArg(const std::string &sArgName, const std::string sEnum, std::vector<SHMEM_tstTraceFilterEntry> *filter, int iArgIndex, bool boAllowNumeric)
   {
      bool boFailed = false;
      
      SHMEM_tstTraceFilterEntry stEntry;
      stEntry.u8ArgIndex = iArgIndex;
      stEntry.u8Flags = SHMEM_nTraceFlagEndProduct;

      int iArgValue;
      if (names::boFindEnumValue(sEnum, sArgName, &iArgValue))
      {
         stEntry.u32ArgValue = iArgValue;
         filter->push_back(stEntry);
      }
      else
      {
         if (boAllowNumeric)
         {
            char *pcEnd;
            stEntry.u32ArgValue = strtoul(sArgName.c_str(), &pcEnd, 0);
            if (*pcEnd == '\0')
            {
               // Numeric data
               filter->push_back(stEntry);
            }
            else
            {
               // Not numeric data
               util::Log::vPrint(util::LOG_ERROR, "Could not translate \"%s\" into a numeric value!", sArgName.c_str());
               boFailed = true;
            }
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Could not find \"%s\" in enumeration!", sArgName.c_str());
            boFailed = true;
         }
      }

      return !boFailed;
   }

   // The message filter checks if the Message ID and the Message Data both match (AND)
   bool boFilterMessage(const std::vector<std::string> &args, std::vector<SHMEM_tstTraceFilterEntry> *filter)
   {
      bool boFailed = false;

      if (args.size() > 1)
      {
         // 1st argument is the Message ID
         if (!boFilterArg(args[1], "MessageIds", filter, 0, false))
            boFailed = true;

         if (!boFailed && args.size() > 2)
         {
            // 2nd argument depends on message type
            if (util::sMakeLower(args[1]).compare("keymessage") == 0)
            {
               // For KeyMessage, the argument is expected to be a key name
               if (!boFilterArg(args[2], "Keys", filter, 1, false))
                  boFailed = true;
            }
            else
            {
               // For everything else a numeric value is expected
               if (!boFilterArg(args[2], "", filter, 1, true))
                  boFailed = true;
            }
         }
      }

      return !boFailed;
   }


   ///////////////////////////////////////////////////////////////////////////
   // Part III
   // Translation functions for binary trace data with uint32 arguments

   // Default argument translator just prints out all arguments as hexadecimal numbers
   void vDefTransArgs(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stLogParam = {"Arg"};
         std::stringstream ss;
         ss << "0x";
         ss.width(8);
         ss.fill('0');
         ss << std::hex << *pu32Args;
         stLogParam.sValue = ss.str();
         xLogEntry->push_back(stLogParam);

         // Recurse for following arguments
         vDefTransArgs(pu32Args + 1, u8ArgCount - 1, xLogEntry);
      }
      else
      {
         // End of recursion
      }
   }

   // Helper function to print an argument that is expected to match an enumeration.
   // If the value is not found, it is printed as number
   void vTranslateArg(const std::string sArgName, uint32 u32ArgValue, const std::string sEnum, TraceSection::txLogEntry *xLogEntry)
   {
      TraceSection::tstLogEntryParam stParam;
      bool boNameFound = false;

      stParam.sName = sArgName;

      // Is there a enum to search in?
      if (!sEnum.empty())
      {
         std::string sArgString;
         if (names::boFindEnumName(sEnum, u32ArgValue, &sArgString))
         {
            // Element found
            stParam.sValue = sArgString;
            boNameFound = true;
         }
        #if 0
         else
         {
            // Element not found
            util::Log::vPrint(util::LOG_WARNING, "Value %i not found in enum %s.", u32ArgValue, sEnum.c_str());
         }
        #endif
      }

      // If either no enum to search in, or nothing found: use numeric data
      if (!boNameFound)
      {
         std::stringstream ss;
         ss << u32ArgValue;
         stParam.sValue = ss.str();
      }

      xLogEntry->push_back(stParam);
   }   

   // Default argument translator just prints out all arguments as hexadecimal numbers
   void vTrArgsDec(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stLogParam = {"Arg"};
         std::stringstream ss;
         ss << (int)(*pu32Args);
         stLogParam.sValue = ss.str();
         xLogEntry->push_back(stLogParam);

         // Recurse for following arguments
         vTrArgsDec(pu32Args + 1, u8ArgCount - 1, xLogEntry);
      }
      else
      {
         // End of recursion
      }
   }

   // WidgetId argument translator
   void vTrWidget(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("Widget", pu32Args[0], "WidgetIds", xLogEntry);
         
         if (u8ArgCount > 1)
         {
           vTranslateArg("Msg", pu32Args[1], "MessageIds", xLogEntry);
         }
      }
   }

   // RecEvent argument translator
   void vTrRecEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("Event", pu32Args[0], "RecEvents", xLogEntry);         
      }
   }

   // HMI time domain event argument translator
   void vTrTdEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("Event", pu32Args[0], "TimeDomainEvents", xLogEntry);         
      }
   }

   // SignalIds argument translator
   void vTrSignalId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("Id", pu32Args[0], "SignalIds", xLogEntry);         
      }
   }

   // ApimDataId argument translator
   void vTrApimDataId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("DataId", pu32Args[0], "ApimDataId", xLogEntry);         
      }
   }

   // DependencyType argument translator
   void vTrDepType(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("DepType", pu32Args[0], "DependentType", xLogEntry);         
         if (u8ArgCount > 1)
         {
            TraceSection::tstLogEntryParam stLogParam = {"DepCode"};
            std::stringstream ss;
            ss << pu32Args[1];
            stLogParam.sValue = ss.str();
            xLogEntry->push_back(stLogParam);
         }
      }
   }

   // SM State argument translator
   void vTrSmState(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("State", pu32Args[0], "CRDHL_SMState", xLogEntry);         
         if (u8ArgCount > 1)
         {
            TraceSection::tstLogEntryParam stLogParam = {"State"};
            std::stringstream ss;
            ss << pu32Args[1];
            stLogParam.sValue = ss.str();
            xLogEntry->push_back(stLogParam);
         }
      }
   }

   // Key message translators looks up the message data.
   void vTransKeyMessage(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         // Key messages use only the lower 16 bits of message data for the actual key code.
         // The upper16 bits are the "u16MflTimeStamp" which is uninteresting for us.
         vTranslateArg("Key", pu32Args[0] & 0x0000FFFF, "Keys", xLogEntry);
      }
   }

   // Message translator looks up the message ID. If there is a translation function for the
   // message data available, it is called. Otherwise the message data is translated using the
   // default translator.
   void vTrMsgId(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTranslateArg("Msg", pu32Args[0], "MessageIds", xLogEntry);

         if (u8ArgCount > 1)
         {
            // Check message data
            if (util::sMakeLower(xLogEntry->back().sValue).compare("keymessage") == 0)
            {
               // It's a key message
               vTransKeyMessage(pu32Args + 1, u8ArgCount - 1, xLogEntry);
            }
            else
            {
               // Use numeric data
               vTranslateArg("MsgData", pu32Args[1], "", xLogEntry);
            }
         }
      }
   }


   // ExecFunction name translator looks up the table. If there is a translation function for the
   // function name available, it is used. Otherwise the function name is translated using the
   // default translator.
   void vTrExecFunc(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stParam;
//         bool boNameFound = false;

         stParam.sName = "Name";

         if (false == ExecFunctions::boCodeToFuncName(pu32Args[0], stParam.sValue))
         {
            std::stringstream ss;
            ss << pu32Args[0];
            stParam.sValue = ss.str();
         }

         xLogEntry->push_back(stParam);

         if (u8ArgCount > 1)
         {
             stParam.sName = "Param";
             if (false == ExecFunctions::boCodeToFuncParam(pu32Args[0], pu32Args[1], stParam.sValue))
             {
                 std::stringstream ss;
                 ss << pu32Args[1];
                 stParam.sValue = ss.str();
             }
             xLogEntry->push_back(stParam);
         }
      }
   }

    // DPOOL name translator looks up the message ID. If there is a translation function for the
    // DPOOL name available, it is called. Otherwise the DPOOL name is translated using the
    // default translator.
    void vTrDPool(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
    {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stParam;
         bool boNameFound = false;

         stParam.sName = "Var";

         // Is there a enum to search in?
         if ( pu32Args[0] < ARRAY_SIZE(stTraceDPoolNameTable) )
         {
           stParam.sValue = stTraceDPoolNameTable[pu32Args[0]];
           boNameFound = true;
         }

         // If either no enum to search in, or nothing found: use numeric data
         if (!boNameFound)
         {
            std::stringstream ss;
            ss << pu32Args[0];
            stParam.sValue = ss.str();
         }

         xLogEntry->push_back(stParam);

         if (u8ArgCount > 1)
         {
            vTranslateArg("Arg", pu32Args[1], "", xLogEntry);
         }
      }
    }

    // SelfState name translator
    void vTrSState(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
    {
      if (u8ArgCount > 0)
      {
         vTranslateArg("State", pu32Args[0], "SSMState", xLogEntry);

          if (u8ArgCount > 1)
          {
             // Recurse for following arguments
             vDefTransArgs(pu32Args + 1, u8ArgCount - 1, xLogEntry);      
             //vTranslateArg("OldState", pu32Args[1], "SSMState", xLogEntry);
          }
      }
    }

    // Value translator
    void vTrValue(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
    {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stLogParam = {"Value"};
         std::stringstream ss;
         ss << (int)(*pu32Args);
         stLogParam.sValue = ss.str();
         xLogEntry->push_back(stLogParam);

         // Recurse for following arguments
         vDefTransArgs(pu32Args + 1, u8ArgCount - 1, xLogEntry);      
      }
      else
      {
         // End of recursion
      }
    }

    // Easing type name translator
    void vTrEasType(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
    {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stParam;
         bool boNameFound = false;

         stParam.sName = "Type";

         // Is there a enum to search in?
         if ( pu32Args[0] < ARRAY_SIZE(stEasingTypeNameTable) )
         {
           stParam.sValue = stEasingTypeNameTable[pu32Args[0]];
           boNameFound = true;
         }

         // If either no enum to search in, or nothing found: use numeric data
         if (!boNameFound)
         {
            std::stringstream ss;
            ss << pu32Args[0];
            stParam.sValue = ss.str();
         }

         xLogEntry->push_back(stParam);

         if (u8ArgCount > 1)
         {
            vTranslateArg("Arg", pu32Args[1], "", xLogEntry);
         }
      }
    }

    // Key name translator
    void vTrKey(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
    {
      if (u8ArgCount > 0)
      {
         TraceSection::tstLogEntryParam stParam;
         bool boNameFound = false;

         stParam.sName = "Key";

         // Is there a enum to search in?
         if ( pu32Args[0] < ARRAY_SIZE(stKeyNameTable) )
         {
           stParam.sValue = stKeyNameTable[pu32Args[0]];
           boNameFound = true;
         }

         // If either no enum to search in, or nothing found: use numeric data
         if (!boNameFound)
         {
            std::stringstream ss;
            ss << pu32Args[0];
            stParam.sValue = ss.str();
         }

         xLogEntry->push_back(stParam);

         if (u8ArgCount > 1)
         {
            vTranslateArg("Arg", pu32Args[1], "", xLogEntry);
         }
      }
    }

    void vTrHmiEvent(const std::string sArgName, const uint32 u32ArgValue, TraceSection::txLogEntry *xLogEntry)
    {
        TraceSection::tstLogEntryParam stParam;        
        bool boNameFound = false;

        stParam.sName = sArgName;
        
        // Is there a signal name for the data?
        for (int i = 0; i < ARRAY_SIZE(astHmiEventTable); i++)
        {
            if (u32ArgValue == astHmiEventTable[i].u32SignalId)
            {
                stParam.sValue = astHmiEventTable[i].sItemName;
                boNameFound = true;
                break;
            }
        }

        // If either no enum to search in, or nothing found: use numeric data
        if (!boNameFound)
        {
            std::stringstream ss;
            ss << u32ArgValue;
            stParam.sValue = ss.str();
        }

         xLogEntry->push_back(stParam);
    }
    
    void vTrSysEvent(const std::string sArgName, const uint32 u32ArgValue, TraceSection::txLogEntry *xLogEntry)
    {
        TraceSection::tstLogEntryParam stParam;
        bool boNameFound = false;

        stParam.sName = sArgName;

        // Is there a signal name for the data?
        for (int i = 0; i < ARRAY_SIZE(astSysEventTable); i++)
        {
            if (u32ArgValue == astSysEventTable[i].u32SignalId)
            {
                stParam.sValue = astSysEventTable[i].sItemName;
                boNameFound = true;
                break;
            }
        }

        // If either no enum to search in, or nothing found: use numeric data
        if (!boNameFound)
        {
            std::stringstream ss;
            ss << u32ArgValue;
            stParam.sValue = ss.str();
        }

        xLogEntry->push_back(stParam);
    }

    // Create parameter strings for KeyFilter event.
   void vTrSendEvent(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 0)
      {
         vTrSysEvent("Data", pu32Args[0], xLogEntry);
      }
   }

   // Create parameter strings for KeyFilter event.
   void vTrKeyFilter(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 1)
      {
         vTranslateArg("Keep", pu32Args[0], "", xLogEntry);         
         vTrHmiEvent("Data", pu32Args[1], xLogEntry);
      }
   }

   // Create parameter strings for vRun event.
   void vTrRun(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry)
   {
      if (u8ArgCount > 1)
      {
         vTranslateArg("ID", pu32Args[0], "", xLogEntry);
         vTrHmiEvent("Data", pu32Args[1], xLogEntry);
      }
   }
   
   void vTranslateEntryArgs(const SHMEM_tstTraceEntryArgs &stEntry, TraceSection::txLogEntry *xLogEntry)
   {
      TraceSection::tstLogEntryParam stLogParam;
      const tstItemAttr *pstAttr = pstGetItemAttr(stEntry.stHeader.u16Id);
      tpfArgsTranslator pfTrans = vDefTransArgs;

      if (pstAttr)
      {
         switch (pstAttr->u8Type)
         {
         case SHMEM_nTraceItemTypeEvent:
            stLogParam.sName = "Event";
            break;
         case SHMEM_nTraceItemTypeData:
            stLogParam.sName = "Data";
            break;
         default:
            util::Log::vPrint(util::LOG_ERROR, "Invalid trace item type for ID %i found", stEntry.stHeader.u16Id);
            break;
         }
         stLogParam.sValue = pstAttr->sItemName;
         if (pstAttr->pfArgsTrans)
            pfTrans = pstAttr->pfArgsTrans;

         //wti 20170117: Added timestamp 
         LARGE_INTEGER largeInt;
         QueryPerformanceFrequency(&largeInt);
         //wti: This may lead to overflow if timer runs very fast and/or machine is running for a long time
         __int64 microSec = ( stEntry.stHeader.i64Timestamp * 1000 * 1000 / largeInt.QuadPart);
         //workaround (reduce precision to millisecond):
         //if (stEntry.stHeader.i64Timestamp > (__int64)9000000000000)
         //{
         //    __int64 microSec = (stEntry.stHeader.i64Timestamp * 1000 / largeInt.QuadPart) * 1000;
         //}

         std::stringstream ss;
         ss << microSec;
         stLogParam.sTime = ss.str();  
         //wti: --- end of added timestamp ---
      }
      else
      {
         util::Log::vPrint(util::LOG_WARNING, "Unknown trace item of type args with ID %i found", stEntry.stHeader.u16Id);
         std::stringstream ss;
         ss << stEntry.stHeader.u16Id;
         stLogParam.sValue = ss.str();
      }

      xLogEntry->push_back(stLogParam);
      // Calculate argument count from size of arguments field
      uint8 u8ArgCount = (uint8) (stEntry.stHeader.u16ArgSize / sizeof(uint32));
      // If we have found item attributes, we can do additional checking
      if (pstAttr && (u8ArgCount != pstAttr->u8ArgCount))
         util::Log::vPrint(util::LOG_WARNING, "Configured argument count and actual argument count differ to trace item with ID %i", stEntry.stHeader.u16Id);
      pfTrans(stEntry.au32Args, u8ArgCount, xLogEntry);
   }


   ///////////////////////////////////////////////////////////////////////////
   // Part IV
   // Translation functions for binary trace data of block type

   // Default block translator prints bytes as hex string
   void vDefTransBlock(ByteBuffer &oData, TraceSection::txLogEntry *xLogEntry)
   {
      bool boFailed = false;
      TraceSection::tstLogEntryParam stLogParam;

      stLogParam.sName = "Data";
      while (!oData.boIsEmpty() && !boFailed)
      {
         uint8 u8Byte;
         if (oData.boGetNextItem(&u8Byte, 1))
         {
            const char hexstr[] = "0123456789ABCDEF";
            uint8 u8Low =  (u8Byte & 0x0F);
            uint8 u8High = (u8Byte & 0xF0) >> 4;
            stLogParam.sValue.push_back(hexstr[u8High]);
            stLogParam.sValue.push_back(hexstr[u8Low]);
            stLogParam.sValue.push_back(' ');
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading hex block data");
            boFailed = true;
         }
      }

      xLogEntry->push_back(stLogParam);
   }

   // Text string translator print block as text string
   void vTransTextString(ByteBuffer &oData, TraceSection::txLogEntry *xLogEntry)
   {
      bool boFailed = false;
      TraceSection::tstLogEntryParam stLogParam;

      stLogParam.sName = "Text";
      stLogParam.sValue.push_back('"');
      while (!oData.boIsEmpty() && !boFailed)
      {
         uint8 u8Byte;
         if (oData.boGetNextItem(&u8Byte, 1))
         {
            stLogParam.sValue.push_back(u8Byte);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading text block data");
            boFailed = true;
         }
      }

      stLogParam.sValue.push_back('"');
      xLogEntry->push_back(stLogParam);
   }

   void vTranslateEntryBlock(const SHMEM_tstTraceEntryBlock &stEntry, TraceSection::txLogEntry *xLogEntry)
   {
      TraceSection::tstLogEntryParam stLogParam;
      const tstItemAttr *pstAttr = pstGetItemAttr(stEntry.stHeader.u16Id);
      tpfBlockTranslator pfTrans = vDefTransBlock;

      if (pstAttr)
      {
         stLogParam.sName = "Block";
         stLogParam.sValue = pstAttr->sItemName;
         if (pstAttr->pfBlockTrans)
            pfTrans = pstAttr->pfBlockTrans;
      }
      else
      {
         util::Log::vPrint(util::LOG_WARNING, "Unknown trace item of type block with ID %i found", stEntry.stHeader.u16Id);
         std::stringstream ss;
         ss << stEntry.stHeader.u16Id;
         stLogParam.sValue = ss.str();
      }

      ByteBuffer oBlockData;
      oBlockData.vAppendItem(stEntry.au8Data, stEntry.stHeader.u16ArgSize);

      xLogEntry->push_back(stLogParam);
      pfTrans(oBlockData, xLogEntry);
   }

}
