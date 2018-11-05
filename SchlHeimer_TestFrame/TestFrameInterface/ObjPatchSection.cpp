#include <cassert>

#include <lib-util.hpp>
#include <lib-names.hpp>

#include "ObjPatchSection.hpp"

int ObjPatchSection::iObjTypeNameToCode(const std::string &sName)
{
   std::string sLcName = util::sMakeLower(sName);

   return -1;
}

int ObjPatchSection::iObjPropNameToCode(const std::string &sName)
{
   std::string sLcName = util::sMakeLower(sName);

   return -1;
}

bool ObjPatchSection::boObjPropValueToCode(const std::string &sProp, const std::string &sName, uint32 *pu32Value)
{
   bool boTranslated = false;

   return boTranslated;
}

bool ObjPatchSection::boAddPropPatch(const std::vector<std::string> &args)
{
   SHMEM_tstObjPatchSpec stSpec;
   char *pcEnd;

   // Type of object to patch
   int iObjType = iObjTypeNameToCode(args[0]);
   if (iObjType == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown object type: %s", args[0].c_str());
      return false;
   }

   std::string sFilterProperty;
   std::string sFilterValue;
   std::string sPatchProperty;
   std::string sPatchValue;

   if (args.size() > 3)
   {
      // Variant 1 with filter property and patch property
      sFilterProperty = args[1];
      sFilterValue = args[2];
      sPatchProperty = args[3];
      sPatchValue = args[4];
   }
   else
   {
      // Variant 2 with only patch property
      sPatchProperty = args[1];
      sPatchValue = args[2];
   }

   // ID of filter property
   int iFilterProperty = SHMEM_nenObjProp_Nothing;
   if (!sFilterProperty.empty())
   {
      int iFilterProperty = iObjPropNameToCode(sFilterProperty);
      if (iFilterProperty == -1)
      {
         util::Log::vPrint(util::LOG_ERROR, "Unknown property: %s", sFilterProperty.c_str());
         return false;
      }
   }

   // Value of filter property
   uint32 u32FilterValue = 0;
   if (!sFilterValue.empty())
   {
      if (boObjPropValueToCode(sFilterProperty, sFilterValue, &u32FilterValue) == false)
      {
         // Was not a name, try to translate it as nuermic value
         u32FilterValue = strtoul(sFilterValue.c_str(), &pcEnd, 0);
         if (*pcEnd != '\0')
         {
            // Not numeric data
            util::Log::vPrint(util::LOG_ERROR, "Filter value \"%s\" is not numeric!", sFilterValue.c_str());
            return false;
         }
      }
   }

   // ID of patch property
   int iPatchProperty = iObjPropNameToCode(sPatchProperty);
   if (iPatchProperty == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown property: %s", sPatchProperty.c_str());
      return false;
   }

   uint32 u32PatchValue = 0;
   if (boObjPropValueToCode(sPatchProperty, sPatchValue, &u32PatchValue) == false)
   {
      // Was not a name, try to translate it as nuermic value
      u32PatchValue = strtoul(sPatchValue.c_str(), &pcEnd, 0);
      if (*pcEnd != '\0')
      {
         // Not numeric data
         util::Log::vPrint(util::LOG_ERROR, "Filter value \"%s\" is not numeric!", sPatchValue.c_str());
         return false;
      }
   }

   stSpec.u8ObjType = iObjType;
   stSpec.u8FilterProperty = iFilterProperty;
   stSpec.u32FilterValue = u32FilterValue;
   stSpec.u8PatchProperty = iPatchProperty;
   stSpec.u32PatchValue = u32PatchValue;
   stSpec.u8ApplyCounter = 0;
   m_oPatches.push_back(stSpec);

   return true;
}

bool ObjPatchSection::boCreateFromText(const std::vector<std::string> &text)
{
   bool boFailed = false;
   std::vector<std::string>::const_iterator it;

   for (it = text.begin(); !boFailed && it != text.end(); it++)
   {
      std::string sCommand;
      std::vector<std::string> args;
      if (util::boParseCommandLine(*it, &sCommand, &args))
      {
         std::string sLcCommand = util::sMakeLower(sCommand);
         if (sLcCommand.compare("patchproperty") == 0)
            boFailed = !boAddPropPatch(args);
      }
      else
      {
         util::Log::vPrint(util::LOG_WARNING, "Skipping bad line \"%s\"", it->c_str());
      }

   }

   return !boFailed;
}

void ObjPatchSection::vSerialize(ByteBuffer *poBuffer) const
{
   SHMEM_tstObjPatchHeader stHeader;
   stHeader.u8PatchCount = (uint8) m_oPatches.size();
   poBuffer->vAppendItem(&stHeader, sizeof(stHeader));

   std::vector<SHMEM_tstObjPatchSpec>::const_iterator it;
   for (it = m_oPatches.begin(); it != m_oPatches.end(); it++)
      poBuffer->vAppendItem(&*it, sizeof(*it));
}

bool ObjPatchSection::boDeserialize(ByteBuffer *poBuffer)
{
   bool boFailed = false;

   // Get header
   SHMEM_tstObjPatchHeader stHeader;
   if (poBuffer->boGetNextItem(&stHeader, sizeof(stHeader)))
   {
      // Loop over all entries and check their apply count
      for (int i = 0; i < stHeader.u8PatchCount; i++)
      {
         SHMEM_tstObjPatchSpec stSpec;
         if (poBuffer->boGetNextItem(&stSpec, sizeof(stSpec)))
         {
            util::Log::vPrint(util::LOG_INFO, "Patch #1 was applied %i times.", stSpec.u8ApplyCounter);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading object patch specification!");
            boFailed = true;
         }
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Error reading object patch header!");
      boFailed = true;
   }

   return !boFailed;
}
