#include <fstream>

#include "mrts-parser.hpp"
#include "mrts-writer.hpp"

#include "lib-mrts.hpp"

bool mrts::read(const std::string &path, mrts::document *doc, mrts::fail_info *fail)
{
   struct fail_info dummy;

   if (fail == NULL)
      fail = &dummy;

   fail->reason = NO_ERROR_OCCURED;
   fail->line_no = 0;

   if (doc == NULL)
   {
      fail->reason = INVALID_PARAMETER;
      return false;
   }

   std::ifstream in(path.c_str());
   if (!in.is_open())
   {
      fail->reason = OPEN_FILE_FAILED;
      return false;
   }

   parser p;
   return p.parse(in, doc, fail);
}

bool mrts::write(const std::string &path, const document &doc)
{
   std::ofstream out(path.c_str());
   if (!out.is_open())
      return false;

   writer w;
   return w.write(out, doc);
}

std::string mrts::fail_reason_to_string(mrts::fail_reason reason)
{
   switch (reason)
   {
   case mrts::NO_ERROR_OCCURED:          return "NO_ERROR_OCCURED";
   case mrts::INVALID_PARAMETER:         return "INVALID_PARAMETER";
   case mrts::OPEN_FILE_FAILED:          return "OPEN_FILE_FAILED";
   case mrts::INVALID_BLOCK_NAME:        return "INVALID_BLOCK_NAME";
   case mrts::NESTED_BLOCK_FOUND:        return "NESTED_BLOCK_FOUND";
   case mrts::TOO_MANY_BLOCK_ENDS:       return "TOO_MANY_BLOCK_ENDS";
   case mrts::INVALID_ATTRIBUTE_NAME:    return "INVALID_ATTRIBUTE_NAME";
   case mrts::MALFORMED_BEGIN_TEST:      return "MALFORMED_BEGIN_TEST";
   case mrts::MALFORMED_BEGIN_TEST_CASE: return "MALFORMED_BEGIN_TEST_CASE";
   case mrts::UNCLEAR_TEST_RESULT:       return "UNCLEAR_TEST_RESULT";
   case mrts::INVALID_INPUT_LINE:        return "INVALID_INPUT_LINE";
   }

   return "Unknown error";
}


std::string mrts::make_process_string(const mrts::process_step &step)
{
   char buffer[MRTS_LIB_PROC_MAX];
   if (step.well_formed)
   {
      char date_str[MRTS_LIB_PROC_MAX];
      strftime(date_str, sizeof(date_str), "%Y-%m-%d", &step.date);
      char time_str[MRTS_LIB_PROC_MAX];
      strftime(time_str, sizeof(time_str), "%H:%M:%S", &step.date);
      sprintf_s(buffer, sizeof(buffer), "On %s at %s by %s", date_str, time_str, step.actor.c_str());
   }
   else
   {
      sprintf_s(buffer, sizeof(buffer), "%s", step.actor.c_str());
   }

   return buffer;
}

mrts::process_step mrts::make_process_step(const std::string &actor)
{
   mrts::process_step step;
   time_t now = time(NULL);

   step.actor = actor.c_str();
   localtime_s(&step.date, &now);
   step.well_formed = true;
   step.present = true;

   return step;
}

