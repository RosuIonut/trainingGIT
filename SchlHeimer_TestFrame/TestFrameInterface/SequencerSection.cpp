#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <lib-util.hpp>

#include "TT_ExecFunctions.hpp"
// #include "lib-names.hpp"
#include "SequencerSection.hpp"

int SequencerSection::iKeyNameToCode(const std::string &sName)
{
   std::string sLcName = util::sMakeLower(sName);

   if (sLcName.compare("mfl_next") == 0)
      return SHMEM_nSeqKey_MFL_NEXT;
   if (sLcName.compare("mfl_prev") == 0)
      return SHMEM_nSeqKey_MFL_PREV;
   if (sLcName.compare("mfl_up") == 0)
      return SHMEM_nSeqKey_MFL_UP;
   if (sLcName.compare("mfl_down") == 0)
      return SHMEM_nSeqKey_MFL_DOWN;
   if (sLcName.compare("mfl_ok") == 0)
      return SHMEM_nSeqKey_MFL_OK;
   if (sLcName.compare("mfl_menu") == 0)
      return SHMEM_nSeqKey_MFL_MENU;
   if (sLcName.compare("mfl_fas") == 0)
      return SHMEM_nSeqKey_MFL_FAS;

   return -1;
}

// 20170131 wti
bool SequencerSection::boAddStartSync(std::vector<SHMEM_tstSeqCommand> &seq)
{
   SHMEM_tstSeqCommand cmd;

   cmd.u8Processed  = 0;
   cmd.u8ActionCode = SHMEM_nSeqCodeStartSync;
   cmd.u16Argument  = 0;
   cmd.u16Argument2 = 0;
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boAddKeyPress(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeKeyPress;

   int iKeyCode = iKeyNameToCode(args[0]);
   if (iKeyCode == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown key: %s", args[0].c_str());
      return false;
   }

   cmd.u16Argument2 = 0;
   cmd.u16Argument = (uint16)iKeyCode;
   cmd.u8Processed = 0; //20170118 wti
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boAddKeyRelease(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeKeyRelease;

   int iKeyCode = iKeyNameToCode(args[0]);
   if (iKeyCode == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown key: %s", args[0].c_str());
      return false;
   }

   cmd.u16Argument2 = 0;
   cmd.u16Argument = (uint16)iKeyCode;
   cmd.u8Processed = 0; //20170118 wti
   seq.push_back(cmd);
   return true;
}

bool boIsHexValue(const std::string &sName)
{
    if (sName.size() < 2)
        return false;
    return (sName.compare(0, 2, "0x") == 0 || sName.compare(0, 2, "0X") == 0);
}

int SequencerSection::iStrToInt(const std::string &sName)
{
    int retval = 0;
    const char *my_str = sName.c_str();
    if (boIsHexValue(sName))
        retval = strtol(my_str, NULL, 16);
    else
        retval = atoi(my_str);

  return retval;
}

bool SequencerSection::boAddWarnRequest(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeWarnRequest;

   int iWarnId = iStrToInt(args[0]);
   if (iWarnId == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown warning: %s", args[0].c_str());
      return false;
   }

   cmd.u16Argument2 = 0;
   cmd.u16Argument = (uint16)iWarnId;
   cmd.u8Processed = 0; //20170118 wti
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boAddWarnRelease(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeWarnRelease;

   int iWarnId = iStrToInt(args[0]);
   if (iWarnId == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Unknown warning: %s", args[0].c_str());
      return false;
   }

   cmd.u16Argument2 = 0;
   cmd.u16Argument = (uint16)iWarnId;
   cmd.u8Processed = 0; //20170118 wti
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boAddHmiWarn(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
    SHMEM_tstSeqCommand cmd;
    cmd.u8ActionCode = SHMEM_nSeqCodeHmiWarn;

    int iWarnId = iStrToInt(args[0]);
    if (iWarnId == -1)
    {
        util::Log::vPrint(util::LOG_ERROR, "Unknown warning: %s", args[0].c_str());
        return false;
    }

    cmd.u16Argument2 = 0;
    cmd.u16Argument = (uint16)iWarnId;
    cmd.u8Processed = 0; //20170118 wti
    seq.push_back(cmd);
    return true;
}

bool SequencerSection::boSCommandWoArg(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args, uint8 a_SeqCode)
{
    SHMEM_tstSeqCommand cmd;
    cmd.u8ActionCode = a_SeqCode;

    cmd.u16Argument2 = 0;
    cmd.u16Argument = 0;
    cmd.u8Processed = 0; //20170118 wti
    seq.push_back(cmd);
    return true;
}

bool SequencerSection::boSCommandWithIntArg(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args, uint8 a_SeqCode)
{
    SHMEM_tstSeqCommand cmd;
    cmd.u8ActionCode = a_SeqCode;

    int iItem = iStrToInt(args[0]);
    if (iItem == -1)
    {
        util::Log::vPrint(util::LOG_ERROR, "Unknown warning: %s", args[0].c_str());
        return false;
    }

    cmd.u16Argument2 = 0;
    cmd.u16Argument = (uint16)iItem;
    cmd.u8Processed = 0; //20170118 wti
    seq.push_back(cmd);
    return true;
}

bool SequencerSection::boAddHmiWarnOff(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
    SHMEM_tstSeqCommand cmd;
    cmd.u8ActionCode = SHMEM_nSeqCodeHmiWarnOff;

    int iWarnId = iStrToInt(args[0]);
    if (iWarnId == -1)
    {
        util::Log::vPrint(util::LOG_ERROR, "Unknown warning: %s", args[0].c_str());
        return false;
    }

    cmd.u16Argument2 = 0;
    cmd.u16Argument = (uint16)iWarnId;
    cmd.u8Processed = 0; //20170118 wti
    seq.push_back(cmd);
    return true;
}


bool SequencerSection::boAddSleep(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeSleep;

   int iTimeout = atoi(args[0].c_str());
   if (iTimeout == 0 || iTimeout > 0xFFFF)
   {
      util::Log::vPrint(util::LOG_ERROR, "Invalid timeout");
      return false;
   }

   cmd.u16Argument2 = 0;
   cmd.u16Argument = (uint16)iTimeout;
   cmd.u8Processed = 0; //20170118 wti
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boAddExecFunction(std::vector<SHMEM_tstSeqCommand> &seq, const std::vector<std::string> &args)
{
   SHMEM_tstSeqCommand cmd;
   cmd.u8ActionCode = SHMEM_nSeqCodeExecFunction;
   
   int iFuncCode = ExecFunctions::iFuncNameToCode( args[0] );
   if (iFuncCode == -1)
   {
      util::Log::vPrint(util::LOG_ERROR, "Invalid function name: %s", args[0].c_str());
      return false;
   }
   // 20170306 wti - Added 2nd Argument and used it as parameter for ExecFunction
   int iExecParam = 0;
   if ( args.size() > 1 )
   {
     iExecParam = ExecFunctions::iFuncParamToCode( args[1] );
     if (-1 == iExecParam)
     {
       iExecParam = atoi( args[1].c_str() );
       if ( -1 == iExecParam )
       {
           util::Log::vPrint(util::LOG_ERROR, "Invalid parameter %s for function %s", args[1].c_str(), args[0].c_str());
           return false;
       }
     }
   }

   cmd.u8Processed  = 0;                    // 20170118 wti
   cmd.u16Argument2 = (uint16)iExecParam;   // 20170306 wti
   cmd.u16Argument  = (uint16)iFuncCode;
   seq.push_back(cmd);
   return true;
}

bool SequencerSection::boCreateFromText(const std::vector<std::string> &text)
{
   std::vector<SHMEM_tstSeqCommand>         xRepeatBlock;
   std::vector<std::string>::const_iterator it;
   uint8                                    u8RepeatCount   = 0;
   bool                                     boFailed        = false;

   boAddStartSync(m_Commands); // 20170131 wti
   
   for (it = text.begin(); !boFailed && it != text.end(); it++)
   {
      std::string sCommand;
      std::vector<std::string> args;
      if (util::boParseCommandLine(*it, &sCommand, &args))
      {
         std::string sLcCommand = util::sMakeLower(sCommand);
         if (sLcCommand.compare("precondition") == 0)
         {
            util::Log::vPrint(util::LOG_INFO, "Preconditions are not supported yet...");
         }

         if (sLcCommand.compare("repeat") == 0)
         {
            int iCount = atoi(args[0].c_str());
            if (iCount == 0 || iCount > 0xFF)
            {
               util::Log::vPrint(util::LOG_ERROR, "Invalid repeat count!");
               boFailed = true;
            }
            else
            {
               u8RepeatCount = (uint8) iCount;
               xRepeatBlock.clear();
            }
         }
         else if (sLcCommand.compare("end") == 0)
         {
            if (util::sMakeLower(args[0]).compare("repeat") == 0)
            {
               // Insert repeat block n times
               while (u8RepeatCount > 0)
               {
                  m_Commands.insert(m_Commands.end(), xRepeatBlock.begin(), xRepeatBlock.end());
                  u8RepeatCount--;
               }
            }
            else
            {
               util::Log::vPrint(util::LOG_ERROR, "Invalid end statement!");
               boFailed = true;
            }
         }
         else if (sLcCommand.compare("keypress") == 0)
            boFailed = !boAddKeyPress(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("keyrelease") == 0)
            boFailed = !boAddKeyRelease(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("warnrequest") == 0)
            boFailed = !boAddWarnRequest(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("warnrelease") == 0)
            boFailed = !boAddWarnRelease(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("hmiwarn") == 0)
             boFailed = !boAddHmiWarn(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("hmiwarnoff") == 0)
             boFailed = !boAddHmiWarnOff(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("sleep") == 0)
            boFailed = !boAddSleep(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("execfunction") == 0)
            boFailed = !boAddExecFunction(u8RepeatCount ? xRepeatBlock : m_Commands, args);
         else if (sLcCommand.compare("#") == 0)
            { /* comment line */ }
         else
         {
             util::Log::vPrint(util::LOG_WARNING, "Skipping bad line \"%s\"", it->c_str());
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_WARNING, "Skipping bad line \"%s\"", it->c_str());
      }
   }

   return !boFailed;
}

void SequencerSection::vSerialize(ByteBuffer *poBuffer) const
{
   SHMEM_tstSeqHeader stHeader;
   stHeader.u16CmdCount = (uint16) m_Commands.size();
   poBuffer->vAppendItem(&stHeader, sizeof(stHeader));

   std::vector<SHMEM_tstSeqCommand>::const_iterator it;
   for (it = m_Commands.begin(); it != m_Commands.end(); it++)
      poBuffer->vAppendItem(&*it, sizeof(*it));
}

bool SequencerSection::boDeserialize(ByteBuffer *poBuffer)
{
   // Is this ever needed?
   assert(0);
   return false;
}
