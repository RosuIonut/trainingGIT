#include "match.hpp"
#include <lib-util.hpp>

namespace eval
{

   int last_matched(const std::vector<match_line> &v)
   {
      int i = (int) v.size() - 1;
      while (i >= 0 && v[i].match == -1)
         i--;
      return i;
   }

   bool does_arg_match(std::string a, std::string b)
   {
      a = util::sMakeLower(a);
      b = util::sMakeLower(b);

      // Exact match?
      if (a.compare(b) == 0)
         return true;

      // Maybe numeric values?
      char *end;
      unsigned long a_num = strtoul(a.c_str(), &end, 0);
      bool a_is_num = *end == '\0';
      unsigned long b_num = strtoul(b.c_str(), &end, 0);
      bool b_is_num = *end == '\0';

      if (a_is_num && b_is_num)
         return a_num == b_num;

      // Maybe booleans?
      if (!a.compare("true") && b_is_num)
         return b_num != 0;
      if (!a.compare("false") && b_is_num)
         return b_num == 0;

      return false;
   }

   bool do_lines_match(const std::string &exp, const std::string &out)
   {
      bool match = true;

      std::string cmd_name;
      std::vector<std::string> cmd_args;

      bool boExpValid = util::boParseCommandLine(exp, &cmd_name, &cmd_args);

      std::string trace_type;
      std::vector<std::string> trace_args;

      bool boOutValid = util::boParseTraceLine(out, &trace_type, &trace_args);

      std::string lwr_cmd_name = util::sMakeLower(cmd_name);
      if (!lwr_cmd_name.compare("happened") || !lwr_cmd_name.compare("nothappened"))
      {
         // Type must be event, otherwise no match
         if (util::sMakeLower(trace_type).compare("event") != 0)
            match = false;
      }
      else if (!lwr_cmd_name.compare("changed") || !lwr_cmd_name.compare("notchanged"))
      {
         // Type must be data, otherwise no match
         if (util::sMakeLower(trace_type).compare("data") != 0)
            match = false;
      }
      else
      {
         // Unexpected input: command does neither request Event nor Data.
         // Maybe a new type was invented and this project must be adapted?
         match = false;
      }

      // If input args is bigger than trace args the lines cannot match
      if (cmd_args.size() > trace_args.size())
         match = false;
      
      // If a match is still an option, check the actual arguments.
      // All arguments in the exepcted output must match the actual trace
      // arguments exactly. If there are more arguments in the trace, this is
      // not a problem (e.g. any KeyMessage is expected, but the trace contains
      // a very specific KeyMessage; this is a match).
      for (int i = 0; match && (i < (int) cmd_args.size()); i++)
         if (!does_arg_match(cmd_args[i], trace_args[i]))
            match = false;

      return match;
   }

   bool is_specific(const std::string &exp)
   {
      bool result = false;

      std::string cmd;
      std::vector<std::string> args;

      if (util::boParseCommandLine(exp, &cmd, &args))
      {
         if (util::sMakeLower(cmd).compare("happened") == 0)
            result = true;
         if (util::sMakeLower(cmd).compare("changed") == 0)
            result = true;
      }

      return result;
   }

   bool match_expected_with_trace(match_result *result)
   {
      bool failed = false;

      // 1st pass: align every specific expected line with an output line
      result->first_unmatched_expected = -1;
      for (int i = 0; !failed && i < (int) result->expected.size(); i++)
      {
         // Is specific: Happened, Changed
         if (is_specific(result->expected[i].line))
         {
            int j = last_matched(result->trace) + 1;
            bool matched = false;
            while (!matched && j < (int) result->trace.size())
            {
               if (do_lines_match(result->expected[i].line, result->trace[j].line))
               {
                  result->expected[i].match = j;
                  result->trace[j].match = i;
                  matched = true;
               }
               j++;
            }
            // Still not matched?
            if (!matched)
            {
               result->first_unmatched_expected = i;
               failed = true;
            }
         }
      }

      // 2nd pass: check if NotHappened and NotChanged are satisfied
      result->first_matched_unexpected = -1;
      for (int i = 0; !failed && i < (int) result->expected.size(); i++)
      {
         // Is not specific: NotHappened, NotChanged
         if (!is_specific(result->expected[i].line))
         {
            // Lower limit: the first trace line that could be evil
            int low_lim = 0;
            for (int j = i - 1; j >= 0; j--)
            {
               if (result->expected[j].match != -1)
               {
                  low_lim = result->expected[j].match + 1;
                  break;
               }
            }
            // Upper limit: the first trace line that is above the possibly evil
            int up_lim = (int) result->trace.size();
            // Try to find a closer match
            for (int j = i; j < (int) result->expected.size(); j++)
            {
               if (result->expected[j].match != -1)
               {
                  up_lim = result->expected[j].match;
                  break;
               }
            }

            // Now check if the evil exists between low_lim and up_lim
            for (int j = low_lim; j < up_lim; j++)
            {
               if (do_lines_match(result->expected[i].line, result->trace[j].line))
               {
                  result->first_matched_unexpected = j;
                  failed = true;               
               }
            }
         }
      }

      return !failed;
   }

   bool eval_trace(const std::vector<std::string> &expected, const std::vector<std::string> &trace, match_result *result)
   {
      if (result != NULL)
      {
         for (int i = 0; i < (int) expected.size(); i++)
         {
            struct match_line l;
            l.line = expected[i];
            l.match = -1;
            result->expected.push_back(l);
         }

         for (int i = 0; i < (int) trace.size(); i++)
         {
            struct match_line l;
            l.line = trace[i];
            l.match = -1;
            result->trace.push_back(l);
         }
         
         return match_expected_with_trace(result);
      }

      return false;
   }

}
