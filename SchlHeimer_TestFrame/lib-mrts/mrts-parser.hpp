#ifndef MRTS_PARSER_HPP
#define MRTS_PARSER_HPP

#include <fstream>

#include "lib-mrts.hpp"

namespace mrts
{

class parser
{
public:
   parser();
   virtual ~parser();

   bool parse(std::ifstream &in, document *out, fail_info *fail);

private:

   enum block
   {
      BL_TESTSPEC,
      BL_TEST,
      BL_TESTCASE,
      BL_INPUT,
      BL_EXPECTED,
      BL_OUTPUT,
      BL_NUMBER_OF_BLOCKS,
   };

   enum attribute
   {
      AT_FORMAT,
      AT_FILENAME,
      AT_REVISION,
      AT_EXTRACTED,
      AT_ANNOTATED,
      AT_EVALUATED,
      AT_NAME,
      AT_PRIO,
      AT_RESULT,
      AT_NUMBER_OF_ATTRIBUTES,
   };

   struct pieces_attr
   {
      bool valid;
      attribute key;
      std::string value;
      std::vector<std::string> orig;
      std::vector<std::string> lower;
   };

   struct pieces
   {
      std::string line;
      std::vector<std::string> orig;
      std::vector<std::string> lower;
      pieces_attr attr;
   };

   void split(const std::string &line, std::vector<std::string> *tokens);
   void parse_line(const std::string &line, pieces *p);
   bool parse_attribute(const std::string &line, pieces_attr *a);
   bool parse_process_step(const std::string &in, struct tm *out_tm, std::string *out_actor);

   bool is_empty(const std::string &line);
   bool is_comment(const std::string &line);
   bool is_in_free_text_block(const pieces &p, block *b);
   bool is_matching_end(const pieces &p, block b);
   block block_from_name(const std::string &name);
   attribute attribute_from_name(const std::string &name);
   bool is_begin(const pieces &p, block *b);
   bool is_end(const pieces &p, block *b);
   void enter_block(const pieces &p, block b);
   void leave_block (const pieces &p, block b, document *d);
   void on_attribute(const pieces &p, const attribute a);
   void on_misc_line(const pieces &p);

   void init_doc(document *d);
   void init_test(test *t, const std::string &id);
   void init_test_case(test_case *tc, const std::string &id);

   bool insert_date_string(const std::string &in, struct tm *out);
   bool insert_time_string(const std::string &in, struct tm *out);

   void insert_doc(const document &in, document *out);
   void insert_test(const test &in, document *out);
   void insert_test_case(const test_case &in, test *out);
   void insert_process_step(const std::string &in, process_step *out);
   void insert_test_result(const std::string &in, test_result *out);


   fail_info error(fail_reason reason);
   fail_info error(fail_reason reason, const std::string &word);

   std::string make_lower(const std::string &s);

   bool m_active_block[BL_NUMBER_OF_BLOCKS];
   std::vector<std::string> m_curr_string_list;
   document m_curr_doc;
   test m_curr_test;
   test_case m_curr_test_case;
};

}

#endif
