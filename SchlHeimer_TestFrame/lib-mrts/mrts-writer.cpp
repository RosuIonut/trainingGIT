#include <ctime>
#include <algorithm>
#include <sstream>

#include "mrts-writer.hpp"

namespace mrts
{

writer::writer()
{
}

writer::~writer()
{
}

bool writer::write(std::ofstream &out, const document &doc)
{
   out << "# File written by MRTS-lib " <<
      MRTS_LIB_MAJOR_VER << "." << MRTS_LIB_MINOR_VER << std::endl;

   print_doc(out, doc);

   return true;
}

void writer::print_doc(std::ofstream &out, const document &doc)
{
   out << "Begin Testspec" << std::endl;
   out << "Format: MRTSF " << MRTS_LIB_MAJOR_VER << std::endl;
   out << "Filename: " << doc.filename << std::endl;
   out << "Revision: " << doc.revision << std::endl;
   out << "Extracted:";
   if (doc.extracted.present)
      print_process_step(out, doc.extracted);
   out << std::endl;
   out << "Annotated:";
   if (doc.annotated.present)
      print_process_step(out, doc.annotated);
   out << std::endl;
   out << "Evaluated:";
   if (doc.evaluated.present)
      print_process_step(out, doc.evaluated);
   out << std::endl;

   print_tests(out, doc.tests);

   out << "End Testspec" << std::endl;
}

void writer::print_process_step(std::ofstream &out, const process_step &s)
{
   if (s.well_formed)
   {
      char buf[100];
      strftime(buf, sizeof(buf), " on %Y-%m-%d at %H:%M:%S by ", &s.date);
      out << buf << s.actor;
   }
   else
   {
      out << " " << s.actor;
   }
}

void writer::print_tests(std::ofstream &out, const std::vector<test> &tests)
{
   for (std::vector<test>::const_iterator it = tests.begin(); it != tests.end(); it++)
   {
      out << std::endl;

      std::stringstream title;
      title << it->id << ": " << it->name;
      print_header_comment(out, '=', title.str(), MRTS_LIB_TITLE_MAX, MRTS_LIB_LINE_MAX);

      out << "Begin Test " << it->id << std::endl;
      out << "Name: " << it->name << std::endl;

      print_test_cases(out, it->test_cases);

      out << "End Test" << std::endl;
      print_header_comment(out, '=', MRTS_LIB_LINE_MAX);
   }
}

void writer::print_test_cases(std::ofstream &out, const std::vector<test_case> &test_cases)
{
   for (std::vector<test_case>::const_iterator it = test_cases.begin(); it != test_cases.end(); it++)
   {
      out << std::endl;

      std::stringstream title;
      title << it->id << ": " << it->name;
      print_header_comment(out, '-', title.str(), MRTS_LIB_TITLE_MAX, MRTS_LIB_LINE_MAX);

      out << "Begin Testcase " << it->id << std::endl;
      out << "Name: " << it->name << std::endl;
      out << "Prio: " << it->prio << std::endl;
      out << std::endl;

      out << "Begin Input" << std::endl;
      print_string_list(out, it->input);
      out << "End Input" << std::endl;
      out << "Begin Expected" << std::endl;
      print_string_list(out, it->expected);
      out << "End Expected" << std::endl;
      out << "Begin Output" << std::endl;
      print_string_list(out, it->output);
      out << "End Output" << std::endl;

      out << std::endl;
      print_test_result(out, it->result);

      out << "End Testcase" << std::endl;
      print_header_comment(out, '-', MRTS_LIB_LINE_MAX);
   }
}

void writer::print_string_list(std::ofstream &out, const std::vector<std::string> &list)
{
   for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); it++)
      out << *it << std::endl;
}

void writer::print_test_result(std::ofstream &out, const test_result &result)
{
   bool allow_comment = false;

   out << "Result:";
   switch (result.code)
   {
   case FAILED:
      out << " FAILED";
      allow_comment = true;
      break;
   case PASSED:
      out << " PASSED";
      allow_comment = true;
      break;
   }

   if (allow_comment && (result.comment.length() > 0))
      out << ", " << result.comment;

   out << std::endl;
}

void writer::print_header_comment(std::ofstream &out, char fillchar, int max_line)
{
   print_header_comment(out, fillchar, "", 0, max_line);
}

void writer::print_header_comment(std::ofstream &out, char fillchar, const std::string &title, int max_title, int max_line)
{
   int title_len = std::min((int) title.length(), max_title);
   int fill_len = max_line - title_len - 2;

   out << "#";
   while (fill_len-- > 0)
      out << fillchar;

   if  (title_len > 0)
   {
      out << " ";
      std::string::const_iterator it = title.begin();
      while (title_len-- > 0)
         out << *it++;
   }
   else
   {
      out << fillchar;
   }

   out << std::endl;
}

}
