#include "lib-eval.hpp"

namespace eval
{
   void dump_unmatched_expected(const std::string &exp, std::vector<std::string> *dump)
   {
      std::string line;

      // expectation that failed
      dump->push_back(exp);

      // pointer
      for (int n = 0; n < (int) exp.length(); n++)
         line.push_back('^');

      // hint
      line += " first unmatched";
      dump->push_back(line);
   }

   void dump_matched_unexpected(const std::string &trc, int indent, std::vector<std::string> *dump)
   {
      std::string line;
      const std::string hint = "first unexpected ";

      // alignment for right column
      int pos = 0;
      while (pos++ < indent)
         line.push_back(' ');

      // unexpected event
      line += trc;
      dump->push_back(line);
      line.clear();

      // hint
      pos = 0;
      while (pos++ < indent - (int) hint.length())
         line.push_back(' ');
      line += hint;

      // pointer
      // wti: This is to adapt the length of the pointer line only. 
      // Originally the timestamp is at the end of the string but this is not displayed therefore
      // an adaption is necessary here.
      std::string sLine;
      sLine = trc.substr(0, trc.find(", Timestamp ="));
      //for (int n = 0; n < (int) trc.length(); n++)
      for (int n = 0; n < (int) sLine.length(); n++)
         line.push_back('^');
      dump->push_back(line);
   }

   void dump_diff(const std::vector<diff_line> &diff, std::vector<std::string> *dump)
   {
      // Have a look for the longest line to be printed.
      // This is the width of the left column.
      int lc_width = 0;
      for (int i = 0; i < (int) diff.size(); i++)
      {
         if (lc_width < (int) diff[i].left.length())
            lc_width = (int) diff[i].left.length();
      }

      const std::string marker_exact = " <==> ";
      const std::string marker_empty = "      ";

      for (int i = 0; i < (int) diff.size(); i++)
      {
         std::string line;

         if (diff[i].result == nenExpFailed)
         {
            if (!diff[i].left.empty())
            {
               dump_unmatched_expected(diff[i].left, dump);
            }
            else
            {
               int indent = lc_width + (int) marker_empty.length();
               dump_matched_unexpected(diff[i].right, indent, dump);
            }
         }
         else
         {
            int pos = 0;
            if (!diff[i].left.empty())
            {
               // print left column
               line = diff[i].left;
               pos = (int) line.length();
            }

            // Filling and alignment indicator only needed of there is a right column
            if (!diff[i].right.empty())
            {
               // alignment for right column
               while (pos++ < lc_width)
                  line.push_back(' ');

               if (diff[i].result == nenMatched)
                  line += marker_exact;
               else
                  line += marker_empty;
            }

            if (!diff[i].right.empty())
            {
               // print right column
               line += diff[i].right;
            }
         }

         dump->push_back(line);
      }   
   }

}
