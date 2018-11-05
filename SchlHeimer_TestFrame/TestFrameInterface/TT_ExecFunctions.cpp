extern "C"
{
#include "SHMEM_Layout.h"
}
#include <string>
#include <lib-util.hpp>

#include "TT_ExecFunctions.hpp"


namespace ExecFunctions
{

typedef struct {
   const char *sFunctionName;               // Name of ExecFunction
   uint8 u8ExecFunctionId;                  // Sequencer item ID
   uint8 u8FunctionParamType;               // Type of parameter 
} tstExecFunctionAttr;


typedef struct {
   uint8  u8FunctionParamType;              // Type of parameter 
   uint16 u16ParamValue;                    // Parameter value Type of parameter 
   const char *sParameterName;              // Name of a ExecFunction parameter
} tstParamNameDef;


/* Table of defined ExecFunctions with information about type of parameter */
const tstExecFunctionAttr astExecFunctionTable[] = {
    { "DebugFunction",                 SHMEM_nSeqFunc_DebugFunction,                SHMEM_nSeqParamType_None            },
    { "Kl15On",                        SHMEM_nSeqFunc_KL15On,                       SHMEM_nSeqParamType_None            },
    { "Kl15Off",                       SHMEM_nSeqFunc_KL15Off,                      SHMEM_nSeqParamType_None            },
    { "FillMessageQueue",              SHMEM_nSeqFunc_FillMessageQueue,             SHMEM_nSeqParamType_None            },
    { "FullFillMessageQueue",          SHMEM_nSeqFunc_FullFillMessageQueue,         SHMEM_nSeqParamType_None            },
    { "AddMessageQueueDelayItems",     SHMEM_nSeqFunc_AddMessageQueueDelayItems,    SHMEM_nSeqParamType_None            },
    { "AddMessageQueueDiscardItems",   SHMEM_nSeqFunc_AddMessageQueueDiscardItems,  SHMEM_nSeqParamType_None            },
    { "AddMessageQueuePassItems",      SHMEM_nSeqFunc_AddMessageQueuePassItems,     SHMEM_nSeqParamType_None            },
    { "AddMessageQueueKeyItems1",      SHMEM_nSeqFunc_AddMessageQueueKeyItems1,     SHMEM_nSeqParamType_Value           },
    { "AddMessageQueueKeyItems2",      SHMEM_nSeqFunc_AddMessageQueueKeyItems2,     SHMEM_nSeqParamType_Value           },
    { "IncrementTageswegstrecke",      SHMEM_nSeqFunc_IncrementTageswegstrecke,     SHMEM_nSeqParamType_None            },
    { "DisplayUpDown",                 SHMEM_nSeqFunc_DisplayUpDown,                SHMEM_nSeqParamType_UpDown          },
    { "HmiEnvironment",                SHMEM_nSeqFunc_HmiEnvironment,               SHMEM_nSeqParamType_EnableDisable   },
    { "OverVoltage",                   SHMEM_nSeqFunc_OverVoltage,                  SHMEM_nSeqParamType_SetClear        },
    { "Diagnostics",                   SHMEM_nSeqFunc_Diagnostics,                  SHMEM_nSeqParamType_EnableDisable   },
    { "SelectLanguage",                SHMEM_nSeqFunc_SelectLanguage,               SHMEM_nSeqParamType_Language        },
    { "SetLanguageInDPOOL",            SHMEM_nSeqFunc_SetLanguageInDPOOL,           SHMEM_nSeqParamType_Value           },
    { "VolumeChange",                  SHMEM_nSeqFunc_VolumeChange,                 SHMEM_nSeqParamType_None            },
	{ "ActivateWarning",               SHMEM_nSeqFunc_ActivateWarning,              SHMEM_nSeqParamType_Value           },
    { "VZE_Request",                   SHMEM_nSeqFunc_VZE_Request,                  SHMEM_nSeqParamType_Value           },
    { "VZE_Verkehrszeichen_1",         SHMEM_nSeqFunc_VZE_Verkehrszeichen_1,        SHMEM_nSeqParamType_Value           },
    { "Navigation_available",          SHMEM_nSeqFunc_Navigation_available,         SHMEM_nSeqParamType_Value           },
    { "Navi_CompassInfo_available",    SHMEM_nSeqFunc_Navi_CompassInfo_available,   SHMEM_nSeqParamType_Value           },
    { "PresetsStatus",                 SHMEM_nSeqFunc_PresetsStatus,                SHMEM_nSeqParamType_Value           },
    { "CompassInfo_Direction_Angle",   SHMEM_nSeqFunc_CompassInfo_Direction_Angle,  SHMEM_nSeqParamType_Value           },
	{ "ActivateDeveloperMenu",         SHMEM_nSeqFunc_ActivateDeveloperMenu,        SHMEM_nSeqParamType_None            },
	{ "DeveloperMenu_Down",            SHMEM_nSeqFunc_DeveloperMenu_Down,           SHMEM_nSeqParamType_None            },
	{ "DeveloperMenu_Up",              SHMEM_nSeqFunc_DeveloperMenu_Up,             SHMEM_nSeqParamType_None            },
	{ "DeveloperMenu_Press",           SHMEM_nSeqFunc_DeveloperMenu_Press,          SHMEM_nSeqParamType_None            },
};
    
/* Table of defined names for parameter values of the different types */
const tstParamNameDef astParamNameDefTable[] = {
    { SHMEM_nSeqParamType_SetClear,       SHMEM_nSeqParam_Clear,            "CLEAR"         },
    { SHMEM_nSeqParamType_SetClear,       SHMEM_nSeqParam_Set,              "SET"           },
    
    { SHMEM_nSeqParamType_EnableDisable,  SHMEM_nSeqParam_Disable,          "DISABLE"       },
    { SHMEM_nSeqParamType_EnableDisable,  SHMEM_nSeqParam_Enable,           "ENABLE"        },
    
    { SHMEM_nSeqParamType_UpDown,         SHMEM_nSeqParam_DisplayDown,      "DOWN"          },
    { SHMEM_nSeqParamType_UpDown,         SHMEM_nSeqParam_DisplayUp,        "UP"            },
    
    { SHMEM_nSeqParamType_Language,       SHMEM_nSeqParam_DiagAdaption,     "DiagAdaption"  },
    { SHMEM_nSeqParamType_Language,       SHMEM_nSeqParam_DiagCoding,       "DiagCoding"    },
    { SHMEM_nSeqParamType_Language,       SHMEM_nSeqParam_M_Language,       "M_Language"    },
};


int iFuncNameToCode(const std::string &sName)
{
    std::string sLocName = util::sMakeLower(sName);
    std::string sTblName;

    for ( int i = 0; i < ARRAY_SIZE(astExecFunctionTable); i++ )
    {
        sTblName = util::sMakeLower(astExecFunctionTable[i].sFunctionName);
    
        if (sLocName.compare(sTblName) == 0) 
        {
            return astExecFunctionTable[i].u8ExecFunctionId 
                 | ((uint16)astExecFunctionTable[i].u8FunctionParamType << 8);
        }
   }
   
   return -1;
}


int iFuncParamToCode(const std::string &sName)
{
    std::string sLocName = util::sMakeLower(sName);
    std::string sTblName;

    for ( int i = 0; i < ARRAY_SIZE(astParamNameDefTable); i++ )
    {
        sTblName = util::sMakeLower(astParamNameDefTable[i].sParameterName);
    
        if (sLocName.compare(sTblName) == 0)
        {
            return astParamNameDefTable[i].u16ParamValue;
        }
   }
   
   return -1;
}


bool boCodeToFuncName(const int FnCode, std::string &sName)
{
    bool ret = false;
    
    for (int i = 0; i < ARRAY_SIZE(astExecFunctionTable); i++)
    {
        if (astExecFunctionTable[i].u8ExecFunctionId == (uint8)(FnCode & 0xFF))
        {
            sName = astExecFunctionTable[i].sFunctionName;
            ret = true;
        }
   }
    
    return ret;
};


bool boCodeToFuncParam(const int FnCode, const int ParaCode, std::string &sName)
{
    bool ret = false;

    for (int i = 0; i < ARRAY_SIZE(astParamNameDefTable); i++)
    {
        if ((astParamNameDefTable[i].u8FunctionParamType == (uint8)(FnCode >> 8))
            && (astParamNameDefTable[i].u16ParamValue == ParaCode))
        {
            sName = astParamNameDefTable[i].sParameterName;
            ret = true;
        }
    }

    return ret;
};

}
