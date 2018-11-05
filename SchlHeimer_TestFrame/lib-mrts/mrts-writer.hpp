#ifndef MRTS_WRITER_HPP
#define MRTS_WRITER_HPP

#include <fstream>

#include "lib-mrts.hpp"

namespace mrts
{

class writer
{
public:
   writer();
   ~writer();

   bool write(std::ofstream &out, const document &doc);

private:
   void print_doc(std::ofstream &out, const document &doc);
   void print_process_step(std::ofstream &out, const process_step &s);
   void print_tests(std::ofstream &out, const std::vector<test> &tests);
   void print_test_cases(std::ofstream &out, const std::vector<test_case> &test_cases);
   void print_string_list(std::ofstream &out, const std::vector<std::string> &list);
   void print_test_result(std::ofstream &out, const test_result &result);
   void print_header_comment(std::ofstream &out, char fillchar, int max_line);
   void print_header_comment(std::ofstream &out, char fillchar, const std::string &title, int max_title, int max_line);
};

}

#endif
