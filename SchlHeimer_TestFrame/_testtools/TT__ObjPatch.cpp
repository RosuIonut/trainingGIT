extern "C"
{
#include "TT__ObjPatch.h"
}

/*#include "WPA_Scene.hpp"
#include "WPA_EasingScene.hpp"
#include "WPA_EasingSceneSelf.hpp"
#include "WPA_AnimControl.hpp"
#include "WPA_ACFade.hpp"
#include "WPA_ACMove.hpp"
#include "WPA_ACScale.hpp"
#include "ANTF_RuntimeConfiguration.h"*/

typedef bool (*TT__tpfGetProperty)(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value);
typedef bool (*TT__tpfSetProperty)(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value);
typedef struct
{
   TT__tpfGetProperty boGetProperty;
   TT__tpfSetProperty boSetProperty;
} TT__tstPropMethods;

static struct
{
   bool boActive;
   uint8 u8PatchCount;
   SHMEM_tstObjPatchSpec *pstPatches;
} TT__stObjPatch;

void TT__vObjPatchInit(void)
{
   TT__stObjPatch.boActive = false;
}

void TT__vObjPatchOnDownload(void *pvData, uint32 u32Length)
{
   SHMEM_tstObjPatchHeader *pstHeader = (SHMEM_tstObjPatchHeader*) pvData;

   if (pstHeader->u8PatchCount > 0)
   {
      TT__stObjPatch.u8PatchCount = pstHeader->u8PatchCount;
      TT__stObjPatch.pstPatches = (SHMEM_tstObjPatchSpec*) (pstHeader + 1);
      TT__stObjPatch.boActive = true;
   }
}

void TT__vObjPatchConfirmUpload(void)
{
   // Disable patch mechanism until new patch data is received
   TT__stObjPatch.boActive = false;
}

static SHMEM_tenObjTypes TT__enIdentifyObject(const void *pvObject, SHMEM_tenObjTypes enObjType)
{
   // This function looks at "magic stuff" to decide which derived type fits
   SHMEM_tenObjTypes enSpecificType = enObjType;

   /*
   switch (enObjType)
   {
   case SHMEM_nenObjType_Scene:
      // The caller passed it as Scene.
      // Let's check if it is of a more specific type we know.
      switch (((HMI::WFC::Widget*) pvObject)->enGetType())
      {
      case HMI::WRS::WidgetClassID_HMI_WPA_WPA_EasingSceneSelf:
         enSpecificType = SHMEM_nenObjType_EasingSceneSelf;
         break;
      }
      break;

   case SHMEM_nenObjType_AnimControl:
      // The caller passed it as AnimControl.
      // Let's check if it is of a more specific type we know.
      switch (((HMI::WFC::Widget*) pvObject)->enGetType())
      {
      case HMI::WRS::WidgetClassID_HMI_WPA_WPA_ACFade:
         enSpecificType = SHMEM_nenObjType_ACFade;
         break;
      case HMI::WRS::WidgetClassID_HMI_WPA_WPA_ACMove:
         enSpecificType = SHMEM_nenObjType_ACMove;
         break;
      case HMI::WRS::WidgetClassID_HMI_WPA_WPA_ACScale:
         enSpecificType = SHMEM_nenObjType_ACScale;
         break;
      }
      break;
   }*/

   return enSpecificType;
}

static bool TT__boGetProp_Scene(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_Scene *poScene = (const HMI::WPA::WPA_Scene*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //*pu32Value = poScene->u8GetNumOfFrames();
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boSetProp_Scene(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*HMI::WPA::WPA_Scene *poScene = (HMI::WPA::WPA_Scene*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //poScene->vSetNumOfFrames((uint8) u32Value);
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boGetProp_EasingSceneSelf(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_EasingSceneSelf *poEasingSceneSelf = (const HMI::WPA::WPA_EasingSceneSelf*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //*pu32Value = poEasingSceneSelf->u8GetNumOfFrames();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EasingTypeWpa:
      //*pu32Value = (uint32) poEasingSceneSelf->enGetEasingType();
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boSetProp_EasingScene(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*HMI::WPA::WPA_EasingScene *poEasingScene = (HMI::WPA::WPA_EasingScene*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //poEasingScene->vSetNumOfFrames((uint8) u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EasingTypeWpa:
      //poEasingScene->vSetEasingType((HMI::WPA::WPA_tenEasingType) u32Value);
      boResult = true;
      break;
   }

   return boResult;*/
}




static bool TT__boGetProp_EasingScene(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_EasingScene *poEasingScene = (const HMI::WPA::WPA_EasingScene*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //*pu32Value = poEasingScene->u8GetNumOfFrames();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EasingTypeWpa:
      //*pu32Value = (uint32) poEasingScene->enGetEasingType();
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boSetProp_EasingSceneSelf(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*HMI::WPA::WPA_EasingSceneSelf *poEasingSceneSelf = (HMI::WPA::WPA_EasingSceneSelf*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_NumOfFrames:
      //poEasingSceneSelf->vSetNumOfFrames((uint8) u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EasingTypeWpa:
      //poEasingSceneSelf->vSetEasingType((HMI::WPA::WPA_tenEasingType) u32Value);
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boGetProp_ACFade(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_ACFade *poFade = (const HMI::WPA::WPA_ACFade*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartAlpha:
      //*pu32Value = poFade->u8GetStartAlpha();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndAlpha:
      //*pu32Value = poFade->u8GetEndAlpha();
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boSetProp_ACFade(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*HMI::WPA::WPA_ACFade *poFade = (HMI::WPA::WPA_ACFade*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartAlpha:
      //poFade->vSetStartAlpha((uint8) u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndAlpha:
      //poFade->vSetEndAlpha((uint8) u32Value);
      boResult = true;
      break;
   }

   return boResult;Ü*/
}

static bool TT__boGetProp_ACMove(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_ACMove *poMove = (const HMI::WPA::WPA_ACMove*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartPos:
      //*pu32Value = poMove->i16GetStartPos();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndPos:
      //*pu32Value = poMove->i16GetEndPos();
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boSetProp_ACMove(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*HMI::WPA::WPA_ACMove *poMove = (HMI::WPA::WPA_ACMove*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartPos:
      //poMove->vSetStartPos((int16) u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndPos:
      //poMove->vSetEndPos((int16) u32Value);
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boGetProp_ACScale(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
   /*const HMI::WPA::WPA_ACScale *poScale = (const HMI::WPA::WPA_ACScale*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartFactorX:
      //*pu32Value = poScale->u32GetStartScaleFactorX();
      boResult = true;
      break;
   case SHMEM_nenObjProp_StartFactorY:
      //*pu32Value = poScale->u32GetStartScaleFactorY();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndFactorX:
      //*pu32Value = poScale->u32GetEndScaleFactorX();
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndFactorY:
      //*pu32Value = poScale->u32GetEndScaleFactorY();
      boResult = true;
      break;
   }

   return boResult;*/
}


static bool TT__boSetProp_ACScale(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
    /*HMI::WPA::WPA_ACScale *poScale = (HMI::WPA::WPA_ACScale*) pvObject;
   bool boResult = false;

   switch (enPropId)
   {
   case SHMEM_nenObjProp_StartFactorX:
      //poScale->vSetStartScaleFactorX(u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_StartFactorY:
      //poScale->vSetStartScaleFactorY(u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndFactorX:
      //poScale->vSetEndScaleFactorX(u32Value);
      boResult = true;
      break;
   case SHMEM_nenObjProp_EndFactorY:
      //poScale->vSetEndScaleFactorY(u32Value);
      boResult = true;
      break;
   }

   return boResult;*/
}

static bool TT__boGetSubItemInfo_ANTFRuntimeCfg(SHMEM_tenObjPropIds enPropId, uint8 *pu8SubItem, uint8 *pu8BasePropId)
{
   bool boResult = false;

   /*if (enPropId >= SHMEM_nenObjProp_IsActive1 && enPropId <= SHMEM_nenObjProp_EasingVariant3)
   {
      // u8PropIndex       is the property index, starting with zero
      // u8PropsPerItem    is the number of properties per sub item
      // Make sure that always the first property ID of a set is used for these
      // calculations (currently this is IsActive)
      const uint8 u8PropIndex = enPropId - SHMEM_nenObjProp_IsActive1;
      const uint8 u8PropsPerItem = SHMEM_nenObjProp_IsActive2 - SHMEM_nenObjProp_IsActive1;
      // Get index of subitem of the array.
      *pu8SubItem = u8PropIndex / u8PropsPerItem;
      // Normalize property ID to match enum elements of first subitem
      // (for use as case label to prevent magic numbers here)
      *pu8BasePropId = (u8PropIndex % u8PropsPerItem) + SHMEM_nenObjProp_IsActive1;

      boResult = true;
   }*/

   return boResult;
}

static bool TT__boGetProp_ANTFRuntimeCfg(const void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 *pu32Value)
{
    return false;
/*  const tstItem **pstItem = (const tstItem**) pvObject;
   bool boResult = false;

   // Index into array of items
   uint8 u8SubItem;
   // Property to access, normalized to property IDs for first sub item
   uint8 u8BasePropId;
   if (TT__boGetSubItemInfo_ANTFRuntimeCfg(enPropId, &u8SubItem, &u8BasePropId))
   {
      switch (u8BasePropId)
      {
      case SHMEM_nenObjProp_IsActive1:
         *pu32Value = pstItem[u8SubItem]->boIsActive;
         boResult = true;
         break;
      case SHMEM_nenObjProp_DataType1:
         *pu32Value = pstItem[u8SubItem]->u8Type;
         boResult = true;
         break;
      case SHMEM_nenObjProp_InstanceID1:
         *pu32Value = pstItem[u8SubItem]->u16InstanceID;
         boResult = true;
         break;
      case SHMEM_nenObjProp_Duration1:
         *pu32Value = pstItem[u8SubItem]->stDuration.u16DurationMS;
         boResult = true;
         break;
      case SHMEM_nenObjProp_EasingType1:
         *pu32Value = pstItem[u8SubItem]->stEasing.u32EasingType;
         boResult = true;
         break;
      case SHMEM_nenObjProp_EasingVariant1:
         *pu32Value = pstItem[u8SubItem]->stEasing.u32EasingVariant;
         boResult = true;
         break;
      }
   }

   return boResult;*/
}

static bool TT__boSetProp_ANTFRuntimeCfg(void *pvObject, SHMEM_tenObjPropIds enPropId, uint32 u32Value)
{
    return false;
   /*tstItem **pstItem = (tstItem**) pvObject;
   bool boResult = false;

   // Index into array of items
   uint8 u8SubItem;
   // Property to access, normalized to property IDs for first sub item
   uint8 u8BasePropId;
   if (TT__boGetSubItemInfo_ANTFRuntimeCfg(enPropId, &u8SubItem, &u8BasePropId))
   {
      switch (u8BasePropId)
      {
      case SHMEM_nenObjProp_IsActive1:
         pstItem[u8SubItem]->boIsActive = u32Value != 0;
         boResult = true;
         break;
      case SHMEM_nenObjProp_DataType1:
         pstItem[u8SubItem]->u8Type = (uint8)u32Value;
         boResult = true;
         break;
      case SHMEM_nenObjProp_InstanceID1:
         pstItem[u8SubItem]->u16InstanceID = (uint16)u32Value;
         boResult = true;
         break;
      case SHMEM_nenObjProp_Duration1:
         pstItem[u8SubItem]->stDuration.u16DurationMS = (uint16)u32Value;
         boResult = true;
         break;
      case SHMEM_nenObjProp_EasingType1:
         pstItem[u8SubItem]->stEasing.u32EasingType = u32Value;
         boResult = true;
         break;
      case SHMEM_nenObjProp_EasingVariant1:
         pstItem[u8SubItem]->stEasing.u32EasingVariant = u32Value;
         boResult = true;
         break;
      }
   }

   return boResult;*/
}

/*
static TT__tstPropMethods TT__astPropMethods[] =
{
   * Enum as index                         Getter                          Setter                         
   /* SHMEM_nenObjType_Scene           * { TT__boGetProp_Scene,            TT__boSetProp_Scene            },
   /* SHMEM_nenObjType_EasingScene     * { TT__boGetProp_EasingScene,      TT__boSetProp_EasingScene      },
   /* SHMEM_nenObjType_EasingSceneSelf * { TT__boGetProp_EasingSceneSelf,  TT__boSetProp_EasingSceneSelf  },
   /* SHMEM_nenObjType_AnimControl     *{ NULL,                           NULL                           },
   /* SHMEM_nenObjType_ACFade          * { TT__boGetProp_ACFade,           TT__boSetProp_ACFade           },
   /* SHMEM_nenObjType_ACMove          * { TT__boGetProp_ACMove,           TT__boSetProp_ACMove           },
   /* SHMEM_nenObjType_ACScale         * { TT__boGetProp_ACScale,          TT__boSetProp_ACScale          },
   /* SHMEM_nenObjType_ANTFRuntimeCfg  * { TT__boGetProp_ANTFRuntimeCfg,   TT__boSetProp_ANTFRuntimeCfg   },
};*/

static bool TT__boIsPatchTarget(const void *pvObject, SHMEM_tenObjTypes enObjType, const SHMEM_tstObjPatchSpec *pstSpec)
{
   return false;
   /*bool boResult = false;
   
   if (enObjType < SHMEM_nenObjTypeCount)
   {
      if (pstSpec->u8FilterProperty == SHMEM_nenObjProp_Nothing)
         return true;

      SHMEM_tenObjPropIds enProp = (SHMEM_tenObjPropIds) pstSpec->u8FilterProperty;
      TT__tpfGetProperty boGetProp = TT__astPropMethods[enObjType].boGetProperty;
      if (boGetProp)
      {
         uint32 u32Value;
         if (boGetProp(pvObject, enProp, &u32Value))
         {
            boResult = u32Value == pstSpec->u32FilterValue;
         }
      }
   }

   return boResult;*/
}

static void TT__vApplyPatch(void *pvObject, SHMEM_tenObjTypes enObjType, SHMEM_tstObjPatchSpec *pstSpec)
{
   return;
   /*if (enObjType < SHMEM_nenObjTypeCount)
   {
      SHMEM_tenObjPropIds enProp = (SHMEM_tenObjPropIds) pstSpec->u8PatchProperty;
      TT__tpfSetProperty boSetProp = TT__astPropMethods[enObjType].boSetProperty;
      if (boSetProp)
      {
         if (boSetProp(pvObject, enProp, pstSpec->u32PatchValue))
         {
            if (pstSpec->u8ApplyCounter < 0xff)
               pstSpec->u8ApplyCounter++;
         }
      }
   }*/
}

void TT__vPatchObject(void *pvObject, SHMEM_tenObjTypes enType)
{
   /*if (TT__stObjPatch.boActive && (enType < SHMEM_nenObjTypeCount))
   {
      SHMEM_tenObjTypes enSpecificType = TT__enIdentifyObject(pvObject, enType);
      for (uint8 u8Index = 0; u8Index < TT__stObjPatch.u8PatchCount; u8Index++)
      {
         if (TT__boIsPatchTarget(pvObject, enSpecificType, &TT__stObjPatch.pstPatches[u8Index]))
         {
            TT__vApplyPatch(pvObject, enSpecificType, &TT__stObjPatch.pstPatches[u8Index]);
         }
      }
   }*/
}
