#include <lib-util.hpp>

#include "lib-names.hpp"

#include "..\..\adapt\gen\apimgr\APIM__GenSystemEvents.h"
#include <stdint.h>
using namespace HMI::APIM;
#include "target-enums.hpp"

namespace names
{
   typedef struct {
      const char *sName;
      int iValue;
   } tstEnumEntry;

   typedef struct {
      const char *sName;
      tstEnumEntry *pstEntries;
      int iCount;
   } tstEnum;

   tstEnumEntry astMessageIds[] =
   {
      /* Element name                               Element value                                   */                                        
      { "MsgID_UpdateDataBinding",                  target::MsgID_UpdateDataBinding                 },
      { "MsgID_ForceRedraw",                        target::MsgID_ForceRedraw                       },
      { "MsgID_LastBeforeDraw",                     target::MsgID_LastBeforeDraw                    },
      { "MsgID_APIUpdateDone",                      target::MsgID_APIUpdateDone                     },
      { "MsgID_TreeInit",                           target::MsgID_TreeInit                          },
      { "MsgID_TreeBuildDone",                      target::MsgID_TreeBuildDone                     },
      { "MsgID_ChangeTreeNotification",             target::MsgID_ChangeTreeNotification            },
      { "MsgID_PreDestroyNotification",             target::MsgID_PreDestroyNotification            },
      { "MsgID_SetterControllerStrategy",           target::MsgID_SetterControllerStrategy          },
      { "MsgID_ChangeControllerState",              target::MsgID_ChangeControllerState             },
      { "MsgID_PreNotify",                          target::MsgID_PreNotify                         },
      { "MsgID_NotifyPropChange",                   target::MsgID_NotifyPropChange                  },
      { "MsgID_NotifyDependencyChange",             target::MsgID_NotifyDependencyChange            },
      { "MsgID_SetDependency",                      target::MsgID_SetDependency                     },
      { "MsgID_ForceSetDependency",                 target::MsgID_ForceSetDependency                },
      { "MsgID_TimerActivated",                     target::MsgID_TimerActivated                    },
      { "MsgID_KillFilterModelKeyDisplacement",     target::MsgID_KillFilterModelKeyDisplacement    },
      { "MsgID_StopSelfFilterMessage",              target::MsgID_StopSelfFilterMessage             },
      { "MsgID_FilterPreactivate",                  target::MsgID_FilterPreactivate                 },
      { "MSG_ID_WPA_STARTANIMATION",                target::MSG_ID_WPA_STARTANIMATION               },
      { "MSG_ID_WPA_STOPANIMATION",                 target::MSG_ID_WPA_STOPANIMATION                },
      { "MSG_ID_WPA_ANIMATIONSTATE",                target::MSG_ID_WPA_ANIMATIONSTATE               },
      { "MSG_ID_WPA_ANIMATIONTRIGGERSTATUS",        target::MSG_ID_WPA_ANIMATIONTRIGGERSTATUS       },
      { "MSG_ID_WPA_ACTIVATESTORYBOARD",            target::MSG_ID_WPA_ACTIVATESTORYBOARD           },
      { "MSG_ID_WPA_STORYBOARDSTATECHANGE",         target::MSG_ID_WPA_STORYBOARDSTATECHANGE        },
      { "MSG_ID_WPA_PROPERTYCHANGENOTIFICATION",    target::MSG_ID_WPA_PROPERTYCHANGENOTIFICATION   },
      { "MSG_ID_WPA_EASINGAC_UPDATESTARTVALUE",     target::MSG_ID_WPA_EASINGAC_UPDATESTARTVALUE    },
      { "MSG_ID_WPA_EASINGAC_UPDATEENDVALUE",       target::MSG_ID_WPA_EASINGAC_UPDATEENDVALUE      },
      { "EVENT_MenuManager_MSG_SYSTEM_EVENT",       target::EVENT_MenuManager_MSG_SYSTEM_EVENT      },
      { "EVENT_MenuManager_MSG_HARDKEY_EVENT",      target::EVENT_MenuManager_MSG_HARDKEY_EVENT     },
      { "MSG_ID_WARNING_POPUP_ACTIVATED",           target::MSG_ID_WARNING_POPUP_ACTIVATED			},
      { "MSG_ID_WARNING_POPUP_DEACTIVATED",         target::MSG_ID_WARNING_POPUP_DEACTIVATED		},
      { "MSG_ID_EVENTSENDER_SELF_MSG",              target::MSG_ID_EVENTSENDER_SELF_MSG				},
      { "MSG_ID_CAROUSEL_LONGPRESS_LOOP",           target::MSG_ID_CAROUSEL_LONGPRESS_LOOP			},
      { "FIRST_VISIBLE_BAPID_CHANGED",              target::FIRST_VISIBLE_BAPID_CHANGED             },
      { "FOCUSED_BAPID_CHANGED",                    target::FOCUSED_BAPID_CHANGED                   },
      
   };

/*   tstEnumEntry astWidgetIds[] =
   { */
      /*Element name								Element value                                     */
/*	  { "Widget_FPK_DigVel",						target::Widget_FPK___cBACK_LIGHT_ON__cK150N__cINCLUDE_VIEW_REPOSITORY__vREF_All_COMMON__REF_All_COMMON__DigitalSpeed__LY0__DigVel },
	  { "Widget_FPK_VZE",							target::Widget_FPK___cBACK_LIGHT_ON__cK150N__cINCLUDE_VIEW_REPOSITORY__vREF_TRAFFIC_SIGN__REF_TRAFFIC_SIGN__VZE_Container__VZE__TrafficSign },
	  { "Widget_FPK_ACC_Container",					target::Widget_FPK___cBACK_LIGHT_ON__cK150N__cINCLUDE_VIEW_REPOSITORY__vREF_ACC_GRA_LIM_LDW__REF_ACC__ACC_GRA_LIM_LDW__GRA_LIM_PEA__SpeedStatus__LIM_LAM__ACC_Container },
	  { "Widget_FPK_PACA_Container",				target::Widget_FPK___cBACK_LIGHT_ON__cK150N__cINCLUDE_VIEW_REPOSITORY__vREF_ACC_GRA_LIM_LDW__REF_ACC__ACC_GRA_LIM_LDW__GRA_LIM_PEA__SpeedStatus__PACA_Container__Paca },
   }; */

   tstEnumEntry astSignalIds[] =
   {
      /* Element name                                   Element value                                     */
      { "SI_EV_CRHDL_InitRES_HMI_Environment_ANY",      target::SI_EV_CRHDL_InitRES_HMI_Environment_ANY    },
      { "SI_EV_Init_RES_HMI_Environment_BY_HMI_Comp",   target::SI_EV_Init_RES_HMI_Environment_BY_HMI_Comp },
      
      { "SI_EV_CRHDL_InitRES_HMI_Framwork_ANY",         target::SI_EV_CRHDL_InitRES_HMI_Framwork_ANY       },
      { "SI_EV_Init_RES_HMI_Framwork_BY_HMI_Comp",      target::SI_EV_Init_RES_HMI_Framwork_BY_HMI_Comp    },
      
      { "SI_EV_CRHDL_DeinitRES_HMI_Framwork_ANY",       target::SI_EV_CRHDL_DeinitRES_HMI_Framwork_ANY     },
      { "SI_EV_Deinit_RES_HMI_Framwork_BY_HMI_Comp",    target::SI_EV_Deinit_RES_HMI_Framwork_BY_HMI_Comp  },
      
      { "SI_EV_CRHDL_DeinitRES_HMI_Environment_ANY",    target::SI_EV_Init_RES_HMI_Environment_BY_HMI_Comp },
      { "SI_EV_Deinit_RES_HMI_Environment_BY_HMI_Comp", target::SI_EV_Deinit_RES_HMI_Environment_BY_HMI_Comp },
      

   };

   tstEnumEntry astSndEvents[] =
   {  /* Element name                                   Element value                                     */
	  { "KL_15_ACTIVATED",								target::EV_APIM_SysEv_KL_15_ACTIVATED     },
      { "KL_15_DEACTIVATED",							target::EV_APIM_SysEv_KL_15_DEACTIVATED   },
   };

   tstEnumEntry astRecEvents[] =
   {
      /* Element name                                           Element value                                     */
      { "EV_ControllerSyncChanged_SMG_vDoEventProcessing",      target::EV_ControllerSyncChanged_SMG_vDoEventProcessing     },
   };

   tstEnumEntry astApimDataId[] =
   {
      /* Element name                                   Element value                               */
      { "APIM_nDId_aboDP_BOOL",                         target::APIM_nDId_aboDP_BOOL                },
      { "APIM_nDId_ai32DP_SINT",                        target::APIM_nDId_ai32DP_SINT               },
   };

   tstEnumEntry astDependentType[] =
   {
      /* Element name                                   Element value                               */
      { "RSST_nDep_Independent",                        target::RSST_nDep_Independent               },
      { "RSST_nDep_Language",                           target::RSST_nDep_Language                  },
      { "RSST_nDep_Brand",                              target::RSST_nDep_Brand                     },
      { "RSST_nDep_DefaultLanguage",                    target::RSST_nDep_DefaultLanguage           },
      { "RSST_nDep_Theme",                              target::RSST_nDep_Theme                     },
   };

   tstEnumEntry astCRHDL_SMState[] =
   {
      /* Element name                                   Element value                               */
      { "CRHDL_nStateOn",                               target::CRHDL_nStateOn                      },
      { "CRHDL_nStateOff",                              target::CRHDL_nStateOff                     },
      { "CRHDL_nStateInit",                             target::CRHDL_nStateInit                    },
      { "CRHDL_nStateDeinit",                           target::CRHDL_nStateDeinit                  },
      { "CRHDL_nInvalidState",                          target::CRHDL_nInvalidState                 },
   };

   tstEnumEntry astSSMState[] =
   {
      /* Element name                                   Element value                               */
      { "Idle",                                         target::nenSSMStateIdle                     },
      { "Normal",                                       target::nenSSMStateNormal                   },
      { "Invert",                                       target::nenSSMStateInvert                   },
	  { "SSMStates",									target::nenSSMStates						},
	  { "Invalid",										target::nenSSMInvalidState					},
   };
            
   tstEnumEntry astTimeDomainEvents[] =
   {
      /* Element name                                   Element value                               */
      { "nenWakeUp",                                    target::nenWakeUp                           },
      { "nenDisplayOn",                                 target::nenDisplayOn                        },
      { "nenDisplayOff",                                target::nenDisplayOff                       },
      { "nenTaskRun",                                   target::nenTaskRun                          },
      { "nenTimerRun",                                  target::nenTimerRun                         },
      { "nenSleep",                                     target::nenSleep                            },
      { "nenDisableProcessing",                         target::nenDisableProcessing                },
    };

   tstEnumEntry astBool[] =
   {
      /* Element name                          Element value                                      */
      { "false",                               0                                                  },
	  { "true",                                1                                                  },
   };

   tstEnum astEnumList[] =
   {
      /* Enum name             Table with entries      Number of entries                 */
      { "MessageIds",          astMessageIds,          ARRAY_SIZE(astMessageIds)         },
//      { "WidgetIds",           astWidgetIds,           ARRAY_SIZE(astWidgetIds)          },
      { "SignalIds",           astSignalIds,           ARRAY_SIZE(astSignalIds)          },
      { "SndEvents",           astSndEvents,           ARRAY_SIZE(astSndEvents)          },
      { "RecEvents",           astRecEvents,           ARRAY_SIZE(astRecEvents)          },
      { "ApimDataId",          astApimDataId,          ARRAY_SIZE(astApimDataId)         },
      { "DependentType",       astDependentType,       ARRAY_SIZE(astDependentType)      },
      { "CRDHL_SMState",       astCRHDL_SMState,       ARRAY_SIZE(astCRHDL_SMState)      },
      { "SSMState",            astSSMState,            ARRAY_SIZE(astSSMState)           },
      { "TimeDomainEvents",    astTimeDomainEvents,    ARRAY_SIZE(astTimeDomainEvents)   },
   };


   const tstEnum *pstFindEnum(const std::string &sEnum)
   {
      std::string sLcEnum = util::sMakeLower(sEnum);

      for (int i = 0; i < ARRAY_SIZE(astEnumList); i++)
         if (sLcEnum.compare(util::sMakeLower(astEnumList[i].sName)) == 0)
            return &astEnumList[i];

      return NULL;
   }

   bool boFindEnumValue(const std::string &sEnum, const std::string &sName, int *piValue)
   {
      bool boFound = false;

      std::string sLcName = util::sMakeLower(sName);
      const tstEnum *pstEnum = pstFindEnum(sEnum);
      if (pstEnum)
      {
         for (int i = 0; !boFound && (i < pstEnum->iCount); i++)
         {
            if (sLcName.compare(util::sMakeLower(pstEnum->pstEntries[i].sName))== 0)
            {
               *piValue = pstEnum->pstEntries[i].iValue;
               boFound = true;
            }
         }
      }

      return boFound;
   }

   bool boFindEnumName(const std::string &sEnum, int iValue, std::string *sName)
   {
      bool boFound = false;

      const tstEnum *pstEnum = pstFindEnum(sEnum);
      if (pstEnum)
      {
         for (int i = 0; !boFound && (i < pstEnum->iCount); i++)
         {
            if (pstEnum->pstEntries[i].iValue == iValue)
            {
               *sName = pstEnum->pstEntries[i].sName;
               boFound = true;
            }
         }
      }

      return boFound;
   }

}
