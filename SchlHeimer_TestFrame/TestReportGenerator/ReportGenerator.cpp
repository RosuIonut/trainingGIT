#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <windows.h>

#include <lib-util.hpp>

#include "ReportGenerator.hpp"

const char *assStyleSheet[] =
{
   "body { font-family: Arial, Helvetica, Sans-Serif; }",
   "h1 { text-align: center; }",
   "table { border-collapse: collapse; }",
   "th, td { border-style: solid;  border-width: 1px; border-color: #000000; padding-left: 3px; padding-right: 3px; vertical-align: top; }",
   "th { background: #d8d8d8; text-align: left; }",
   "table.diff { border-style: solid;  border-width: 1px; }",
   "td.diff { border-style: none }",
   ".two_col { width: 1000px; }",
   "th.id { width: 225px; }",
   "th.name { width: 873px; }",
   "th.result { width: 120px; }",
   "th.comment { width: 1080px; }",
   ".code { font-family: Courier New, Courier; }",
   ".good { background: #aaffaa; }",
   ".bad { background: #ffaaaa; }",
   ".neutral { background: #ffffff; }",
};

struct {
   const char c;
   const char *s;
} astHtmlEntityTable[] =
{
   /* char  encoded   */
   {  '"',  "&quot;"  },
   {  '&',  "&amp;"   },
   {  '<',  "&lt;"    },
   {  '>',  "&gt;"    },
   {  '§',  "&sect;"  },
   {  '°',  "&deg;"   },
   {  'µ',  "&micro;" },
};

std::string ReportGenerator::ssEncode(const std::string &usString)
{
   std::string usResult;

   for (std::string::const_iterator it = usString.begin(); it != usString.end(); it++)
   {
      bool boFound = false;

      for (int i = 0; !boFound && i < ARRAY_SIZE(astHtmlEntityTable); i++)
      {
         if (astHtmlEntityTable[i].c == *it)
         {
            usResult.append(astHtmlEntityTable[i].s);
            boFound = true;
         }
      }

      if (!boFound)
         usResult.push_back(*it);
   }

   return usResult;
}

ReportGenerator::tstTestResult ReportGenerator::stTranslateResultCode(mrts::result_code code)
{
   tstTestResult stResult;

   switch (code)
   {
   case mrts::PASSED:
      stResult.ssText = "PASSED";
      stResult.ssStyle = "good";
      break;
   case mrts::FAILED:
      stResult.ssText = "FAILED";
      stResult.ssStyle = "bad";
      break;
   default:
   case mrts::NOT_TESTED:
      stResult.ssText = "NOT TESTED";
      stResult.ssStyle = "neutral";
      break;
   }

   return stResult;
}

void ReportGenerator::vWriteHeader(FILE *file, const std::string &usTitle)
{
   fprintf(file, "  <head>\n");
   fprintf(file, "    <title>%s</title>\n", ssEncode(usTitle).c_str());
   fprintf(file, "    <style type=\"text/css\">\n");
   for (int i = 0; i < ARRAY_SIZE(assStyleSheet); i++)
      fprintf(file, "      %s\n", assStyleSheet[i]);
   fprintf(file, "    </style>\n");
   fprintf(file, "  </head>\n");
}

void ReportGenerator::vWriteTitlePage(FILE *file, const std::string &usTitle)
{
   fprintf(file, "    <h1>%s</h1>\n", ssEncode(usTitle).c_str());
   fprintf(file, "    <div align=\"center\">\n");
   fprintf(file, "      <table>\n");
   fprintf(file, "        <tr>\n");
   fprintf(file, "          <th>Specification:</th>\n");
   fprintf(file, "          <td>%s</td>\n", ssEncode(m_usSpecName).c_str());
   fprintf(file, "        </tr>\n");
   fprintf(file, "        <tr>\n");
   fprintf(file, "          <th>Revision:</th>\n");
   fprintf(file, "          <td>%s</td>\n", ssEncode(m_usSpecRev).c_str());
   fprintf(file, "        </tr>\n");
   fprintf(file, "        <tr>\n");
   fprintf(file, "          <th>Extracted:</th>\n");
   fprintf(file, "          <td>%s</td>\n", ssEncode(mrts::make_process_string(m_stExtracted)).c_str());
   fprintf(file, "        </tr>\n");
   fprintf(file, "        <tr>\n");
   fprintf(file, "          <th>Annotated:</th>\n");
   fprintf(file, "          <td>%s</td>\n", ssEncode(mrts::make_process_string(m_stAnnotated)).c_str());
   fprintf(file, "        </tr>\n");
   fprintf(file, "        <tr>\n");
   fprintf(file, "          <th>Evaluated:</th>\n");
   fprintf(file, "          <td>%s</td>\n", ssEncode(mrts::make_process_string(m_stEvaluated)).c_str());
   fprintf(file, "        </tr>\n");
   fprintf(file, "      </table>\n");
   fprintf(file, "    </div>\n");
}

void ReportGenerator::vWriteTestSummary(FILE *file)
{
   fprintf(file, "    <h2><a name=\"summary\">Test case summary</a></h2>\n");
   for (int i = 0; i < (int) m_oTests.size(); i++)
   {
      fprintf(file, "    <h3>%s: &quot;%s&quot;</h3>\n", ssEncode(m_oTests[i].usId).c_str(), ssEncode(m_oTests[i].usName).c_str());
      fprintf(file, "    <table>\n");

      fprintf(file, "      <tr>\n");
      fprintf(file, "        <th class=\"id\">ID</th>\n");
      fprintf(file, "        <th class=\"name\">Name</th>\n");
      fprintf(file, "        <th class=\"result\">Result</th>\n");
      fprintf(file, "      </tr>\n");

      for (int j = 0; j < (int) m_oTests[i].oTestCases.size(); j++)
      {
         fprintf(file, "      <tr>\n");
         std::string ssId = ssEncode(m_oTests[i].oTestCases[j].usId);
         std::string ssHref;
         if (m_boSingleFile)
            ssHref = "#details_" + ssId;
         else
            ssHref = "details_" + ssId + ".html";
         fprintf(file, "        <td><a href=\"%s\">%s</a></td>\n", ssHref.c_str(), ssId.c_str());
         fprintf(file, "        <td>%s</td>\n", ssEncode(m_oTests[i].oTestCases[j].usName).c_str());
         tstTestResult stResult = stTranslateResultCode(m_oTests[i].oTestCases[j].stResult.code);
         fprintf(file, "        <td class=\"%s\">%s</td>\n",
            stResult.ssStyle.c_str(), stResult.ssText.c_str());
         fprintf(file, "      </tr>\n");
      }

      fprintf(file, "    </table>\n");
   }
}

void ReportGenerator::vWriteTestCaseDetailsSpec(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel)
{
   fprintf(file, "%s  <h%i>Specification</h%i>\n", pcIndent, iLevel + 1, iLevel + 1);
   if (stCase.oInput.size() > 0 || stCase.oExpected.size() > 0)
   {
      fprintf(file, "%s  <table>\n", pcIndent);
      fprintf(file, "%s    <tr>\n", pcIndent);
      fprintf(file, "%s      <th class=\"two_col\">Input</th>\n", pcIndent);
      fprintf(file, "%s      <th class=\"two_col\">Expected output</th>\n", pcIndent);
      fprintf(file, "%s    </tr>\n", pcIndent);
      fprintf(file, "%s    <tr>\n", pcIndent);
      fprintf(file, "%s      <td class=\"code\">\n", pcIndent);
      for (int i = 0; i < (int) stCase.oInput.size(); i++)
          if ('#' == stCase.oInput[i][0]) // 20170302 wti
            fprintf(file, "%s        <font color=\"green\">%s</font><br />\n", pcIndent, ssEncode(stCase.oInput[i]).c_str());
          else
            fprintf(file, "%s        %s<br />\n", pcIndent, ssEncode(stCase.oInput[i]).c_str());
      //  fprintf(file, "%s        %s<br />\n", pcIndent, ssEncode(stCase.oInput[i]).c_str());  // 20170302 wti
      fprintf(file, "%s      </td>\n", pcIndent);
      fprintf(file, "%s      <td class=\"code\">\n", pcIndent);
      for (int i = 0; i < (int) stCase.oExpected.size(); i++)
          if ('#' == stCase.oExpected[i][0]) // 20170302 wti
              fprintf(file, "%s        <font color=\"green\">%s</font><br />\n", pcIndent, ssEncode(stCase.oExpected[i]).c_str());
          else
              fprintf(file, "%s        %s<br />\n", pcIndent, ssEncode(stCase.oExpected[i]).c_str());
      // fprintf(file, "%s        %s<br />\n", pcIndent, ssEncode(stCase.oExpected[i]).c_str());  // 20170302 wti
      fprintf(file, "%s      </td>\n", pcIndent);
      fprintf(file, "%s    </tr>\n", pcIndent);
      fprintf(file, "%s  </table>\n", pcIndent);
   }
}

void ReportGenerator::vWriteTestCaseDetailsTrace(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel)
{
   fprintf(file, "%s  <h%i>Actual output (trace)</h%i>\n", pcIndent, iLevel + 1, iLevel + 1);
   fprintf(file, "%s  <div class=\"code\">\n", pcIndent);
   for (int i = 0; i < (int) stCase.oOutput.size(); i++)
   {
      // wti: This is to adapt the length of the pointer line only. 
      // Originally the timestamp is at the end of the string but this is not displayed therefore
      // an adaption is necessary here.
      std::string sTime;
      std::string sLine;
      sTime = stCase.oOutput[i].substr(stCase.oOutput[i].find("Timestamp = ")+12);
      sLine = stCase.oOutput[i].substr(0, stCase.oOutput[i].find(", Timestamp ="));
      //fprintf(file, "%s    %s<br />\n", pcIndent, ssEncode(stCase.oOutput[i]).c_str());
      fprintf(file, "%s    %s : %s<br />\n", pcIndent, ssEncode(sTime).c_str(), ssEncode(sLine).c_str());
   }
   fprintf(file, "%s  </div>\n", pcIndent);
}

void ReportGenerator::vWriteTestCaseDetailsEval(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel)
{
   tstTestResult stResult = stTranslateResultCode(stCase.stResult.code);

   fprintf(file, "%s  <h%i>Evaluation result</h%i>\n", pcIndent, iLevel + 1, iLevel + 1);
   fprintf(file, "%s  <table>\n", pcIndent);
   fprintf(file, "%s    <tr>\n", pcIndent);
   fprintf(file, "%s      <th class=\"result\">Result</th>\n", pcIndent);
   fprintf(file, "%s      <th class=\"comment\">Comment</th>\n", pcIndent);
   fprintf(file, "%s    </tr>\n", pcIndent);
   fprintf(file, "%s    <tr>\n", pcIndent);
   fprintf(file, "%s      <td class=\"%s\">%s</td>\n", pcIndent, stResult.ssStyle.c_str(), stResult.ssText.c_str());
   fprintf(file, "%s      <td>%s</td>\n", pcIndent, ssEncode(stCase.stResult.comment).c_str());
   fprintf(file, "%s    </tr>\n", pcIndent);
   fprintf(file, "%s  </table>\n", pcIndent);

   if (stCase.oDiff.size() > 0)
   {
      fprintf(file, "%s  <br />\n", pcIndent);
      fprintf(file, "%s  <table class=\"diff\">\n", pcIndent);
      fprintf(file, "%s    <tr>\n", pcIndent);
      fprintf(file, "%s      <th class=\"two_col\">Expected output</th>\n", pcIndent);
      fprintf(file, "%s      <th class=\"two_col\">Actual output</th>\n", pcIndent);
      fprintf(file, "%s    </tr>\n", pcIndent);
      for (int i = 0; i < (int) stCase.oDiff.size(); i++)
      {
         std::string ssStyle;
         switch (stCase.oDiff[i].result)
         {
         case eval::nenMatched:   ssStyle = "good";    break;
         case eval::nenExpFailed: ssStyle = "bad";     break;
         case eval::nenDontCare:  ssStyle = "neutral"; break;
         }

         // wti: This is to remove the timestamp is at the end of the actual output string.
         std::string sLine = stCase.oDiff[i].right.substr(0, stCase.oDiff[i].right.find(", Timestamp ="));  //20170302 wti

         fprintf(file, "%s    <tr>\n", pcIndent);
         fprintf(file, "%s      <td class=\"two_col diff code %s\">%s</td>\n", pcIndent, ssStyle.c_str(), ssEncode(stCase.oDiff[i].left).c_str());
       //fprintf(file, "%s      <td class=\"two_col diff code %s\">%s</td>\n", pcIndent, ssStyle.c_str(), ssEncode(stCase.oDiff[i].right).c_str());  //20170302 wti
         fprintf(file, "%s      <td class=\"two_col diff code %s\">%s</td>\n", pcIndent, ssStyle.c_str(), ssEncode(sLine).c_str());
         fprintf(file, "%s    </tr>\n", pcIndent);
      }
      fprintf(file, "%s  </table>\n", pcIndent);
   }
}

bool ReportGenerator::boWriteTestCaseDetails(FILE *file, const tstTestCaseEntry &stCase)
{
   bool boFailed = false;
   FILE *file_details = NULL;

   // Open file in case of multi file configuration, use
   // same file otherwise
   if (m_boSingleFile)
   {
      file_details = file;
   }
   else
   {
      std::string usFile = m_usOutputPath + "/details_" + stCase.usId + ".html";
      util::Log::vPrint(util::LOG_INFO, "Creating detail file %s", usFile.c_str());
      file_details = fopen(usFile.c_str(), "w");
      if (file_details == NULL)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error creating detail file: %s", usFile.c_str());
         boFailed = true;
      }
   }

   if (!boFailed)
   {
      std::string usTitle = "Details for " + stCase.usId + ": \"" + stCase.usName + "\"";
      int iLevel;
      const char *pcIndent;

      if (m_boSingleFile)
      {
         pcIndent = "  ";
         iLevel = 3;
      }
      else
      {
         pcIndent = "";
         iLevel = 1;
         fprintf(file_details, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n");
         fprintf(file_details, "\t\"http://www.w3.org/TR/html4/strict.dtd\">\n");
         fprintf(file_details, "<html>\n");
         vWriteHeader(file_details, usTitle);
         fprintf(file_details, "  <body>\n");
      }

      fprintf(file_details, "%s  <h%i><a name=\"details_%s\">%s</a></h%i>\n",
         pcIndent, iLevel, ssEncode(stCase.usId).c_str(), ssEncode(usTitle).c_str(), iLevel);

      vWriteTestCaseDetailsSpec(file_details, stCase, pcIndent, iLevel);
      vWriteTestCaseDetailsTrace(file_details, stCase, pcIndent, iLevel);
      vWriteTestCaseDetailsEval(file_details, stCase, pcIndent, iLevel);

      std::string ssUrl = m_boSingleFile ? "#summary" : "report.html";
      fprintf(file_details, "%s  <p>\n", pcIndent);
      fprintf(file_details, "%s    <a href=\"%s\">Back to summary</a>\n", pcIndent, ssUrl.c_str());
      fprintf(file_details, "%s  </p>\n", pcIndent);

      if (!m_boSingleFile)
      {
         fprintf(file_details, "  </body>\n");
         fprintf(file_details, "</html>\n");
      }
   }

   if (!m_boSingleFile)
   {
      if (file_details != NULL)
         fclose(file_details);
   }

   return !boFailed;
}

bool ReportGenerator::boWriteTestDetails(FILE *file, const tstTestEntry &stTest)
{
   bool boFailed = false;

   // This function adds output to the HTML file only in single file
   // mode. Otherwise the detailed files are created for each test case.
   if (m_boSingleFile)
   {
      fprintf(file, "    <h2>Details for %s: &quot;%s&quot;</h2>\n", ssEncode(stTest.usId).c_str(), ssEncode(stTest.usName).c_str());
   }

   for (int i = 0; !boFailed && i < (int) stTest.oTestCases.size(); i++)
   {
      if (boWriteTestCaseDetails(file, stTest.oTestCases[i]) == false)
         boFailed = true;
   }

   return !boFailed;
}

bool ReportGenerator::boWriteMainFile(FILE *file)
{
   bool boFailed = false;

   fprintf(file, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n");
   fprintf(file, "\t\"http://www.w3.org/TR/html4/strict.dtd\">\n");
   fprintf(file, "<html>\n");
   // Header with title and style sheet
   vWriteHeader(file, "Integration test report for " + m_usSpecName);
   fprintf(file, "  <body>\n");
   // Title page with info table
   vWriteTitlePage(file, "Integration test report for " + m_usSpecName);
   fprintf(file, "  \n");
   // Summary table with links
   vWriteTestSummary(file);
   fprintf(file, "  \n");
   // Write details for tests
   for (int i = 0; !boFailed && i < (int) m_oTests.size(); i++)
      if (boWriteTestDetails(file, m_oTests[i]) == false)
         boFailed = true;
   fprintf(file, "  </body>\n");

   fprintf(file, "</html>\n");

   return !boFailed;
}

bool ReportGenerator::boWriteReport(void)
{
   bool boFailed = false;

   util::Log::vPrint(util::LOG_INFO, "Writing HTML output into directory \"%s\"", m_usOutputPath.c_str());

   // First create output directory, if not already existing
   if (util::boCreatePath(m_usOutputPath) == false)
   {
      util::Log::vPrint(util::LOG_ERROR, "Could not create output path: %s", m_usOutputPath.c_str());
      boFailed = true;
   }

   if (!boFailed)
   {
      std::string usReportFile = m_usOutputPath + "/report.html";
      util::Log::vPrint(util::LOG_INFO, "Creating main report file (%s)", usReportFile.c_str());
      FILE *file = fopen(usReportFile.c_str(), "w");
      if (file)
      {
         if (boWriteMainFile(file) == false)
         {
            // Some sub function failed, it should already have reported this
            boFailed = true;
         }
         fclose(file);
      }
      else
      {
         util::Log::vPrint(util::LOG_ERROR, "Could not create report file: %s", usReportFile.c_str());
         boFailed = true;
      }
   }

   return !boFailed;
}

void ReportGenerator::vAddTest(const std::string &usId, const std::string &usName)
{
   tstTestEntry stTest;
   stTest.usId = usId;
   stTest.usName = usName;
   m_oTests.push_back(stTest);
}

void ReportGenerator::vAddTestCase(const std::string &usTestId, const std::string &usId, const std::string &usName)
{
   for (int i = 0; i < (int) m_oTests.size(); i++)
   {
      if (m_oTests[i].usId.compare(usTestId) == 0)
      {
         tstTestCaseEntry stCase;
         stCase.usId = usId;
         stCase.usName = usName;
         m_oTests[i].oTestCases.push_back(stCase);
      }
   }
}

ReportGenerator::tstTestCaseEntry *ReportGenerator::pstFindTestCase(const std::string &usTestCaseId)
{
   for (int i = 0; i < (int) m_oTests.size(); i++)
   {
      for (int j = 0; j < (int) m_oTests[i].oTestCases.size(); j++)
      {
         if (m_oTests[i].oTestCases[j].usId.compare(usTestCaseId) == 0)
            return &m_oTests[i].oTestCases[j];
      }
   }

   return NULL;
}

void ReportGenerator::vAddTestCaseInput(const std::string &usTestCaseId, const std::vector<std::string> &oInput)
{
   tstTestCaseEntry *pstCase = pstFindTestCase(usTestCaseId);
   if (pstCase)
   {
      pstCase->oInput = oInput;
   }
}

void ReportGenerator::vAddTestCaseExpected(const std::string &usTestCaseId, const std::vector<std::string> &oExpected)
{
   tstTestCaseEntry *pstCase = pstFindTestCase(usTestCaseId);
   if (pstCase)
   {
      pstCase->oExpected = oExpected;
   }
}

void ReportGenerator::vAddTestCaseOutput(const std::string &usTestCaseId, const std::vector<std::string> &oOutput)
{
   tstTestCaseEntry *pstCase = pstFindTestCase(usTestCaseId);
   if (pstCase)
   {
      pstCase->oOutput = oOutput;
   }
}

void ReportGenerator::vAddTestCaseResult(const std::string &usTestCaseId, const mrts::test_result &stResult)
{
   tstTestCaseEntry *pstCase = pstFindTestCase(usTestCaseId);
   if (pstCase)
   {
      pstCase->stResult = stResult;
   }
}

void ReportGenerator::vAddTestCaseDiff(const std::string &usTestCaseId, const std::vector<eval::diff_line> &oDiff)
{
   tstTestCaseEntry *pstCase = pstFindTestCase(usTestCaseId);
   if (pstCase)
   {
      pstCase->oDiff = oDiff;
   }
}
