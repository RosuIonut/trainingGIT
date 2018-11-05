#ifndef COMMANDINTERPRETER_HPP
#define COMMANDINTERPRETER_HPP

#include <string>
#include <vector>

#include "lib-mrts.hpp"
#include "ShMemManager.hpp"

extern "C"
{
#include "SHMEM_Access.h"
}

//class SequencerSection;
//class TraceSection;
//class ObjPatchSection;


// The command interpreter is the "main loop" of the program. It executes
// commands that can be specifies as macro file (script) or interactive by
// the user. 

class CommandInterpreter
{
public:

   CommandInterpreter::CommandInterpreter() :
      m_boInteractive(false), m_enTraceMode(nenTraceOff), m_enEvalMode(nenEvalShort)
   {
   }

   virtual CommandInterpreter::~CommandInterpreter()
   {
   }

   void vSetStartupScript(const std::string &sFile)
   {
      m_sStartupScript = sFile;
   }

   void vSetInteractive(bool boInteractive)
   {
      m_boInteractive = boInteractive;
   }

   void vSetVersionString(const std::string &sVersion)
   {
      m_sVersionString = sVersion;
   }

   bool boRunSession(void);

private:

   typedef enum {
      nenTraceOff,
      nenTraceConsole,
      nenTraceFile,
   } tenTraceMode;

   typedef enum {
      nenEvalOff,
      nenEvalShort,
      nenEvalFull,
   } tenEvalMode;

   void vSetQuitRequested(bool boQuitRequest)
   {
      m_boQuitRequested = boQuitRequest;
   }

   bool boIsQuitRequested(void)
   {
      return m_boQuitRequested;
   }

   // Executes the commands found in the specified file
   bool boRunScript(const std::string &sFile);
   // Runs an interactive loop that queries the user
   bool boRunInteractiveLoop(void);
   // Executes a command line
   bool boExecute(const std::string &sLine);

   // Functions to parse command lines
   bool boIsEmpty(const std::string &sLine);
   bool boIsComment(const std::string &sLine);
   void vSplitLine(const std::string &line, std::vector<std::string> *tokens);
   mrts::test *pstFindTest(mrts::document &oDoc, const std::string &sId);
   mrts::test_case *pstFindTestCase(mrts::document &oDoc, const std::string &sId);

   // Print help
   void vPrintCommandSummary(void);
   // File handling
   bool boLoadTestSpec(const std::string &sPath);
   bool boSaveTestSpec(const std::string &sPath);
   // Listing of test spec items
   void vListTest(const mrts::test &stTest);
   void vShowTestCase(const mrts::test_case &stTestCase);
   // Running of test spec items
   bool boRunTest(mrts::test &stTest);
   bool boRunTestCase(mrts::test_case &stTestCase);

   // Command handler
   bool boCmdLoad(const std::vector<std::string> &args);
   bool boCmdReload(const std::vector<std::string> &args);
   bool boCmdSave(const std::vector<std::string> &args);
   bool boCmdList(const std::vector<std::string> &args);
   bool boCmdShow(const std::vector<std::string> &args);
   bool boCmdRun(const std::vector<std::string> &args);
   bool boCmdEval(const std::vector<std::string> &args);
   bool boCmdAutoTrace(const std::vector<std::string> &args);
   bool boCmdAutoEval(const std::vector<std::string> &args);
   bool boCmdSource(const std::vector<std::string> &args);
   bool boCmdTiming(const std::vector<std::string> &args);

   bool boWriteTrace(const mrts::test_case &stTestCase);
   bool boEvalTestCase(const mrts::test_case &stTestCase, tenEvalMode enMode);

   bool m_boInteractive;
   std::string m_sStartupScript;
   ShMemManager m_oShMemManager;
   bool m_boQuitRequested;

   tenTraceMode m_enTraceMode;
   std::string m_sTraceFile;

   tenEvalMode m_enEvalMode;

   std::string m_sCurrTestSpecPath;
   mrts::document m_oCurrTestSpec;
   std::string m_sVersionString;
};

#endif
