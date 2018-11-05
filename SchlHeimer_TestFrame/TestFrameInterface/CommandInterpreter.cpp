#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cerrno>
#include <cctype>
#include <cstring>
#include <cassert>

#include <lib-eval.hpp>

#include <lib-util.hpp>

#include "CommandInterpreter.hpp"
#include "TimingSection.hpp"
#include "SequencerSection.hpp"
#include "TraceSection.hpp"
#include "ObjPatchSection.hpp"

#define MAX_LINE_LENGTH         1024
#define DEF_TIMING_CYCLE_COUNT  10

bool CommandInterpreter::boRunSession(void)
{
   bool boFailed = false;
   bool boContextValid = false;

   if (m_oShMemManager.boConnect())
   {
      util::Log::vPrint(util::LOG_INFO, "Connected to shared memory.");
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Error initializing shared memory context!");
      boFailed = true;
   }

   if (!boFailed && m_sStartupScript.size() > 0)
   {
      util::Log::vPrint(util::LOG_INFO, "Run startup script \"%s\".", m_sStartupScript.c_str());
      if (!boRunScript(m_sStartupScript))
      {
         util::Log::vPrint(util::LOG_ERROR, "Error running startup script!");
         boFailed = true;
      }
   }

   if (!boFailed && m_boInteractive)
   {
      util::Log::vPrint(util::LOG_INFO, "Enter interactive mode.");
      if (!boRunInteractiveLoop())
      {
         util::Log::vPrint(util::LOG_ERROR, "Error running interactive main loop!");
         boFailed = true;
      }
   }

   if (m_oShMemManager.boIsConnected())
   {
      m_oShMemManager.vDisconnect();
      util::Log::vPrint(util::LOG_INFO, "Disconnected from shared memory.");
   }

   return !boFailed;
}

bool CommandInterpreter::boRunScript(const std::string &sFile)
{
   bool boFailed = false;
   bool boDone = false;
   char line[MAX_LINE_LENGTH];
   FILE *script = fopen(sFile.c_str(), "r");
   int iLineNr = 0;

   if (!script)
   {
      util::Log::vPrint(util::LOG_ERROR, "Error opening script file \"%s\": %s",
         sFile.c_str(), strerror(errno));
      boFailed = true;
   }

   vSetQuitRequested(false);
   while (!boFailed && !boDone)
   {
      if (fgets(line, sizeof(line), script) != NULL)
      {
         iLineNr++;

         if (!boExecute(line))
         {
            util::Log::vPrint(util::LOG_ERROR, "Error in script \"%s\", line %i", sFile.c_str(), iLineNr);
            boFailed = true;
            continue;
         }

         if (boIsQuitRequested())
            boDone = true;
      }
      else
      {
         if (feof(script))
         {
            boDone = true;
            continue;
         }
         if (ferror(script))
         {
            util::Log::vPrint(util::LOG_ERROR, "Error while reading from \"%s\": %s",
               sFile.c_str(), strerror(errno));
            boFailed = true;
            continue;
         }
         util::Log::vPrint(util::LOG_ERROR, "Error while reading from \"%s\"", sFile.c_str());
         boFailed = true;
         continue;
      }
   }

   if (script)
   {
      fclose(script);
      script = NULL;
   }

   return !boFailed;
}

bool CommandInterpreter::boRunInteractiveLoop(void)
{
   bool boDone = false;
   char line[MAX_LINE_LENGTH];
   bool boFailed = false;

   vSetQuitRequested(false);
   
   do
   {
      printf("> ");
      fflush(stdout);
      if (fgets(line, sizeof(line), stdin) != NULL)
      {
         boExecute(line);

         if (boIsQuitRequested())
            boDone = true;
      }
      else
      {
         boFailed = true;
      }
   }
   while (!boFailed && !boDone);

   return !boFailed;
}

bool CommandInterpreter::boIsEmpty(const std::string &sLine)
{
   const char *p = sLine.c_str();
   while (*p && isspace(*p))
      p++;
   if (*p == '\0')
      return true;

   return false;
}

bool CommandInterpreter::boIsComment(const std::string &sLine)
{
   const char *p = sLine.c_str();
   while (*p && isspace(*p))
      p++;
   if (*p && (*p == '#'))
      return true;

   return false;
}

void CommandInterpreter::vSplitLine(const std::string &sLine, std::vector<std::string> *tokens)
{
   bool boInWord = false;
   std::string sWord;

   assert(tokens != NULL);

   for (std::string::const_iterator it = sLine.begin(); it != sLine.end(); it++)
   {
      if (boInWord)
      {
         // in word
         if (isspace((unsigned char) *it))
         {
            // found space => word is finished
            boInWord = false;
            tokens->push_back(sWord);
            sWord.clear();
         }
         else
         {
            // found char => add to word
            sWord.push_back(*it);
         }
      }
      else
      {
         // not in word
         if (isspace((unsigned char) *it))
         {
            // found space => stay out of word
         }
         else
         {
            // found char => now in word
            boInWord = true;
            sWord.push_back(*it);
         }
      }
   }
   // save last piece, too
   if (boInWord)
      tokens->push_back(sWord);
}

void CommandInterpreter::vPrintCommandSummary(void)
{
   printf("%s\n", m_sVersionString.c_str());
   printf("General commands:\n");
   printf("    help                  print help\n");
   printf("    quit                  quit program\n");
   printf("    auto_trace <mode>     set auto trace mode (off/console/file)\n");
   printf("    auto_eval <mode>      set auto evaluation mode (off/short/full)\n");
   printf("    source <file>         executes the specified script\n");
   printf("\n");
   printf("File handling:\n");
   printf("    load <file>           load test specification\n");
   printf("    reload                reload current test specification\n");
   printf("    save                  save current test specification\n");
   printf("    save <file>           save current test specification to new path\n");
   printf("\n");
   printf("Test handling:\n");
   printf("    list                  list all test cases\n");
   printf("    list <test ID>        list test cases for specified test\n");
   printf("    show <test case ID>   show specified test case\n");
   printf("    eval <test case ID>   evaluates specified test case\n");
   printf("    run                   run all test cases\n");
   printf("    run <test ID>         run test cases for specified test\n");
   printf("    run <test case ID>    run specified test case\n");
   printf("\n");
   printf("Specific test frame functions:\n");
   printf("    timing [cycles]       execute timing test\n");
}

bool CommandInterpreter::boLoadTestSpec(const std::string &path)
{
   bool boFailed = false;

   util::Log::vPrint(util::LOG_INFO, "Loading test spec from \"%s\".", path.c_str());
   mrts::fail_info fail;
   if (mrts::read(path, &m_oCurrTestSpec, &fail))
   {
      m_sCurrTestSpecPath = path;
      util::Log::vPrint(util::LOG_INFO, "\tFilename:        %s", m_oCurrTestSpec.filename.c_str());
      util::Log::vPrint(util::LOG_INFO, "\tRevision:        %s", m_oCurrTestSpec.revision.c_str());
      if (m_oCurrTestSpec.extracted.present)
         util::Log::vPrint(util::LOG_INFO, "\tExtracted:       %s", mrts::make_process_string(m_oCurrTestSpec.extracted).c_str());
      util::Log::vPrint(util::LOG_INFO, "\tNumber of tests: %i", m_oCurrTestSpec.tests.size());
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Loading test spec failed:");
      util::Log::vPrint(util::LOG_ERROR, "\treason = %s", mrts::fail_reason_to_string(fail.reason).c_str());
      util::Log::vPrint(util::LOG_ERROR, "\tline_no = %i", fail.line_no);
      if (fail.line_no > 0)
      {
         util::Log::vPrint(util::LOG_ERROR, "\tline_text = %s", fail.line_text.c_str());
         util::Log::vPrint(util::LOG_ERROR, "\tword = %s", fail.word.c_str());
      }
   }

   return !boFailed;
}

bool CommandInterpreter::boSaveTestSpec(const std::string &sPath)
{
   bool boFailed = false;

   std::string actor = util::get_login_name() + " (" + m_sVersionString + ")";
   m_oCurrTestSpec.annotated = mrts::make_process_step(actor);

   util::Log::vPrint(util::LOG_INFO, "Saving test specification to \"%s\"", sPath.c_str());
   if (!mrts::write(sPath, m_oCurrTestSpec))
   {
      util::Log::vPrint(util::LOG_ERROR, "Writing to \"%s\" failed!", sPath.c_str());
      boFailed = true;
   }

   return !boFailed;
}

void CommandInterpreter::vListTest(const mrts::test &stTest)
{
   printf("%s : %s\n", stTest.id.c_str(), stTest.name.c_str());
   std::vector<mrts::test_case>::const_iterator it;
   for (it = stTest.test_cases.begin(); it != stTest.test_cases.end(); it++)
      printf("    %s : %s\n", it->id.c_str(), it->name.c_str());
}

bool CommandInterpreter::boRunTest(mrts::test &stTest)
{
   bool boFailed = false;

   util::Log::vPrint(util::LOG_INFO, "Run test %s %s", stTest.id.c_str(), stTest.name.c_str());

   std::vector<mrts::test_case>::iterator it;
   for (it = stTest.test_cases.begin(); !boFailed && it != stTest.test_cases.end(); it++)
   {
      if (boRunTestCase(*it) == false)
         boFailed = true;
   }

   return !boFailed;
}

bool CommandInterpreter::boRunTestCase(mrts::test_case &stTestCase)
{
   bool boFailed = false;

   printf("\n");    // 20170307 wti
   util::Log::vPrint(util::LOG_INFO, "Run test case %s %s", stTestCase.id.c_str(), stTestCase.name.c_str());


   // Fill the sequencer, object patches (inputs) 
   // and trace (output) sections with the information in the MRTS scripts.
   //
   // 1. Create Sequencer section
   SequencerSection oSequencer;
   if (oSequencer.boCreateFromText(stTestCase.input) == false)
   {
      util::Log::vPrint(util::LOG_ERROR, "Could not translate \"input\" field (sequencer)!");
      boFailed = true;
   }

   // 2. Create ObjectPatch section
   ObjPatchSection oPatches;
   if (oPatches.boCreateFromText(stTestCase.input) == false)
   {
      util::Log::vPrint(util::LOG_ERROR, "Could not translate \"input\" field (object patches)!");
      boFailed = true;
   }

   // 3. Create Trace section
   TraceSection oTrace;
   if (oTrace.boCreateFromText(stTestCase.expected) == false)
   {
      util::Log::vPrint(util::LOG_ERROR, "Could not translate \"expected\" field!");
      boFailed = true;
   }
   
   // Create shared memory block and initialise with the sections: oSequencer, oPatches, oTrace
   //
   // 4. Insert sections into shared memory
   ShMemBlock oBlock;
   oBlock.vAddSection(oSequencer);
   oBlock.vAddSection(oPatches);
   oBlock.vAddSection(oTrace);
   
   // Send test requests to ST-Component for all clients 
   //
   if (!boFailed)
   {
      // 5. Pass data to client
      if (m_oShMemManager.boSendRequest(oBlock) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error sending request!");
         boFailed = true;
      }
   }

   // Get response from the clients (Legacy and HMI)
   //
   if (!boFailed)
   {
      // 6. Wait for response to be ready
      if (m_oShMemManager.boReceiveResponse(&oBlock) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error receiving request!");
         boFailed = true;
      }
   }
   
   // ??? Get test results from shared memory into the trace and object patch 
   //
   if (!boFailed)
   {
      // 7. Get result from response
      if (oBlock.boGetSection(&oTrace) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error reading Trace section!");
         boFailed = true;
      }
      if (oBlock.boGetSection(&oPatches) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error reading Patches section!");
         boFailed = true;
      }
   }

   if (!boFailed)
   {
      // 8. Store trace log in test specification's output column
      oTrace.vGetTraceLog(&stTestCase.output);
      // Check if it also needs to be written elsewhere
      if (boWriteTrace(stTestCase) == false)
         boFailed = true;
      // Check if auto evaluation is wanted
      if (m_enEvalMode != nenEvalOff)
      {
         // 20170307 wti: printf("\n");
         boEvalTestCase(stTestCase, m_enEvalMode);
      }
   }

   return !boFailed;
}

void CommandInterpreter::vShowTestCase(const mrts::test_case &stTestCase)
{
   std::vector<std::string>::const_iterator it;

   printf("Input:\n");
   for (it = stTestCase.input.begin(); it != stTestCase.input.end(); it++)
      printf("    %s\n", it->c_str());
   printf("Output:\n");
   for (it = stTestCase.output.begin(); it != stTestCase.output.end(); it++)
   {
       //printf("    %s\n", it->c_str());
       // wti: This is for display only. Originally the timestamp is at the end
       // of the string. Timestamp at end of line is necessary because otherwise 
       // evaluation would not work.
       std::string orgLine = it->c_str();
       std::string sTime;
       std::string sLine;
       sTime = orgLine.substr(orgLine.find("Timestamp = ")+12);
       sLine = orgLine.substr(0, orgLine.find(", Timestamp ="));
       printf("    %s : %s\n", sTime.c_str(), sLine.c_str());
   }
   printf("Expected:\n");
   for (it = stTestCase.expected.begin(); it != stTestCase.expected.end(); it++)
      printf("    %s\n", it->c_str());
}

bool CommandInterpreter::boCmdLoad(const std::vector<std::string> &args)
{
   if (args.size() >= 2)
   {
      return boLoadTestSpec(args[1]);
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "No file to load specified!");
      return false;
   }
}

bool CommandInterpreter::boCmdReload(const std::vector<std::string> &args)
{
   if (m_sCurrTestSpecPath.size() > 0)
   {
      return boLoadTestSpec(m_sCurrTestSpecPath);
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "There is no current test specification to reload!");
      return false;
   }
}

bool CommandInterpreter::boCmdSave(const std::vector<std::string> &args)
{
   if (m_sCurrTestSpecPath.length() > 0)
   {
      std::string sSavePath;
      sSavePath = (args.size() >= 2) ? args[1] : m_sCurrTestSpecPath;
      return boSaveTestSpec(sSavePath);
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Currently there is no test specification loaded!");
      return false;
   }
}

bool CommandInterpreter::boCmdList(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (m_sCurrTestSpecPath.length() > 0)
   {
      if (args.size() > 1)
      {
         std::string sId = args[1];
         mrts::test *pstTest = pstFindTest(m_oCurrTestSpec, sId);

         if (pstTest)
         {
            vListTest(*pstTest);
            boResult = true;
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "\"%s\" is not a valid test ID!", sId.c_str());
            boResult = false;
         }
      }
      else
      {
         // List all tests
         std::vector<mrts::test>::const_iterator it;
         for (it = m_oCurrTestSpec.tests.begin(); it != m_oCurrTestSpec.tests.end(); it++)
            vListTest(*it);
         boResult = true;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Currently there is no test specification loaded!");
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdShow(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (m_sCurrTestSpecPath.length() > 0)
   {
      if (args.size() > 1)
      {
         std::string sId = args[1];
         mrts::test_case *pstTestCase = pstFindTestCase(m_oCurrTestSpec, sId);

         if (pstTestCase)
         {
            vShowTestCase(*pstTestCase);
            boResult = true;
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "\"%s\" is not a valid test case ID!", sId.c_str());
            boResult = false;
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "No test case ID specified!");
         boResult = false;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Currently there is no test specification loaded!");
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdRun(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (m_sCurrTestSpecPath.length() > 0)
   {
      if (args.size() > 1)
      {
         std::string sId = args[1];
         mrts::test *pstTest = pstFindTest(m_oCurrTestSpec, sId);
         mrts::test_case *pstTestCase = pstFindTestCase(m_oCurrTestSpec, sId);

         if (pstTest)
         {
            if (boRunTest(*pstTest))
               boResult = true;
         }
         else if (pstTestCase)
         {
            if (boRunTestCase(*pstTestCase))
               boResult = true;
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "\"%s\" is neither a valid test ID nor a valid test case ID!", sId.c_str());
            boResult = false;
         }
      }
      else
      {
         // Run all tests
         bool boFailed = false;
         std::vector<mrts::test>::iterator it;
         for (it = m_oCurrTestSpec.tests.begin(); !boFailed && it != m_oCurrTestSpec.tests.end(); it++)
         {
            if (boRunTest(*it) == false)
               boFailed = true;
         }
         boResult = !boFailed;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Currently there is no test specification loaded!");
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdEval(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (m_sCurrTestSpecPath.length() > 0)
   {
      if (args.size() > 1)
      {
         std::string sId = args[1];
         mrts::test_case *pstTestCase = pstFindTestCase(m_oCurrTestSpec, sId);

         if (pstTestCase)
         {
            boResult = boEvalTestCase(*pstTestCase, nenEvalFull);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "\"%s\" is not a valid test case ID!", sId.c_str());
            boResult = false;
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "No test case ID specified!");
         boResult = false;
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "Currently there is no test specification loaded!");
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdAutoTrace(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (args.size() > 1)
   {
      if (!util::sMakeLower(args[1]).compare("off"))
      {
         m_enTraceMode = nenTraceOff;
         util::Log::vPrint(util::LOG_INFO, "Set auto_trace to 'off'");
         boResult = true;
      }
      else if (!util::sMakeLower(args[1]).compare("console"))
      {
         m_enTraceMode = nenTraceConsole;
         util::Log::vPrint(util::LOG_INFO, "Set auto_trace to 'console'");
         boResult = true;
      }
      else if (!util::sMakeLower(args[1]).compare("file"))
      {
         if (args.size() > 2)
         {
            m_enTraceMode = nenTraceFile;
            m_sTraceFile = args[2];
            util::Log::vPrint(util::LOG_INFO, "Set auto_trace to 'file' with path \"%s\"", m_sTraceFile.c_str());
            boResult = true;
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Missing path for auto_trace mode 'file'!");
            boResult = false;
         }
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Invalid mode argument!");
         boResult = false;
      }
   }
   else
   {
//wti      util::Log::vPrint(util::LOG_ERROR, "Mode argument is missing!");
       switch (m_enTraceMode)
       {
       case nenEvalOff:     util::Log::vPrint(util::LOG_INFO, "auto_trace is 'off'"); break;
       case nenEvalShort:   util::Log::vPrint(util::LOG_INFO, "auto_trace is 'console'"); break;
       case nenEvalFull:    util::Log::vPrint(util::LOG_INFO, "auto_trace is 'file' %s", m_sTraceFile.c_str()); break;
       } //wti
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdAutoEval(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (args.size() > 1)
   {
      if (!util::sMakeLower(args[1]).compare("off"))
      {
         m_enEvalMode = nenEvalOff;
         util::Log::vPrint(util::LOG_INFO, "Set auto_eval to 'off'");
         boResult = true;
      }
      else if (!util::sMakeLower(args[1]).compare("short"))
      {
         m_enEvalMode = nenEvalShort;
         util::Log::vPrint(util::LOG_INFO, "Set auto_eval to 'short'");
         boResult = true;
      }
      else if (!util::sMakeLower(args[1]).compare("full"))
      {
         m_enEvalMode = nenEvalFull;
         util::Log::vPrint(util::LOG_INFO, "Set auto_eval to 'full'");
         boResult = true;
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Invalid mode argument!");
         boResult = false;
      }
   }
   else
   {
//wti      util::Log::vPrint(util::LOG_ERROR, "Mode argument is missing!");
       switch (m_enEvalMode)
       {
       case nenEvalOff:     util::Log::vPrint(util::LOG_INFO, "auto_eval is 'off'"); break;
       case nenEvalShort:   util::Log::vPrint(util::LOG_INFO, "auto_eval is 'short'"); break;
       case nenEvalFull:    util::Log::vPrint(util::LOG_INFO, "auto_eval is 'full'"); break;
       } //wti
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdSource(const std::vector<std::string> &args)
{
   bool boResult = false;

   if (args.size() > 1)
   {
      boResult = boRunScript(args[1]);
      vSetQuitRequested(false);
   }
   else
   {
      util::Log::vPrint(util::LOG_ERROR, "No script for sourcing specified!");
      boResult = false;
   }

   return boResult;
}

bool CommandInterpreter::boCmdTiming(const std::vector<std::string> &args)
{
   bool boFailed = false;
   int iCycleCount = DEF_TIMING_CYCLE_COUNT;

   if (args.size() > 1)
      iCycleCount = strtoul(args[1].c_str(), NULL, 0);

   // 1. Create Timing section
   TimingSection oTiming;
   oTiming.vSetCycleCount(iCycleCount);

   // 2. Insert Timing section into shared memory
   ShMemBlock oBlock;
   oBlock.vAddSection(oTiming);

   if (!boFailed)
   {
      // 3. Pass data to client
      if (m_oShMemManager.boSendRequest(oBlock) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error sending request!");
         boFailed = true;
      }
   }

   if (!boFailed)
   {
      // 4. Wait for response to be ready
      if (m_oShMemManager.boReceiveResponse(&oBlock) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error receiving request!");
         boFailed = true;
      }
   }

   if (!boFailed)
   {
      // 5. Get result from response
      if (oBlock.boGetSection(&oTiming) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error reading Timing section!");
         boFailed = true;
      }
   }

   if (!boFailed)
   {
      printf("Target needed %lu milliseconds for %i cycles.\n", oTiming.u16GetTimeNeeded(), oTiming.u8GetCycleCount());
      printf("Ratio: %.1f%%\n", oTiming.u16GetTimeNeeded() / (double) oTiming.u8GetCycleCount());
   }

   return !boFailed;
}

bool CommandInterpreter::boWriteTrace(const mrts::test_case &stTestCase)
{
   bool boFailed = false;
   FILE *file = NULL;
   const char *indent = NULL;

   switch (m_enTraceMode)
   {
   case nenTraceOff:
      // Nothing to do
      break;
   case nenTraceConsole:
      // Write trace to console
      file = stdout;
      indent = "    ";
      printf("\nTrace output by target:\n");
      break;
   case nenTraceFile:
      // Write trace to file
      file = fopen(m_sTraceFile.c_str(), "a");
      if (file == NULL)
      {
         util::Log::vPrint(util::LOG_ERROR, "Could not open trace file \"%s\"", m_sTraceFile.c_str());
         boFailed = true;
      }
      indent = "";
      printf("Trace will be written to %s\n", m_sTraceFile.c_str());
      break;
   }

   if (file != NULL)
   {
       // Header needed for trace file only
       if (m_enTraceMode == nenTraceFile)
           fprintf(file, "=== Begin trace for %s ===\n", stTestCase.id.c_str());

       // Output of trace for all streams
       std::vector<std::string>::const_iterator it;
       for (it = stTestCase.output.begin(); it != stTestCase.output.end(); it++)
       {
          //fprintf(file, "%s%s\n", indent, it->c_str());
          // wti: This is for trace output only. Originally the timestamp is at the end
          // of the string. Timestamp at end of line is necessary because otherwise 
          // evaluation would not work.
          std::string orgLine = it->c_str();
          std::string sTime;
          std::string sLine;
          sTime = orgLine.substr(orgLine.find("Timestamp = ") + 12);
          sLine = orgLine.substr(0, orgLine.find(", Timestamp ="));
          fprintf(file, "%s%s : %s\n", indent, sTime.c_str(), sLine.c_str());
       }

      // Footer needed for trace file only
      if (m_enTraceMode == nenTraceFile)
         fprintf(file, "=== End trace for %s ===\n", stTestCase.id.c_str());

      // Closing of file to trace file only
      if (m_enTraceMode == nenTraceFile)
         fclose(file);
   }

   return !boFailed;
}

bool CommandInterpreter::boEvalTestCase(const mrts::test_case &stTestCase, tenEvalMode enMode)
{
   bool boFailed = false;

   if (stTestCase.output.size() > 0)
   {
      eval::match_result match;
      bool boResult = eval::eval_trace(stTestCase.expected, stTestCase.output, &match);
      printf("Evaluation result: %s\n", boResult ? "SUCCESS" : "FAILED");
      if (!boResult && enMode == nenEvalFull)
      {
         std::vector<eval::diff_line> diff;
         eval::diff_match(match, &diff);
         std::vector<std::string> dump;
         eval::dump_diff(diff, &dump);
         for (int i = 0; i < (int)dump.size(); i++)
         {
            int pos = dump[i].find(", Timestamp");  //20170127 wti: Remove Timestamp in this view
            dump[i] = dump[i].substr(0, pos);       //20170127 wti: Remove Timestamp in this view
            printf("    %s\n", dump[i].c_str());
         }
      }
   }
   else
   {
      util::Log::vPrint(util::LOG_WARNING, "This test case has no output to compare!");
      boFailed = true;
   }

   return !boFailed;
}

bool CommandInterpreter::boExecute(const std::string &sLine)
{
   bool boSuccess = false;

   // Skip empty lines and comments
   if (boIsEmpty(sLine))
      return true;
   if (boIsComment(sLine))
      return true;

   std::vector<std::string> tokens;
   vSplitLine(sLine, &tokens);

   if (tokens.size() > 0)
   {
      std::string sCommand = util::sMakeLower(tokens[0]);

      if (!sCommand.compare("quit") || !sCommand.compare("exit"))
      {
         vSetQuitRequested(true);
         boSuccess = true;
      }
      else if (!sCommand.compare("help"))
      {
         vPrintCommandSummary();
         boSuccess = true;
      }
      else if (!sCommand.compare("load"))
      {
         boSuccess = boCmdLoad(tokens);
      }
      else if (!sCommand.compare("reload"))
      {
         boSuccess = boCmdReload(tokens);
      }
      else if (!sCommand.compare("save"))
      {
         boSuccess = boCmdSave(tokens);
      }
      else if (!sCommand.compare("list"))
      {
         boSuccess = boCmdList(tokens);
      }
      else if (!sCommand.compare("show"))
      {
         boSuccess = boCmdShow(tokens);
      }
      else if (!sCommand.compare("run"))
      {
         boSuccess = boCmdRun(tokens);
      }
      else if (!sCommand.compare("eval"))
      {
         boSuccess = boCmdEval(tokens);
      }
      else if (!sCommand.compare("auto_trace"))
      {
         boSuccess = boCmdAutoTrace(tokens);
      }
      else if (!sCommand.compare("auto_eval"))
      {
         boSuccess = boCmdAutoEval(tokens);
      }
      else if (!sCommand.compare("source"))
      {
         boSuccess = boCmdSource(tokens);
      }
      else if (!sCommand.compare("timing"))
      {
         boSuccess = boCmdTiming(tokens);
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Unknown command: \"%s\"", tokens[0].c_str());
      }
   }

   return boSuccess;
}

mrts::test *CommandInterpreter::pstFindTest(mrts::document &oDoc, const std::string &sId)
{
   std::vector<mrts::test>::iterator it;
   for (it = oDoc.tests.begin(); it != oDoc.tests.end(); it++)
      if (!util::sMakeLower(it->id).compare(util::sMakeLower(sId)))
         return &*it;

   return NULL;
}

mrts::test_case *CommandInterpreter::pstFindTestCase(mrts::document &oDoc, const std::string &sId)
{
   std::vector<mrts::test>::iterator it1;
   std::vector<mrts::test_case>::iterator it2;
   for (it1 = oDoc.tests.begin(); it1 != oDoc.tests.end(); it1++)
      for (it2 = it1->test_cases.begin(); it2 != it1->test_cases.end(); it2++)
         if (!util::sMakeLower(it2->id).compare(util::sMakeLower(sId)))
            return &*it2;

   return NULL;
}
