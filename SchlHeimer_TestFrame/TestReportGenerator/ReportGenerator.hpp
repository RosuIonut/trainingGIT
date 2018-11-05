#ifndef REPORTGENERATOR_HPP
#define REPORTGENERATOR_HPP

#include <string>
#include <cstdio>
#include <lib-eval.hpp>
#include <lib-mrts.hpp>

// In this file, a special naming convention is used to make sure that strings
// to be printed in HTML are "safe" (e.g. do not contain special characters
// that need encoding):
//
//   us = unsafe string
//   ss = safe string
//
// "normal" strings (e.g. passed as part of a structure already in use, like
// process_step) must be considered as unsafe.
//
// For a good read about this topic, see here:
// http://www.joelonsoftware.com/articles/Wrong.html

class ReportGenerator
{
public:
   ReportGenerator() : m_boSingleFile(false)
   {
   }

   virtual ~ReportGenerator()
   {
   }

   void vSetSingleFile(void)
   {
      m_boSingleFile = true;
   }

   void vSetOutputPath(const std::string &usPath)
   {
      m_usOutputPath = usPath;
   }

   void vSetSpecName(const std::string &usName)
   {
      m_usSpecName = usName;
   }

   void vSetSpecRev(const std::string &usRev)
   {
      m_usSpecRev = usRev;
   }

   void vSetExtracted(const mrts::process_step &stStep)
   {
      m_stExtracted = stStep;
   }

   void vSetAnnotated(const mrts::process_step &stStep)
   {
      m_stAnnotated = stStep;
   }

   void vSetEvaluated(const mrts::process_step &stStep)
   {
      m_stEvaluated = stStep;
   }

   bool boWriteReport(void);

   // Methods called by TestEvaluator to fill the internal data
   // structures from which the report is generated
   void vAddTest(const std::string &usId, const std::string &usName);
   void vAddTestCase(const std::string &usTestId, const std::string &usId, const std::string &usName);
   void vAddTestCaseInput(const std::string &usTestCaseId, const std::vector<std::string> &oInput);
   void vAddTestCaseExpected(const std::string &usTestCaseId, const std::vector<std::string> &oExpected);
   void vAddTestCaseOutput(const std::string &usTestCaseId, const std::vector<std::string> &oOutput);
   void vAddTestCaseResult(const std::string &usTestCaseId, const mrts::test_result &stResult);
   void vAddTestCaseDiff(const std::string &usTestCaseId, const std::vector<eval::diff_line> &oDiff);

private:

   typedef struct
   {
      std::string usId;
      std::string usName;
      std::vector<std::string> oInput;
      std::vector<std::string> oExpected;
      std::vector<std::string> oOutput;
      mrts::test_result stResult;
      std::vector<eval::diff_line> oDiff;
   } tstTestCaseEntry;

   typedef struct
   {
      std::string usId;
      std::string usName;
      std::vector<tstTestCaseEntry> oTestCases;
   } tstTestEntry;

   typedef struct
   {
      std::string ssText;
      std::string ssStyle;
   } tstTestResult;

   // Encode special characters in a string into HTML entities
   std::string ssEncode(const std::string &usString);

   // Translation of a result code (PASSED/FAILED) into a
   // textual representation and a matching CSS class
   tstTestResult stTranslateResultCode(mrts::result_code code);

   // Find a test case entry by its ID
   tstTestCaseEntry *pstFindTestCase(const std::string &usTestCaseId);

   // Write main file (report.html)
   bool boWriteMainFile(FILE *file);
   // Write header with style sheet and title
   void vWriteHeader(FILE *file, const std::string &usTitle);
   // Write title page with info table
   void vWriteTitlePage(FILE *file, const std::string &usTitle);
   // Write test summary table with links to specific test cases
   void vWriteTestSummary(FILE *file);
   // Write test details (either inline or as separate file)
   bool boWriteTestDetails(FILE *file, const tstTestEntry &stTest);
   // Write test case details (either inline or as separate file)
   bool boWriteTestCaseDetails(FILE *file, const tstTestCaseEntry &stCase);
   // Write input/expected specification for test case
   void vWriteTestCaseDetailsSpec(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel);
   // Write actual output (trace) for test case
   void vWriteTestCaseDetailsTrace(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel);
   // Write evaluation result for test case
   void vWriteTestCaseDetailsEval(FILE *file, const tstTestCaseEntry &stCase, const char *pcIndent, int iLevel);

   // Configuration set by caller
   std::string m_usOutputPath;
   bool m_boSingleFile;

   // Testspec name and revision for title page
   std::string m_usSpecName;
   std::string m_usSpecRev;

   // Process steps for history
   mrts::process_step m_stExtracted;
   mrts::process_step m_stAnnotated;
   mrts::process_step m_stEvaluated;

   // Tests and test case entries
   std::vector<tstTestEntry> m_oTests;
};

#endif
