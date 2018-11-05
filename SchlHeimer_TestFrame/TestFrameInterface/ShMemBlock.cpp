extern "C"
{
#include "SHMEM_Layout.h"
}

#include <lib-util.hpp>
#include "ShMemBlock.hpp"

//default constructor
ShMemBlock::ShMemBlock()
{
}

//constructor which initialises the m_sections member with the sections listed in the parameter sections
ShMemBlock::ShMemBlock(std::vector<std::reference_wrapper<const ShMemSection>>& sections)
{
    //for each section, add it...
    for (std::vector<std::reference_wrapper<const ShMemSection>>::const_iterator it = sections.begin(); it != sections.end(); it++)
    {
        vAddSection(*it);
    }
}

//destructor
ShMemBlock::~ShMemBlock()
{
}

//add a section
void ShMemBlock::vAddSection(const ShMemSection &oSection)
{
   bool boAlreadyExists = false;

   // First check if a section with this ID already exists
   for (int i = 0; !boAlreadyExists && i < (int) m_sections.size(); i++)
   {
      if (m_sections[i].u8Type == oSection.u8GetType())
      {
         // Clear old data
         m_sections[i].oData.vClear();
         // Serialize section into internal data buffer
         oSection.vSerialize(&m_sections[i].oData);
         boAlreadyExists = true;
      }
   }

   if (!boAlreadyExists)
   {
      tstSection stSection;
      // Take over type
      stSection.u8Type = oSection.u8GetType();
      // Serialize section into internal data buffer
      oSection.vSerialize(&stSection.oData);
      // Insert new section
      m_sections.push_back(stSection);
   }
}

//get the information from the section in m_sections into the section poSection
bool ShMemBlock::boGetSection(ShMemSection *poSection)
{
   bool boResult = false;

   for (int i = 0; i < (int) m_sections.size(); i++)
   {
      if (m_sections[i].u8Type == poSection->u8GetType())
      {
         // Make sure that the section's read position is at the beginning
         // so that the complete section is deserialized
         m_sections[i].oData.vRewind();
         if (poSection->boDeserialize(&m_sections[i].oData) == false)
            util::Log::vPrint(util::LOG_WARNING, "Deserialization of section with type %i was not successful!", m_sections[i].u8Type);
         // If there is data left, the deserialization was not successful
         // (maybe a different format, or trailing garbage).
         boResult = m_sections[i].oData.boIsEmpty();
      }
   }

   return boResult;
}

//serialise the sections in m_sections into the buffer poBuffer for transfer to shared memory
void ShMemBlock::vSerialize(ByteBuffer *poBuffer)
{
   // Prepare header (will be finished when ObjSize is known)
   SHMEM_tstShMemHeader stHeader;
   stHeader.u8Version = SHMEM_nLayoutVersion;
   stHeader.u8SectCount = (uint8) m_sections.size();

   // Create descriptors and sections
   ByteBuffer oDescriptors;
   ByteBuffer oSections;
   uint32 u32Offset = (uint32) (sizeof(SHMEM_tstShMemHeader) + sizeof(SHMEM_tstSectDescr) * m_sections.size());
   for (int i = 0; i < (int) m_sections.size(); i++)
   {
      SHMEM_tstSectDescr stDescr;
      stDescr.u8Type = m_sections[i].u8Type;
      stDescr.u32Offset = u32Offset;
      stDescr.u32Length = m_sections[i].oData.u32GetTotalSize();
      oDescriptors.vAppendItem(&stDescr, sizeof(stDescr));
      // Make sure the complete data is added
      m_sections[i].oData.vRewind();
      oSections.vAppendItem(m_sections[i].oData);

      u32Offset += stDescr.u32Length;
   }

   // Finalize header and store parts
   stHeader.u32ObjSize = u32Offset;
   poBuffer->vAppendItem(&stHeader, sizeof(stHeader));
   poBuffer->vAppendItem(oDescriptors);
   poBuffer->vAppendItem(oSections);
}

//deserialise the sections from the buffer poBuffer into the m_sections member
bool ShMemBlock::boDeserialize(ByteBuffer *poBuffer)
{
   m_sections.clear();

   // Get header
   SHMEM_tstShMemHeader stHeader;
   poBuffer->boGetNextItem(&stHeader, sizeof(stHeader));

   // Get descriptors (for section type and length, the
   // offset is ignored because we read all sections)
   std::vector<SHMEM_tstSectDescr> descriptors;
   for (int i = 0; i < stHeader.u8SectCount; i++)
   {
      SHMEM_tstSectDescr stDescr;
      poBuffer->boGetNextItem(&stDescr, sizeof(stDescr));
      descriptors.push_back(stDescr);
   }

   // Get sections
   for (int i = 0; i < (int) descriptors.size(); i++)
   {
      tstSection stSection;
      stSection.u8Type = descriptors[i].u8Type;
      poBuffer->boGetNextItem(&stSection.oData, descriptors[i].u32Length);
      m_sections.push_back(stSection);
   }

   // There should not be any data left
   return poBuffer->boIsEmpty();
}
