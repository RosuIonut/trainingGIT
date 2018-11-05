#ifndef TRACESECTION_HPP
#define TRACESECTION_HPP

#include <vector>

extern "C"
{
#include "SHMEM_Layout.h"
}

#include "ShMemSection.hpp"

#define DEF_TRACE_SIZE (512 * 1024)

class TraceSection : public ShMemSection
{
public:
   TraceSection() : ShMemSection(SHMEM_nSectTraceLog), m_u32TraceSize(DEF_TRACE_SIZE)
   {
      for (int i = 0; i < SHMEM_nenTraceItemCount; i++)
         m_astItemConfig[i].boValid = false;
   }

   virtual ~TraceSection()
   {
   }

   void vSetTraceSize(uint32 u32Size)
   {
      m_u32TraceSize = u32Size;
   }

   virtual void vSerialize(ByteBuffer *poBuffer) const;
   virtual bool boDeserialize(ByteBuffer *poBuffer);

   bool boCreateFromText(const std::vector<std::string> &text);
   void vGetTraceLog(std::vector<std::string> *buffer);

   typedef struct 
   {
      std::string sName;
      std::string sValue;
      std::string sTime; //wti
   } tstLogEntryParam;
   typedef std::vector<tstLogEntryParam> txLogEntry;


private:

   bool         boConfigureItem(const std::vector<std::string> &args);
   bool         boDeserializeItem(ByteBuffer *poBuffer, uint32 *pu32BytesRead);
   bool         boFilterPresent(const std::vector<SHMEM_tstTraceFilterEntry> &filter, const std::vector<SHMEM_tstTraceFilterEntry> &subfilter);
   //bool         boHandleTargetModule(const std::vector<std::string>& args,  const std::vector<std::string>::const_iterator currentLineIter);
   //TargetModule boIsTargetModuleArg(const std::string& arg);
   //bool         boValidateNumTargetModuleArgs(TargetModule enTragetModuleId, const std::vector<std::string>& args);

   std::vector<txLogEntry> m_TraceLog;

   typedef struct 
   {
      bool boValid;
      uint8 u8Type;
      uint8 u8Mode;
      uint8 u8ArgCount;
      std::vector<SHMEM_tstTraceFilterEntry> filter;
   } tstItemConfig;

   tstItemConfig                m_astItemConfig[SHMEM_nenTraceItemCount];
   uint32                       m_u32TraceSize;
   static const char*           m_cpTargetModuleNames[];
   static const size_t          m_csNumTargetmoduleNames;
};

#endif

