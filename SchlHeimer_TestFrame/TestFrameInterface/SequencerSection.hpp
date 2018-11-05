#ifndef SEQUENCERSECTION_HPP
#define SEQUENCERSECTION_HPP

#include <vector>

extern "C"
{
#include "SHMEM_Layout.h"
}

#include "ShMemSection.hpp"

class SequencerSection : public ShMemSection
{
public:
   SequencerSection() : ShMemSection(SHMEM_nSectSequencer)
   {
   }

   virtual ~SequencerSection()
   {
   }

   virtual void vSerialize(ByteBuffer *poBuffer) const;
   virtual bool boDeserialize(ByteBuffer *poBuffer);

   bool boCreateFromText(const std::vector<std::string> &text);

private:
   bool boAddStartSync(std::vector<SHMEM_tstSeqCommand> &seq);
   bool boAddKeyPress(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddKeyRelease(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddWarnRequest(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddWarnRelease(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddSleep(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddExecFunction(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddHmiWarn(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boAddHmiWarnOff(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args);
   bool boSCommandWithIntArg(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args, uint8 a_SeqCode);
   bool boSCommandWoArg(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args, uint8 a_SeqCode);

   int iKeyNameToCode(const std::string &sName);
   int iStrToInt(const std::string &sName);

   std::vector<SHMEM_tstSeqCommand> m_Commands;
};

#endif
