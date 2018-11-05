#ifndef LIB_EVAL_HPP
#define LIB_EVAL_HPP

#include <string>
#include <vector>

namespace eval
{
   struct match_line
   {
      std::string line;
      int match;
   };

   struct match_result
   {
      std::vector<match_line> expected;
      int first_unmatched_expected;

      std::vector<match_line> trace;
      int first_matched_unexpected;
   };

   enum diff_result
   {
      nenMatched,
      nenDontCare,
      nenExpFailed,
   };

   struct diff_line
   {
      std::string left;
      std::string right;
      diff_result result;
   };

   bool eval_trace(const std::vector<std::string> &expected, const std::vector<std::string> &trace, match_result *result);
   void diff_match(const match_result &match, std::vector<diff_line> *diff);
   void dump_diff(const std::vector<diff_line> &diff, std::vector<std::string> *dump);
}

#endif
