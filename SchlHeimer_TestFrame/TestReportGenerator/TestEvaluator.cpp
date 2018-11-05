#include <sstream>

#include <lib-mrts.hpp>
#include <lib-util.hpp>

#include "TestEvaluator.hpp"

void TestEvaluator::vInsertResult(mrts::test_case &stCase, ReportGenerator *poRepGen)
{
   mrts::result_code code;
   std::string comment;
   std::vector<eval::diff_line> diff;

   if (stCase.result.code == mrts::NOT_TESTED || m_boForceEvaluation)
   {
      if (stCase.result.code != mrts::NOT_TESTED && m_boForceEvaluation)
         util::Log::vPrint(util::LOG_WARNING, "%s is forced to be evaluated again", stCase.id.c_str());

      if (stCase.output.size() > 0)
      {
         eval::match_result match;
         bool boResult = eval::eval_trace(stCase.expected, stCase.output, &match);
         if (boResult)
         {
            // Evaluation successful, test case is PASSED
            code = mrts::PASSED;
         }
         else
         {
            // Evaluation shows difference in behavior, test case is FAILED
            code = mrts::FAILED;

            // Create a comment string from what the evaluation found out
            std::stringstream s;
            if (match.first_unmatched_expected != -1)
            {
               s << "expectation \"";
               s << match.expected[match.first_unmatched_expected].line;
               s << "\" failed in line ";
               s << match.first_unmatched_expected + 1;
            }
            else if (match.first_matched_unexpected != -1)
            {
               s << "unexpected event \"";
               s << match.trace[match.first_matched_unexpected].line;
               s << "\" happened in line ";
               s << match.first_matched_unexpected + 1;
            }
            comment = s.str();

            // Get a diff from the matching result and store for report generation
            eval::diff_match(match, &diff);
         }
      }
      else
      {
         // Test case has not output => was not tested
         code = mrts::NOT_TESTED;
      }

      stCase.result.code = code;
      stCase.result.comment = comment;

      if (code == mrts::PASSED)
         util::Log::vPrint(util::LOG_INFO, "%s PASSED.", stCase.id.c_str());
      else if (code == mrts::FAILED)
         util::Log::vPrint(util::LOG_INFO, "%s FAILED: %s", stCase.id.c_str(), comment.c_str());
      else if (code == mrts::NOT_TESTED)
         util::Log::vPrint(util::LOG_INFO, "Skipped %s because it was empty", stCase.id.c_str());
   }
   else
   {
      util::Log::vPrint(util::LOG_INFO, "Skipped %s because it was already evaluated", stCase.id.c_str());
   }

   poRepGen->vAddTestCaseResult(stCase.id, stCase.result);
   poRepGen->vAddTestCaseDiff(stCase.id, diff);

}

bool TestEvaluator::boEvaluate(ReportGenerator *poRepGen)
{
   bool boFailed = false;
   mrts::document oDoc;

   if (boLoadTestSpec(m_sInputPath, &oDoc))
   {
      poRepGen->vSetSpecName(oDoc.filename);
      poRepGen->vSetSpecRev(oDoc.revision);
      poRepGen->vSetExtracted(oDoc.extracted);
      poRepGen->vSetAnnotated(oDoc.annotated);
      // Insert ourself as evauator
      oDoc.evaluated = mrts::make_process_step(m_sActorString);
      poRepGen->vSetEvaluated(oDoc.evaluated);

      util::Log::vPrint(util::LOG_INFO, "Starting evaluation of test cases");
      for (int iTestIdx = 0; iTestIdx < (int) oDoc.tests.size(); iTestIdx++)
      {
         const std::string sTestId = oDoc.tests[iTestIdx].id;
         poRepGen->vAddTest(sTestId, oDoc.tests[iTestIdx].name);

         for (int iCaseIdx = 0; iCaseIdx < (int) oDoc.tests[iTestIdx].test_cases.size(); iCaseIdx++)
         {
            const std::string &sCaseId = oDoc.tests[iTestIdx].test_cases[iCaseIdx].id;
            poRepGen->vAddTestCase(sTestId, sCaseId, oDoc.tests[iTestIdx].test_cases[iCaseIdx].name);
            poRepGen->vAddTestCaseInput(sCaseId, oDoc.tests[iTestIdx].test_cases[iCaseIdx].input);
            poRepGen->vAddTestCaseExpected(sCaseId, oDoc.tests[iTestIdx].test_cases[iCaseIdx].expected);
            poRepGen->vAddTestCaseOutput(sCaseId, oDoc.tests[iTestIdx].test_cases[iCaseIdx].output);

            vInsertResult(oDoc.tests[iTestIdx].test_cases[iCaseIdx], poRepGen);
         }
      }
      util::Log::vPrint(util::LOG_INFO, "Evaluation of test cases finished");

      // First create output directory, if not already existing
      if (util::boCreatePath(m_sOutputPath) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Could not create output path: %s", m_sOutputPath.c_str());
         boFailed = true;
      }

      std::string sSavePath = m_sOutputPath + "/" + util::basename(m_sInputPath.c_str());
      util::Log::vPrint(util::LOG_INFO, "Saving evaluated test specification to %s", sSavePath.c_str());
      if (mrts::write(sSavePath, oDoc) == false)
      {
         util::Log::vPrint(util::LOG_ERROR, "Error saving evaluated test specification to %s", sSavePath.c_str());
         boFailed = true;
      }
   }
   else
   {
      // Loading test spec failed
      boFailed = true;
   }

   return !boFailed;
}


bool TestEvaluator::boLoadTestSpec(std::string sFile, mrts::document *poDoc)
{
   bool boResult = false;
   mrts::fail_info fail;

   if (mrts::read(sFile, poDoc, &fail))
   {
      util::Log::vPrint(util::LOG_INFO, "Test specification loaded:");
      util::Log::vPrint(util::LOG_INFO, "\tFilename:        %s", poDoc->filename.c_str());
      util::Log::vPrint(util::LOG_INFO, "\tRevision:        %s", poDoc->revision.c_str());
      if (poDoc->extracted.present)
         util::Log::vPrint(util::LOG_INFO, "\tExtracted:       %s", mrts::make_process_string(poDoc->extracted).c_str());
      if (poDoc->annotated.present)
         util::Log::vPrint(util::LOG_INFO, "\tAnnotated:       %s", mrts::make_process_string(poDoc->annotated).c_str());
      util::Log::vPrint(util::LOG_INFO, "\tNumber of tests: %i", poDoc->tests.size());

      boResult = true;
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

      boResult = false;
   }

   return boResult;
}
