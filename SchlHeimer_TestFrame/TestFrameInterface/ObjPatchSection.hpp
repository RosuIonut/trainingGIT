#ifndef OBJPATCHSECTION_HPP
#define OBJPATCHSECTION_HPP

#include <vector>

extern "C"
{
#include "SHMEM_Layout.h"
}

#include "ShMemSection.hpp"

class ObjPatchSection : public ShMemSection
{
public:
   ObjPatchSection() : ShMemSection(SHMEM_nSectObjectPatches)
   {
   }

   virtual ~ObjPatchSection()
   {
   }

   virtual void vSerialize(ByteBuffer *poBuffer) const;
   virtual bool boDeserialize(ByteBuffer *poBuffer);

   bool boCreateFromText(const std::vector<std::string> &text);

private:
   bool boAddPropPatch(const std::vector<std::string> &args);

   int iObjTypeNameToCode(const std::string &sName);
   int iObjPropNameToCode(const std::string &sName);
   bool boObjPropValueToCode(const std::string &sProp, const std::string &sName, uint32 *pu32Value);

   std::vector<SHMEM_tstObjPatchSpec> m_oPatches;
};

#endif
