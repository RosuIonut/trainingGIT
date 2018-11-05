#ifndef TRACECONFIG_HPP
#define TRACECONFIG_HPP

extern "C"
{
#include "cdef.h"
#include "SHMEM_Layout.h"
}

#include <string>
#include <vector>

#include "TraceSection.hpp"

typedef bool (*tpfFilterFactory)(const std::vector<std::string> &args, std::vector<SHMEM_tstTraceFilterEntry> *filter);
typedef void (*tpfArgsTranslator)(const uint32 *pu32Args, uint8 u8ArgCount, TraceSection::txLogEntry *xLogEntry);
typedef void (*tpfBlockTranslator)(ByteBuffer &oData, TraceSection::txLogEntry *xLogEntry);

namespace TraceConfig
{

typedef struct {
   uint16 u16ItemId;                     // Trace item ID
   const char *sItemName;                // Name of item
   uint8 u8Type;                         // Type (arg event, block event, arg data)
   uint8 u8Mode;                         // Mode (type specific)
   uint8 u8ArgCount;                     // Number of arguments
   tpfFilterFactory pfFilterFactory;     // Filter factory callback
   tpfArgsTranslator pfArgsTrans;        // Translator for arguments type items
   tpfBlockTranslator pfBlockTrans;      // Translator for block type items
} tstItemAttr;

typedef struct {
    const char  *sItemName;              // Name if signal
    uint32      u32SignalId;             // Signal Id's
} tstEventDef;

// Find item configuration for a given item ID or item name
const tstItemAttr *pstGetItemAttr(const std::string &sItemName);
const tstItemAttr *pstGetItemAttr(uint16 u16ItemId);

void vTranslateEntryArgs(const SHMEM_tstTraceEntryArgs &stEntry, TraceSection::txLogEntry *xLogEntry);
void vTranslateEntryBlock(const SHMEM_tstTraceEntryBlock &stEntry, TraceSection::txLogEntry *xLogEntry);

}

#endif
