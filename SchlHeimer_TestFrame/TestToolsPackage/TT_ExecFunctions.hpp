#ifndef TT_EXECFUNCTIONS_HPP
#define TT_EXECFUNCTIONS_HPP

#include <vector>

/* Function IDs used as low byte of u16Argument for SHMEM_nSeqCodeExecFunction */
#define SHMEM_nSeqFunc_KL15On                       0x01
#define SHMEM_nSeqFunc_KL15Off                      0x02
#define SHMEM_nSeqFunc_FillMessageQueue             0x03
#define SHMEM_nSeqFunc_AddMessageQueueDelayItems    0x04
#define SHMEM_nSeqFunc_AddMessageQueueDiscardItems  0x05
#define SHMEM_nSeqFunc_AddMessageQueuePassItems     0x06
#define SHMEM_nSeqFunc_AddMessageQueueKeyItems1     0x07
#define SHMEM_nSeqFunc_AddMessageQueueKeyItems2     0x08
#define SHMEM_nSeqFunc_IncrementTageswegstrecke     0x09
#define SHMEM_nSeqFunc_DisplayUpDown                0x10
#define SHMEM_nSeqFunc_HmiEnvironment               0x12
#define SHMEM_nSeqFunc_OverVoltage                  0x13
#define SHMEM_nSeqFunc_Diagnostics                  0x14
#define SHMEM_nSeqFunc_SelectLanguage               0x15
#define SHMEM_nSeqFunc_SetLanguageInDPOOL           0x16
#define SHMEM_nSeqFunc_VolumeChange                 0x17
#define SHMEM_nSeqFunc_VZE_Request       			0x18
#define SHMEM_nSeqFunc_ActivateDeveloperMenu		0x19
#define SHMEM_nSeqFunc_DeveloperMenu_Down   		0x20
#define SHMEM_nSeqFunc_DeveloperMenu_Up     		0x21
#define SHMEM_nSeqFunc_DeveloperMenu_Press   		0x22
#define SHMEM_nSeqFunc_FullFillMessageQueue         0x23
#define SHMEM_nSeqFunc_VZE_Verkehrszeichen_1        0x24
#define SHMEM_nSeqFunc_ActivateWarning		    	0x25
#define SHMEM_nSeqFunc_Navigation_available      	0x26
#define SHMEM_nSeqFunc_Navi_CompassInfo_available   0x27
#define SHMEM_nSeqFunc_PresetsStatus	    	    0x28
#define SHMEM_nSeqFunc_CompassInfo_Direction_Angle	0x29

#define SHMEM_nSeqFunc_SystemEvent                  0x1A
//                used for debug                    0x1D
#define SHMEM_nSeqFunc_DebugFunction                0x1D

/* The high byte of u16Argument is used to defined the kind for ExecFunction parameter */
#define SHMEM_nSeqParamType_None                    0x00
#define SHMEM_nSeqParamType_Value                   0xFF
#define SHMEM_nSeqParamType_SetClear                0x01
#define SHMEM_nSeqParamType_EnableDisable           0x02
#define SHMEM_nSeqParamType_UpDown                  0x03
#define SHMEM_nSeqParamType_Language                0x04

/* Parameter for ExecFunctions SHMEM_nSeqCodeExecFunction */
// SHMEM_nSeqParamType_SetClear (OverVoltage)
#define SHMEM_nSeqParam_Clear               0
#define SHMEM_nSeqParam_Set                 1
// SHMEM_nSeqParamType_EnableDisable (HmiEnvironment / Diagnostics)
#define SHMEM_nSeqParam_Disable             0
#define SHMEM_nSeqParam_Enable              1
// SHMEM_nSeqParamType_UpDown (DisplayUpDown)
#define SHMEM_nSeqParam_DisplayUp           1
#define SHMEM_nSeqParam_DisplayDown         0
// SHMEM_nSeqParamType_Language (SelectLanguage)
#define SHMEM_nSeqParam_DiagAdaption        1
#define SHMEM_nSeqParam_DiagCoding          2
#define SHMEM_nSeqParam_M_Language          3

namespace ExecFunctions
{
int iFuncNameToCode(const std::string &sName);
int iFuncParamToCode(const std::string &sName);
bool boCodeToFuncName(const int FnCode, std::string &sName);
bool boCodeToFuncParam(const int FnCode, const int ParaCode, std::string &sName);
}

#endif
