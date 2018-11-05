#ifndef LIB_MRTS_HPP
#define LIB_MRTS_HPP

#include <string>
#include <vector>
#include <ctime>

#define MRTS_LIB_MAJOR_VER 0x01
#define MRTS_LIB_MINOR_VER 0x00
#define MRTS_LIB_LINE_MAX  79
#define MRTS_LIB_TITLE_MAX 60
#define MRTS_LIB_PROC_MAX  80

namespace mrts
{
   enum result_code
   {
      NOT_TESTED,
      FAILED,
      PASSED
   };

   struct test_result
   {
      result_code code;
      std::string comment;
   };

   struct test_case
   {
      std::string id;
      std::string name;
      int prio;
      std::vector<std::string> input;
      std::vector<std::string> expected;
      std::vector<std::string> output;
      test_result result;
   };

   struct test
   {
      std::string id;
      std::string name;
      std::vector<test_case> test_cases;
   };
   typedef std::vector<test_case>::iterator test_case_iterator;
   typedef std::vector<test_case>::const_iterator test_case_const_iterator;

   struct process_step
   {
      bool present;
      bool well_formed;
      struct tm date;
      std::string actor;
   };

   struct document
   {
      std::string format;
      std::string filename;
      std::string revision;
      process_step extracted;
      process_step annotated;
      process_step evaluated;
      std::vector<test> tests;
   };
   typedef std::vector<test>::iterator test_iterator;
   typedef std::vector<test>::const_iterator test_const_iterator;

   enum fail_reason
   {
      NO_ERROR_OCCURED,
      INVALID_PARAMETER,
      OPEN_FILE_FAILED,
      INVALID_BLOCK_NAME,
      NESTED_BLOCK_FOUND,
      TOO_MANY_BLOCK_ENDS,
      INVALID_ATTRIBUTE_NAME,
      MALFORMED_BEGIN_TEST,
      MALFORMED_BEGIN_TEST_CASE,
      UNCLEAR_TEST_RESULT,
      INVALID_INPUT_LINE,
   };

   struct fail_info
   {
      fail_reason reason;
      int line_no;
      std::string line_text;
      std::string word;
   };

   bool read(const std::string &path, document *doc, fail_info *fail = NULL);
   bool write(const std::string &path, const document &doc);
   std::string fail_reason_to_string(mrts::fail_reason reason);
   std::string make_process_string(const mrts::process_step &step);
   process_step make_process_step(const std::string &actor);
}

#endif
