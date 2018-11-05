#include <algorithm>
#include <lib-util.hpp>

#include "TraceConfig.hpp"
#include "TraceSection.hpp"

const char*     TraceSection::m_cpTargetModuleNames[]   = {"Legacy", "HMI"};
const size_t    TraceSection::m_csNumTargetmoduleNames  = 2;

void TraceSection::vSerialize(ByteBuffer *poBuffer) const
{
   // 1. Create header
   SHMEM_tstTraceHeader stHeader;
   stHeader.u16FilterEntries = 0;
   stHeader.u32BytesMax = m_u32TraceSize;
   stHeader.u32BytesUsed = 0;
   stHeader.u8Flags = 0;

   // 2. Transfer item configuration into header
   std::vector<SHMEM_tstTraceFilterEntry> filter;
   for (int i = 0; i < SHMEM_nenTraceItemCount; i++)
   {
      // Is the entry used?
      if (m_astItemConfig[i].boValid)
      {
         // Yes, use its content
         stHeader.astItemCfg[i].u8Type = m_astItemConfig[i].u8Type;
         stHeader.astItemCfg[i].u8Mode = m_astItemConfig[i].u8Mode;
         stHeader.astItemCfg[i].u8ArgCount = m_astItemConfig[i].u8ArgCount;

         // Needs a filter to be created?
         if (m_astItemConfig[i].filter.size() > 0)
         {
            // Store current index as start index
            stHeader.astItemCfg[i].u16FilterIndex = (uint16) filter.size();
            // Add elements into filter array
            for (int j = 0; j < (int) m_astItemConfig[i].filter.size(); j++)
               filter.push_back(m_astItemConfig[i].filter[j]);
         }
         else
         {
            // Set index to "no filter"
            stHeader.astItemCfg[i].u16FilterIndex = SHMEM_nTraceNoFilter;
         }
      }
      else
      {
         // No, set header fields to sane values
         stHeader.astItemCfg[i].u8Type = 0;
         stHeader.astItemCfg[i].u8Mode = 0;
         stHeader.astItemCfg[i].u8ArgCount = 0;
         stHeader.astItemCfg[i].u16FilterIndex = SHMEM_nTraceNoFilter;
      }
   }

   // 3. Adjust size of filter section
   stHeader.u16FilterEntries = (uint16) filter.size();

   // 4. Pack it all together
   poBuffer->vAppendItem(&stHeader, sizeof(stHeader));
   for (int i = 0; i < (int) filter.size(); i++)
      poBuffer->vAppendItem(&filter[i], sizeof(filter[i]));
   // reserve space for trace buffer
   for (int i = 0; i < (int) m_u32TraceSize; i++)
   {
      uint8 u8EmptyPattern = 0xee;
      poBuffer->vAppendItem(&u8EmptyPattern, 1);
   }
}

bool TraceSection::boDeserializeItem(ByteBuffer *poBuffer, uint32 *pu32BytesRead)
{
   bool boFailed = false;

   // First peek at item to find out its ID and thus its type
   SHMEM_tstTraceEntryHeader stEntryHeader;
   if (poBuffer->boPeekNextItem(&stEntryHeader, sizeof(stEntryHeader)))
   {
      uint32 u32ItemSize = sizeof(SHMEM_tstTraceEntryHeader) + stEntryHeader.u16ArgSize;
      SHMEM_tstTraceEntryArgs stEntryArgs;
      SHMEM_tstTraceEntryBlock stEntryBlock;

      // Now read it for real
      switch (m_astItemConfig[stEntryHeader.u16Id].u8Type)
      {
      case SHMEM_nTraceItemTypeEvent:
      case SHMEM_nTraceItemTypeData:
         if (poBuffer->boGetNextItem(&stEntryArgs, u32ItemSize))
         {
            *pu32BytesRead = u32ItemSize;
            txLogEntry LogEntry;
            TraceConfig::vTranslateEntryArgs(stEntryArgs, &LogEntry);
            m_TraceLog.push_back(LogEntry);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading trace entry %i", stEntryHeader.u16Id);
            boFailed = true;
         }
         break;
      case SHMEM_nTraceItemTypeBlock:
         if (poBuffer->boGetNextItem(&stEntryBlock, u32ItemSize))
         {
            *pu32BytesRead = u32ItemSize;
            txLogEntry LogEntry;
            TraceConfig::vTranslateEntryBlock(stEntryBlock, &LogEntry);
            m_TraceLog.push_back(LogEntry);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading trace entry %i", stEntryHeader.u16Id);
            boFailed = true;
         }
         break;
      default:
         util::Log::vPrint(util::LOG_WARNING, "Found unknown trace item type, skipping data.");
         poBuffer->vForward(u32ItemSize);
         *pu32BytesRead = u32ItemSize;
         break;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Error reading trace entry");
      boFailed = true;
   }

   return !boFailed;
}

bool TraceSection::boDeserialize(ByteBuffer *poBuffer)
{
   bool boFailed = false;
   // During deserialization, the item configuration is ignored
   // because the target is not allowed to change it anyway

   // 1. Get header
   SHMEM_tstTraceHeader stHeader;
   if (poBuffer->boGetNextItem(&stHeader, sizeof(stHeader)))
   {
      // Check for the buffer full flag
      if (stHeader.u8Flags & SHMEM_nTraceFlagBufferFull)
         util::Log::vPrint(util::LOG_WARNING, "Trace buffer was too small! Maybe important data is missing!");
      // Discard filter config
      poBuffer->vForward(stHeader.u16FilterEntries * sizeof(SHMEM_tstTraceFilterEntry));
      // Get the trace data and convert it into readable text
      uint32 u32BytesRead = 0;
      while (u32BytesRead < stHeader.u32BytesUsed && !boFailed)
      {
         uint32 u32ItemSize;
         if (boDeserializeItem(poBuffer, &u32ItemSize))
         {
            util::Log::vPrint(util::LOG_DEBUG, "Deserialized %lu bytes of trace data.", u32ItemSize);
            u32BytesRead += u32ItemSize;
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Error reading trace entry from log buffer!");
            boFailed = true;
         }
      }
      // Skip the trailing empty pattern from initial section fill
      poBuffer->vForward(stHeader.u32BytesMax - stHeader.u32BytesUsed);
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Error reading trace header!");
      boFailed = true;
   }

   return !boFailed;
}

bool TraceSection::boCreateFromText(const std::vector<std::string> &text)
{
   bool boFailed = false;

   std::vector<std::string>::const_iterator it;
   for (it = text.begin(); !boFailed && it != text.end(); it++)
   {
      std::string sCommand;
      std::vector<std::string> args;
      if (util::boParseCommandLine(*it, &sCommand, &args))
      {         
         bool boSuccess = false; 
         std::string sLcCommand = util::sMakeLower(sCommand);

         if (sLcCommand.compare("happened") == 0)
            boSuccess = boConfigureItem(args);
         else if (sLcCommand.compare("nothappened") == 0)
            boSuccess = boConfigureItem(args);
         else if (sLcCommand.compare("changed") == 0)
            boSuccess = boConfigureItem(args);
         else if (sLcCommand.compare("notchanged") == 0)
             boSuccess = boConfigureItem(args);
         else if (sLcCommand.compare("#") == 0)     //20170307 wti
             boSuccess = true;                      //20170302 wti

         if (!boSuccess)
            boFailed = true;
      }
      else
      {
         util::Log::vPrint(util::LOG_WARNING, "Skipping bad line \"%s\"", it->c_str());
      }
   }

   return !boFailed;
}

bool TraceSection::boConfigureItem(const std::vector<std::string> &args)
{
   bool boFailed = false;

   if (args.size() >= 1)
   {
      // Get static configuration of this item
      const TraceConfig::tstItemAttr *pstAttr = TraceConfig::pstGetItemAttr(args[0]);
      if (pstAttr != NULL)
      {
         // Get a pointer to its runtime configuration
         tstItemConfig *pstConfig = &m_astItemConfig[pstAttr->u16ItemId];

         // Check if item needs configuration
         if (!pstConfig->boValid)
         {
            // Enable this item by taking over mode, type and argument count
            pstConfig->u8Mode = pstAttr->u8Mode;
            pstConfig->u8Type = pstAttr->u8Type;
            pstConfig->u8ArgCount = pstAttr->u8ArgCount;
            // Flag this config as valid
            pstConfig->boValid = true;
         }

         // Check if it is a item that has filtering enabled
         if (pstAttr->pfFilterFactory)
         {
            std::vector<SHMEM_tstTraceFilterEntry> filter;
            bool boAddFilter = false;

            // Check if there are arguments for creating a new filter
            if (args.size() > 1)
            {
               if (pstAttr->pfFilterFactory(args, &filter))
               {
                  // Creating filter was succesful
                  boAddFilter = true;
               }
               else
               {
                  util::Log::vPrint(util::LOG_ERROR, "Creating filter for %s failed!", pstAttr->sItemName);
                  boFailed = true;
               }
            }
            else
            {
               // We found an expectation with no further arguments. Since this is a item
               // that supports filtering, we need to add a "true" to the filter chain in
               // order to have all instances traced.
               SHMEM_tstTraceFilterEntry stTrue;
               stTrue.u8ArgIndex = 0;
               stTrue.u32ArgValue = 0;
               stTrue.u8Flags = SHMEM_nTraceFlagAlwaysTrue | SHMEM_nTraceFlagEndProduct;
               filter.push_back(stTrue);
               boAddFilter = true;
            }

            if (boAddFilter)
            {
               // Check if filter needs to be added
               if (!boFilterPresent(pstConfig->filter, filter))
               {
                  // To add the filter, remove the old "EndSum" flag
                  if (pstConfig->filter.size() > 0)
                     pstConfig->filter.back().u8Flags &= ~SHMEM_nTraceFlagEndSum;
                  // Now append the new filter
                  pstConfig->filter.insert(pstConfig->filter.end(), filter.begin(), filter.end());
                  // Set the "EndSum" flag at the end of the new filter
                  pstConfig->filter.back().u8Flags |= SHMEM_nTraceFlagEndSum;
               }
               else
               {
                  // Very same filter already exists, nothing to do
               }
            }
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Invalid trace item: %s", args[0].c_str());
         boFailed = true;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Invalid syntax.");
      boFailed = true;
   }

   return !boFailed;
}

void TraceSection::vGetTraceLog(std::vector<std::string> *buffer)
{
   buffer->clear();

   std::vector<txLogEntry>::const_iterator entry;
   for (entry = m_TraceLog.begin(); entry != m_TraceLog.end(); entry++)
   {
      txLogEntry::const_iterator param;
      std::string sLine;
      bool boPrev = false;
      for (param = entry->begin(); param != entry->end(); param++)
      {
         if (boPrev)
            sLine += ", ";
         sLine += param->sName;
         sLine += " = ";
         sLine += param->sValue;
         boPrev = true;
      }
      //wti 20170117: Adding Timestamp at end of line
      // Adding at end of line is necessary to allow the evaluation without code change
      sLine += ", Timestamp = ";        //wti
      sLine += entry->begin()->sTime;   //wti
      sLine += " us";                   //wti
      //wti: ---
      buffer->push_back(sLine);
   }
}

bool TraceSection::boFilterPresent(const std::vector<SHMEM_tstTraceFilterEntry> &filter, const std::vector<SHMEM_tstTraceFilterEntry> &subfilter)
{
   bool boFound = false;

   // iterate through complete filter
   std::vector<SHMEM_tstTraceFilterEntry>::const_iterator it = filter.begin();
   while (it != filter.end() && !boFound)
   {
      // find all sub filters
      std::vector<SHMEM_tstTraceFilterEntry> currSub;
      bool boEndProduct;
      do
      {
         currSub.push_back(*it);
         boEndProduct = (it->u8Flags & SHMEM_nTraceFlagEndProduct) != 0;
         it++;
      }
      while (!boEndProduct);

      // compare current sub filter with searched sub filter
      if (currSub.size() == subfilter.size())
      {
         bool boMatch = true;
         for (int i = 0; boMatch && i < (int) currSub.size(); i++)
         {
            bool boIndexMatch = currSub[i].u8ArgIndex == subfilter[i].u8ArgIndex;
            bool boValueMatch = currSub[i].u32ArgValue == subfilter[i].u32ArgValue;
            const uint8 u8Mask = ~(SHMEM_nTraceFlagEndProduct | SHMEM_nTraceFlagEndSum);
            bool boFlagsMatch = (currSub[i].u8Flags & u8Mask) == (subfilter[i].u8Flags & u8Mask);
            boMatch = boMatch && boIndexMatch && boValueMatch && boFlagsMatch;
         }
         if (boMatch)
            boFound = true;
      }
   }

   return boFound;
}

