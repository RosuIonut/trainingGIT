#ifndef SHMEM_TRACEITEMS_H
#define SHMEM_TRACEITEMS_H

typedef enum
{
   /*************************************************************************
    * Event items
    ************************************************************************/
   SHMEM_nenTrace_NextStepLegacy,
   SHMEM_nenTrace_NextStepHmi,
   SHMEM_nenTrace_KeyPress,
   SHMEM_nenTrace_KeyRelease,
   SHMEM_nenTrace_APIM_vTriggerStateChange,
   SHMEM_nenTrace_vToggleAPIMStateChange,
   SHMEM_nenTrace_boIsCriticalState,
   SHMEM_nenTrace_Error_WAS_Exc_MessageQueuePanic,
   SHMEM_nenTrace_Error_WAS_Exc_MessageQueueFull,
   SHMEM_nenTrace_SetPanicMode,
   SHMEM_nenTrace_SetHMIKl15,
   SHMEM_nenTrace_SwitchKl15,
   SHMEM_nenTrace_ExecFunction,
   SHMEM_nenTrace_SendEvent,
   SHMEM_nenTrace_EPFMI_vMmiKeyCallback,
   SHMEM_nenTrace_EPFMI_vMmiDoAction,
   SHMEM_nenTrace_CUH_vRegisterAppForKeys,
   SHMEM_nenTrace_CUH_vDeRegisterAppForKeys,
   SHMEM_nenTrace_vWAS_KeyFilterDoEventProcessing,
   SHMEM_nenTrace_vActivateFilter,
   SHMEM_nenTrace_vGetLastMessages,
   SHMEM_nenTrace_Filter_vActivate,
   SHMEM_nenTrace_KeyFilter_operator,
   SHMEM_nenTrace_vRun_Msg,
   SHMEM_nenTrace_WPAStoryBoard_vSetActive,
   SHMEM_nenTrace_WPAAnimationStartedOutEvent,
   SHMEM_nenTrace_StoryBoardDeactivated,
   SHMEM_nenTrace_WPAAnimationFinishedOutEvent,
   SHMEM_nenTrace_WPAStoryBoard_vTimeTick,
   SHMEM_nenTrace_WPAAnimationStarted,
   SHMEM_nenTrace_WPAAnimationStopped,
   SHMEM_nenTrace_WPAAnimationDestroyNotifcation,
   SHMEM_nenTrace_SelfWidgetBase_vOnAnimActive, 
   SHMEM_nenTrace_WPASBSelf_enProcessMessage,
   SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsInvert,
   SHMEM_nenTrace_WPASBSelf_vTimeTick,
   SHMEM_nenTrace_WPASBSelf_vSetActive,
   SHMEM_nenTrace_WPASBSelf_vStoreStaticValue,
   SHMEM_nenTrace_WPAESSelf_vStoreStaticValue,
   SHMEM_nenTrace_WPASBSelf_vRestoreModelValue,
   SHMEM_nenTrace_WPAESSelf_vRestoreModelValue,
   SHMEM_nenTrace_WPASBSelf_vRequestUpdateAnimParams,
   SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsSelf,
   SHMEM_nenTrace_WPASBSelf_vUpdateAnimParamsSelfRuntime,
   SHMEM_nenTrace_UpdateEasingForAllScenes,
   SHMEM_nenTrace_UpdateDuration,
   SHMEM_nenTrace_UpdateAnimation,
   SHMEM_nenTrace_m_vOnLongPressInit,
   SHMEM_nenTrace_LongPressStarted,
   SHMEM_nenTrace_m_vOnLongPressRetrigger,
   SHMEM_nenTrace_m_vOnLongPressReleased,
   
   SHMEM_nenTrace_Anima_vPropagateAnimData,
   SHMEM_nenTrace_Anima_vSetActiveAnimData,
   SHMEM_nenTrace_Anima_vSetWaitingAnimData,
   SHMEM_nenTrace_Anima_vSetTimeTickDirection,
   SHMEM_nenTrace_Anima_vOnPreLayoutingHook,
   SHMEM_nenTrace_SMG_vControllerSync,
   SHMEM_nenTrace_SMG_vInitActive15,
   SHMEM_nenTrace_SMG_vDeinitActive15,
   SHMEM_nenTrace_SMG_vInitHoldDisplay,
   SHMEM_nenTrace_SMG_vDeinitHoldDisplay,
   SHMEM_nenTrace_SMG_vDoEventProcessing,
   SHMEM_nenTrace_ILU_vInitActiveDisplay, 
   SHMEM_nenTrace_ILU_vDeinitActiveDisplay, 
   SHMEM_nenTrace_ILU_vCPrePostRunDeinit15,
   SHMEM_nenTrace_ILU_vCPrePostRunDoEventProcessing,
   SHMEM_nenTrace_SetReq_HoldDisplayByIluDisplayUsed,
   SHMEM_nenTrace_ClearReq_HoldDisplayByIluDisplayUsed,
   SHMEM_nenTrace_SetReq_Active15BySMGIgnition,
   SHMEM_nenTrace_ClearReq_Active15BySMGIgnition,
   SHMEM_nenTrace_SetReq_HoldDisplayBySMG,
   SHMEM_nenTrace_ClearReq_HoldDisplayBySMG,
   SHMEM_nenTrace_SetReq_ActiveDisplayBySMG,
   SHMEM_nenTrace_ClearReq_ActiveDisplayBySMG,
   SHMEM_nenTrace_TIMER_vStart_nIluPrePostRunTimer,
   SHMEM_nenTrace_TIMER_vStart_nActiveDisplayTimeout,
   SHMEM_nenTrace_srStateHMI_Inactive__ENV_InitEvent,
   SHMEM_nenTrace_srStateHMI_Reset__FW_InitEvent,
   SHMEM_nenTrace_srStateHMI_Reset__ENV_DeinitEvent,
   SHMEM_nenTrace_srStateHMI_Active__FW_DeinitEvent, 
   SHMEM_nenTrace_HMI_vReset,
   SHMEM_nenTrace_HMI_vWakeup,
   SHMEM_nenTrace_HMI_vSleep,
   SHMEM_nenTrace_HMI_vDisplayOn,
   SHMEM_nenTrace_HMI_vDisplayOff,
   SHMEM_nenTrace_HMI_vSyncInit,
   SHMEM_nenTrace_HMI_vAsyncDeInit,
   SHMEM_nenTrace_HMI_boSetDependency,
   SHMEM_nenTrace_APIM_vInitialization,
   SHMEM_nenTrace_APIM_vDeinitialization,
   SHMEM_nenTrace_APIM_Reset,
   SHMEM_nenTrace_APIM_boSetData,
// SHMEM_nenTrace_APIM_boGetData,
   SHMEM_nenTrace_APIM_boGetData__APIM_nDId_ai32DP_SINT,
   SHMEM_nenTrace_APIM_boGetData__APIM_nDId_aboDP_BOOL,
   SHMEM_nenTrace_WMMS_u8Init,
   SHMEM_nenTrace_WMMS_u8DeInit,
   SHMEM_nenTrace_boSendSignalWithoutData,
   SHMEM_nenTrace_transit_CSHStateHMI_Reset,
   SHMEM_nenTrace_transit_CSHStateHMI_Active,
   SHMEM_nenTrace_transit_CSHStateHMI_Inactive,
   SHMEM_nenTrace_API_Pool__vInitResources,
   SHMEM_nenTrace_API_Pool__vDeInitResources,
   SHMEM_nenTrace_API_Timer_vInitResource,
   SHMEM_nenTrace_API_Timer_vDeInitResource,
   SHMEM_nenTrace_WRM_u8Request,
   SHMEM_nenTrace_WRM__vCASendWarningNotification,
   SHMEM_nenTrace_WRA_ActivateZWL,
   SHMEM_nenTrace_boSignalSI__InitRES_HMI_Framwork_ANY,
   SHMEM_nenTrace_boSignalSI__DeinitRES_HMI_Framwork_ANY,
   SHMEM_nenTrace_CRHDL_enStateHdlGetState0,
   SHMEM_nenTrace_CRHDL_enStateHdlGetState1,
   SHMEM_nenTrace_boSignalSI_EV_DiagAdaption_0_Change,
   SHMEM_nenTrace_boSignalSI_EV_DiagCodingChange,
   SHMEM_nenTrace_boSignalSI_EV_DPOOL_OnChange_M_Language,
   SHMEM_nenTrace_GetDPOOL_M_Language,
   SHMEM_nenTrace_MFARoleContainer_enProcessMessage,
   SHMEM_nenTrace_MFARoleContainer_vProcessKey,
   SHMEM_nenTrace_SelfWidgetBase_m_vOnKeyPressed,
   SHMEM_nenTrace_SelfSSMCoreCommon_m_vOnKeyPressed,
   SHMEM_nenTrace_SelfWidgetMFA_boHandleKey,
   SHMEM_nenTrace_SelfWidgetList_boHandleKey,
   SHMEM_nenTrace_Framework_TriggerForcedPaint,
   SHMEM_nenTrace_SelfSSMCoreCommon_vUpdateMC,
   SHMEM_nenTrace_SelfWidgetWPABase_vUpdateAnimation,
   SHMEM_nenTrace_SelfWidgetBase_vUpdateStateSSMCore,
   SHMEM_nenTrace_SelfWidgetBase_stSetAnimData,
   SHMEM_nenTrace_SelfWidgetBase_stGetAnimData,
   SHMEM_nenTrace_SelfWidgetBase_vOnElapsed,
   SHMEM_nenTrace_SelfSSMCoreCommon_vOnElapsed,
   SHMEM_nenTrace_SelfSSMMoveCounter_vOnElapsed,
   SHMEM_nenTrace_vOnMFAIterationAnimationDone,
   SHMEM_nenTrace_SelfWidgetBase_boHandleKeySSMCore,
   SHMEM_nenTrace_vPropagateAnimDataActiveAnim,
   SHMEM_nenTrace_vPropagateAnimDataWaitingAnim,
   SHMEM_nenTrace_vSetUpdateFreezeModeInLastBeforeDraw,
   SHMEM_nenTrace_m_vOnLPInit,
   SHMEM_nenTrace_m_vActivateLPMode,
   SHMEM_nenTrace_m_vOnLPRetrigger,
   SHMEM_nenTrace_m_vOnLPRelease,
   SHMEM_nenTrace_m_vOnViewChange,
   SHMEM_nenTrace_m_vFlush,
   SHMEM_nenTrace_ControlAC_enProcessMessage,
   SHMEM_nenTrace_Messenger_vProcessMessage,
   SHMEM_nenTrace_SelfSSMCoreCommon_vProcessElapsed,
   SHMEM_nenTrace_ListAnimationVwFPK_enProcessMessage,
   SHMEM_nenTrace_SelfSSMCoreCommon_m_vRunStateMachine,
   SHMEM_nenTrace_KeyHandlerMaster_vInit,
   SHMEM_nenTrace_KeyHandlerMaster_vDeinit,
   SHMEM_nenTrace_HMI_boSendMessage,
   SHMEM_nenTrace_WIDVWFPK_vInitGlobalData,
   SHMEM_nenTrace_UTILS_vInitGlobalData,
   SHMEM_nenTrace_PAINTER_vInitGlobalData,
// SHMEM_nenTrace_HMItimedomain_vRun,
// SHMEM_nenTrace_APIM_Lock,
// SHMEM_nenTrace_APIM_pstGetClientHook,
// SHMEM_nenTrace_APIM_HMI_Lock_Pre,
// SHMEM_nenTrace_MOSTA_vPullPictureBuffer,
// SHMEM_nenTrace_APIM_HMI_Lock_Post,
// SHMEM_nenTrace_VSF_vHandleWarningDetailsRequests,
// SHMEM_nenTrace_VSF_vHandleWarningNumberOfLinesRequests,
// SHMEM_nenTrace_VSF_vCheckAPIChanges,
   SHMEM_nenTrace_CarouselContainer_enProcessMessage,
   SHMEM_nenTrace_SelfMainMenuRoll_boHandleKey,
   SHMEM_nenTrace_m_vSetFreezeMode,
   SHMEM_nenTrace_KeyHandlerMaster_vUnfreeze,
   SHMEM_nenTrace_vUpdateWaitingAnimation,
   SHMEM_nenTrace_vOnMsgAddedToKeyQueue,
   SHMEM_nenTrace_u8AddNewKeys,
   SHMEM_nenTrace_m_boNeutralizeNewKeysVirtualKeys,
   SHMEM_nenTrace_boInvert,
   SHMEM_nenTrace_vOnLastBeforeDraw,
   SHMEM_nenTrace_SelfMainMenuRoll_tCreateAnimation,
   SHMEM_nenTrace_KeyHandlerMaster_vFreeze,
   SHMEM_nenTrace_vActivateStaticModeViewSkipping,
   SHMEM_nenTrace_m_vRetryKeyProcessing,
   SHMEM_nenTrace_m_vOnViewChangeFlushKeyQueue,
   SHMEM_nenTrace_vFlushToSystemMsgQueue,
   SHMEM_nenTrace_KeyHandlerMaster_vDeactivate,
   SHMEM_nenTrace_vDiscardAllMessages,
   SHMEM_nenTrace_m_vRemoveAllVirtualKeysFromSSM,
   SHMEM_nenTrace_m_vOnViewChangeDiscardKeyQueue,
   SHMEM_nenTrace_boGetCfgDeactivateKHAtUnfreeze,
   SHMEM_nenTrace_KeyHandlerMaster_vActivate,
   SHMEM_nenTrace_KeyHandlerMaster_boAddMessage,
   SHMEM_nenTrace_HmiFrameId,
   SHMEM_nenTrace_WarningState_enProcessMessage,
   SHMEM_nenTrace_SelfWidgetWarningState_boHandleKey,
   SHMEM_nenTrace_ScrollBar_enProcessMessage,
   SHMEM_nenTrace_SelfOwnerPopupInvert_enProcessMessage,
   SHMEM_nenTrace_SelfPopupInvert_boHandleKey,
   SHMEM_nenTrace_boDisableInversion,
   SHMEM_nenTrace_CarouselContainer_start_timer,
   SHMEM_nenTrace_LayoutController_enProcessMessage,
   SHMEM_nenTrace_SelfLayout_boHandleKey,
   SHMEM_nenTrace_WDOG_vEnableEntity,
   SHMEM_nenTrace_WDOG_vDisableEntity,
   SHMEM_nenTrace_WDOG_vUpdateEntitiesList,
   SHMEM_nenTrace_DispatchEvent,
   SHMEM_nenTrace_Display_vOnLastBeforeDeactivation,
   SHMEM_nenTrace_AVICORE_vTaskDeactivationCallback,
   SHMEM_nenTrace_GRLC_vDisconnectSurfaceID,
   SHMEM_nenTrace_GRLC_vSurfaceDelete,
   SHMEM_nenTrace_GRLC_vLayerDelete,
   SHMEM_nenTrace_Display_vOnTaskActivated,
   SHMEM_nenTrace_GRLC_vVideoOutSetLayerUpdateMode,
   SHMEM_nenTrace_AVICORE_vTaskActivationCallback,
   SHMEM_nenTrace_GRLC_u16LayerCreate,
   SHMEM_nenTrace_GRLC_u16SurfaceCreate,
   SHMEM_nenTrace_GRLC_vConnectSurfaceID,
   SHMEM_nenTrace_GRLC_vLayerSetVisibility,
   SHMEM_nenTrace_GRLC_vLayerSetPerPixelTransparency,
   SHMEM_nenTrace_EEH_u8GetPixelOffset,
   SHMEM_nenTrace_EEH_i8Getp_Verschiebung_Displayinhalte,
   SHMEM_nenTrace_GRLC_vDisplaySetOffset,
   SHMEM_nenTrace_AdaptAPIM_APIM_HMI_Lock_Pre,
   SHMEM_nenTrace_AdaptAPIM_APIM_HMI_Lock_Post,
   
   /*************************************************************************
   * Data items
   ************************************************************************/
   SHMEM_nenTrace_HMIQueueLoad,
   SHMEM_nenTrace_SetDPOOL,
   SHMEM_nenTrace_GetDPOOL,
   SHMEM_nenTrace_SelfState_OLD,
   SHMEM_nenTrace_SelfState,
   SHMEM_nenTrace_MoveCounter,

   /*************************************************************************
    * Block items
    ************************************************************************/
   SHMEM_nenTrace_TextString,
   /********************************************/
   SHMEM_nenTraceItemCount
} SHMEM_tenTraceItemIds;

#endif