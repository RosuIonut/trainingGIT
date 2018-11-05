#include "lib-eval.hpp"

namespace eval
{
   void diff_match(const match_result &match, std::vector<diff_line> *diff)
   {
      // Loop as long as there are lines to print
      bool done = false;
      int e_index = 0;
      int t_index = 0;
      while (!done)
      {
         diff_line line;

         // "good" means: index is valid
         const bool e_good = e_index < (int) match.expected.size();
         const bool t_good = t_index < (int) match.trace.size();

         // "print" means: column will be printed
         bool e_print = false;
         bool t_print = false;

         if (e_index == match.first_unmatched_expected)
         {
            line.left = match.expected[e_index].line;
            line.result = nenExpFailed;
            diff->push_back(line);
            done = true;
            continue;
         }

         if (t_index == match.first_matched_unexpected)
         {
            line.right = match.trace[t_index].line;
            line.result = nenExpFailed;
            diff->push_back(line);
            done = true;
            continue;
         }

         if (e_good)
         {
            // left side is printed if either right side is already at end
            // or if left side entry is matched by current right side entry
            // or if left side entry is random
            e_print = !t_good || e_index <= match.trace[t_index].match || match.expected[e_index].match == -1;
         }

         if (t_good)
         {
            // right side is printed if either left side is already at end
            // or if right side entry is matched by current left side entry
            // or if right side entry is random
            t_print = !e_good || t_index <= match.expected[e_index].match || match.trace[t_index].match == -1;
         }

         // If both are printed and not random, the match is exact, otherwise just aligned
         if (e_print && e_print && match.expected[e_index].match != -1)
            line.result = nenMatched;
         else
            line.result = nenDontCare;

         if (e_print)
            line.left = match.expected[e_index++].line;

         if (t_print)
            line.right = match.trace[t_index++].line;

         diff->push_back(line);
      }   
   }

}
