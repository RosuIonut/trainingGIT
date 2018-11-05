#include <cassert>
#include <string>
#include <cctype>

#include "trace.hpp"

#include "mrts-parser.hpp"


namespace mrts
{

parser::parser()
{
   for (int i = 0; i < BL_NUMBER_OF_BLOCKS; i++)
      m_active_block[i] = false;
}

parser::~parser()
{
}

bool parser::parse(std::ifstream &in, document *out, fail_info *fail)
{
   assert(out != NULL);
   assert(fail != NULL);

   fail->line_no = 0;

   try
   {
      while (in.good())
      {
         std::string line;
         pieces p;

         getline(in, line);
         fail->line_no++;
         fail->line_text = line;

         if (is_empty(line))
            continue;
        if (is_comment(line))
        {
           m_curr_string_list.push_back(line);  // Take line in original
           continue;
        }

         parse_line(line, &p);

         block b;
         if (is_in_free_text_block(p, &b))
         {
            if (is_matching_end(p, b) == false)
            {
               // Take line in original form for Input/Expected/Output blocks
               m_curr_string_list.push_back(p.line);
               continue;
            }
         }

         if (is_begin(p, &b))
         {
            enter_block(p, b);
            continue;
         }
         if (is_end(p, &b))
         {
            leave_block(p, b, out);
            continue;
         }

         if (p.attr.valid)
         {
            on_attribute(p, p.attr.key);
            continue;
         }

         on_misc_line(p);
      }
   }
   catch (fail_info info)
   {
      fail->reason = info.reason;
      fail->word = info.word;

      TRACE("*** Exception caught: reason = %i, line_no = %i, line_text = <%s> ***\n",
         fail->reason, fail->line_no, fail->line_text.c_str());

      return false;
   }

   return true;
}

void parser::split(const std::string &line, std::vector<std::string> *tokens)
{
   bool in_word = false;
   std::string word;

   assert(tokens != NULL);

   for (std::string::const_iterator it = line.begin(); it != line.end(); it++)
   {
      if (in_word)
      {
         // in word
         if (isspace((unsigned char) *it))
         {
            // found space => word is finished
            in_word = false;
            tokens->push_back(word);
            word.clear();
         }
         else
         {
            // found char => add to word
            word.push_back(*it);
         }
      }
      else
      {
         // not in word
         if (isspace((unsigned char) *it))
         {
            // found space => stay out of word
         }
         else
         {
            // found char => now in word
            in_word = true;
            word.push_back(*it);
         }
      }
   }
   // save last piece, too
   if (in_word)
      tokens->push_back(word);
}

void parser::parse_line(const std::string &line, pieces *p)
{
   assert(p != NULL);

   p->line = line;
   p->lower.clear();
   p->orig.clear();

   std::vector<std::string> tokens;
   split(line, &tokens);
   for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
   {
      p->orig.push_back(*it);
      p->lower.push_back(make_lower(*it));
   }

   p->attr.valid = parse_attribute(line, &p->attr);
}

bool parser::parse_attribute(const std::string &line, pieces_attr *a)
{
   assert(a != NULL);

   a->value.clear();
   a->lower.clear();
   a->orig.clear();

   // Text within these blocks is free text and is not analzyed
   if (m_active_block[BL_INPUT] || m_active_block[BL_EXPECTED] ||m_active_block[BL_OUTPUT])
      return false;

   std::string key_str;

   std::string::const_iterator it = line.begin();
   // skip heading whitespace
   while (it != line.end() && isspace((unsigned char) *it))
      it++;
   // take everything up to colon
   while (it != line.end() && (*it != ':'))
      key_str.push_back(*it++);
   // this must be a colon, otherwise this is no attribute
   if (it == line.end() || (*it != ':'))
      return false;
   // get attribute as enum
   a->key = attribute_from_name(make_lower(key_str));
   // skip colon, skip whitespace
   it++;
   while (it != line.end() && isspace((unsigned char) *it))
      it++;
   // rest is value
   while (it != line.end())
      a->value.push_back(*it++);

   // also split the value into tokens
   std::vector<std::string> tokens;
   split(a->value, &tokens);
   for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
   {
      a->orig.push_back(*it);
      a->lower.push_back(make_lower(*it));
   }

   return true;
}

bool parser::is_empty(const std::string &line)
{
   for (std::string::const_iterator it = line.begin(); it != line.end(); it++)
   {
      if (isspace((unsigned char) *it))
         continue;

      // found a non-white space, so it is not empty
      return false;
   }
   return true;
}

bool parser::is_comment(const std::string &line)
{
   for (std::string::const_iterator it = line.begin(); it != line.end(); it++)
   {
      if (isspace((unsigned char) *it))
         continue;
      if (*it == '#')
         return true;
      return false;
   }
   return false;
}

bool parser::is_in_free_text_block(const pieces &p, block *b)
{
   assert(b != NULL);

   if (m_active_block[BL_INPUT])
   {
      *b = BL_INPUT;
      return true;
   }

   if (m_active_block[BL_EXPECTED])
   {
      *b = BL_EXPECTED;
      return true;
   }

   if (m_active_block[BL_OUTPUT])
   {
      *b = BL_OUTPUT;
      return true;
   }

   return false;
}

bool parser::is_matching_end(const pieces &p, block b)
{
   if (p.lower[0].compare("end") != 0)
      return false;

   switch (b)
   {
   case BL_INPUT:
      return p.lower[1].compare("input") == 0;
   case BL_EXPECTED:
      return p.lower[1].compare("expected") == 0;
   case BL_OUTPUT:
      return p.lower[1].compare("output") == 0;
   }

   return false;
}

parser::block parser::block_from_name(const std::string &name)
{
   if (name.compare("testspec") == 0)
      return BL_TESTSPEC;
   if (name.compare("test") == 0)
      return BL_TEST;
   if (name.compare("testcase") == 0)
      return BL_TESTCASE;
   if (name.compare("input") == 0)
      return BL_INPUT;
   if (name.compare("expected") == 0)
      return BL_EXPECTED;
   if (name.compare("output") == 0)
      return BL_OUTPUT;

   throw error(INVALID_BLOCK_NAME, name);
}

parser::attribute parser::attribute_from_name(const std::string &name)
{
   if (name.compare("format") == 0)
      return AT_FORMAT;
   if (name.compare("filename") == 0)
      return AT_FILENAME;
   if (name.compare("revision") == 0)
      return AT_REVISION;
   if (name.compare("extracted") == 0)
      return AT_EXTRACTED;
   if (name.compare("annotated") == 0)
      return AT_ANNOTATED;
   if (name.compare("evaluated") == 0)
      return AT_EVALUATED;
   if (name.compare("name") == 0)
      return AT_NAME;
   if (name.compare("prio") == 0)
      return AT_PRIO;
   if (name.compare("result") == 0)
      return AT_RESULT;

   throw error(INVALID_ATTRIBUTE_NAME, name);
}

bool parser::is_begin(const pieces &p, block *b)
{
   assert(b != NULL);

   if (p.orig.size() >= 2)
   {
      if (p.lower[0].compare("begin") == 0)
      {
         *b = block_from_name(p.lower[1]);
         return true;
      }
   }

   return false;
}

bool parser::is_end(const pieces &p, block *b)
{
   assert(b != NULL);

   if (p.orig.size() == 2)
   {
      if (p.lower[0].compare("end") == 0)
      {
         *b = block_from_name(p.lower[1]);
         return true;
      }
   }

   return false;
}

void parser::enter_block(const pieces &p, block b)
{
   assert(b >= 0 && b <= BL_NUMBER_OF_BLOCKS);

   if (m_active_block[b])
      throw error(NESTED_BLOCK_FOUND, p.orig[1].c_str());
   m_active_block[b] = true;

   switch (b)
   {
   case BL_TESTSPEC:
      init_doc(&m_curr_doc);
      break;
   case BL_TEST:
      if (p.orig.size() < 3)
         throw error(MALFORMED_BEGIN_TEST);
      init_test(&m_curr_test, p.orig[2]);
      break;
   case BL_TESTCASE:
      if (p.orig.size() < 3)
         throw error(MALFORMED_BEGIN_TEST_CASE);
      init_test_case(&m_curr_test_case, p.orig[2]);
   case BL_INPUT:
   case BL_EXPECTED:
   case BL_OUTPUT:
      m_curr_string_list.clear();
      break;
   default:
      throw error(INVALID_BLOCK_NAME);
   }

   TRACE("entered \"%s\"\n", p.lower[1].c_str());
}

void parser::leave_block(const pieces &p, block b, document *d)
{
   assert(b >= 0 && b <= BL_NUMBER_OF_BLOCKS);

   if (!m_active_block[b])
      throw error(TOO_MANY_BLOCK_ENDS, p.orig[1].c_str());
   m_active_block[b] = false;

   switch (b)
   {
   case BL_TESTSPEC:
      insert_doc(m_curr_doc, d);
      break;
   case BL_TEST:
      insert_test(m_curr_test, &m_curr_doc);
      break;
   case BL_TESTCASE:
      insert_test_case(m_curr_test_case, &m_curr_test);
      break;
   case BL_INPUT:
      // block "test case" must be active
      assert(m_active_block[BL_TESTCASE]);
      m_curr_test_case.input = m_curr_string_list;
      break;
   case BL_EXPECTED:
      // block "test case" must be active
      assert(m_active_block[BL_TESTCASE]);
      m_curr_test_case.expected = m_curr_string_list;
      break;
   case BL_OUTPUT:
      // block "test case" must be active
      assert(m_active_block[BL_TESTCASE]);
      m_curr_test_case.output = m_curr_string_list;
      break;
   default:
      throw error(INVALID_BLOCK_NAME);
   }

   TRACE("left \"%s\"\n", p.lower[1].c_str());
}

fail_info parser::error(fail_reason reason)
{
   return error(reason, "");
}

fail_info parser::error(fail_reason reason, const std::string &word)
{
   fail_info info;

   info.reason = reason;
   info.word = word;

   return info;
}

void init_process_step(process_step *s)
{
   assert(s != NULL);

   s->present = false;
   s->well_formed = false;
   s->actor.clear();
}

void parser::init_doc(document *d)
{
   assert(d != NULL);

   d->format.clear();
   d->filename.clear();
   d->revision.clear();
   init_process_step(&d->extracted);
   init_process_step(&d->annotated);
   init_process_step(&d->evaluated);
   d->tests.clear();
}

void parser::init_test(test *t, const std::string &id)
{
   assert(t != NULL);

   t->id = id;
   t->name.clear();
   t->test_cases.clear();
}

void parser::init_test_case(test_case *tc, const std::string &id)
{
   assert(tc != NULL);

   tc->id = id;
   tc->name.clear();
   tc->prio = -1;
   tc->input.clear();
   tc->expected.clear();
   tc->output.clear();
   tc->result.code = NOT_TESTED;
   tc->result.comment.clear();
}

std::string parser::make_lower(const std::string &s)
{
   std::string result;

   for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
      result.push_back(tolower(*it));

   return result;
}

void parser::insert_doc(const document &in, document *out)
{
   assert(out);
   *out = in;
}

void parser::insert_test(const test &in, document *out)
{
   assert(out);
   out->tests.push_back(in);
}

void parser::insert_test_case(const test_case &in, test *out)
{
   assert(out);
   out->test_cases.push_back(in);
}

void parser::on_attribute(const pieces &p, const attribute a)
{
   TRACE("found attribute %i (\"%s\")\n", a, p.lower[0].c_str());

   switch (a)
   {
   case AT_FORMAT:
      assert(m_active_block[BL_TESTSPEC]);
      m_curr_doc.format = p.attr.value;
      break;
   case AT_FILENAME:
      assert(m_active_block[BL_TESTSPEC]);
      m_curr_doc.filename = p.attr.value;
      break;
   case AT_REVISION:
      assert(m_active_block[BL_TESTSPEC]);
      m_curr_doc.revision = p.attr.value;
      break;
   case AT_EXTRACTED:
      assert(m_active_block[BL_TESTSPEC]);
      insert_process_step(p.attr.value, &m_curr_doc.extracted);
      break;
   case AT_ANNOTATED:
      assert(m_active_block[BL_TESTSPEC]);
      insert_process_step(p.attr.value, &m_curr_doc.annotated);
      break;
   case AT_EVALUATED:
      assert(m_active_block[BL_TESTSPEC]);
      insert_process_step(p.attr.value, &m_curr_doc.evaluated);
      break;
   case AT_NAME:
      // Name can be in test or in test case
      if (m_active_block[BL_TESTCASE])
      {
         m_curr_test_case.name = p.attr.value;
      }
      else if (m_active_block[BL_TEST])
      {
         m_curr_test.name = p.attr.value;
      }
      else
      {
         // Neither test nor test case active
         assert(0);
      }
      break;
   case AT_PRIO:
      assert(m_active_block[BL_TESTCASE]);
      m_curr_test_case.prio = strtoul(p.attr.value.c_str(), NULL, 0);
      break;
   case AT_RESULT:
      assert(m_active_block[BL_TESTCASE]);
      insert_test_result(p.attr.value, &m_curr_test_case.result);
      break;
   default:
      throw error(INVALID_ATTRIBUTE_NAME);
   }
}

void parser::on_misc_line(const pieces &p)
{
   // Unknown misc line without use!
   throw error(INVALID_INPUT_LINE);
}

bool parser::insert_date_string(const std::string &in, struct tm *out)
{
   std::string year;
   std::string month;
   std::string day;

   std::string::const_iterator it = in.begin();

   // YYYY-MM-DD
   // ^^^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      year.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      year.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      year.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      year.push_back(*it++);
   // YYYY-MM-DD
   //     ^
   if ((it == in.end()) || (*it != '-'))
      return false;
   it++;
   // YYYY-MM-DD
   //      ^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      month.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      month.push_back(*it++);
   // YYYY-MM-DD
   //        ^
   if ((it == in.end()) || (*it != '-'))
      return false;
   it++;
   // YYYY-MM-DD
   //         ^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      day.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      day.push_back(*it++);

   out->tm_year = strtoul(year.c_str(), NULL, 10) - 1900;
   out->tm_mon = strtoul(month.c_str(), NULL, 10) - 1;
   out->tm_mday = strtoul(day.c_str(), NULL, 10);

   return true;
}

bool parser::insert_time_string(const std::string &in, struct tm *out)
{
   std::string hour;
   std::string minute;
   std::string second;

   std::string::const_iterator it = in.begin();

   // HH:MM:SS
   // ^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      hour.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      hour.push_back(*it++);
   // HH:MM:SS
   //   ^
   if ((it == in.end()) || (*it != ':'))
      return false;
   it++;
   // HH:MM:SS
   //    ^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      minute.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      minute.push_back(*it++);
   // HH:MM:SS
   //      ^
   if ((it == in.end()) || (*it != ':'))
      return false;
   it++;
   // HH:MM:SS
   //       ^^
   if ((it != in.end()) && isdigit((unsigned char) *it))
      second.push_back(*it++);
   if ((it != in.end()) && isdigit((unsigned char) *it))
      second.push_back(*it++);

   out->tm_hour = strtoul(hour.c_str(), NULL, 10);
   out->tm_min = strtoul(minute.c_str(), NULL, 10);
   out->tm_sec = strtoul(second.c_str(), NULL, 10);

   return true;
}

bool parser::parse_process_step(const std::string &in, struct tm *out_tm, std::string *out_actor)
{
   std::vector<std::string> tokens;
   split(in, &tokens);

   // Common format has at least 6 items
   if (tokens.size() < 6)
      return false;

   // Check if separators are okay
   if (make_lower(tokens[0]).compare("on") ||
      make_lower(tokens[2]).compare("at") ||
      make_lower(tokens[4]).compare("by"))
   {
      return false;
   }

   bool result = true;
   result = result && insert_date_string(tokens[1], out_tm);
   result = result && insert_time_string(tokens[3], out_tm);
   *out_actor = in.substr(in.find("by") + 3);

   // Use mktime to automatically calculate wday, yday and isdst
   mktime(out_tm);

   return result;
}

void parser::insert_process_step(const std::string &in, process_step *out)
{
   assert(out != NULL);

   if (!is_empty(in))
   {
      // Attribute is set
      out->present = true;

      struct tm date;
      std::string actor;
      // Our special syntax?
      if (parse_process_step(in, &date, &actor))
      {
         // Yes, save details
         out->well_formed = true;
         out->actor = actor;
         out->date = date;
      }
      else
      {
         // No, store everything as actor
         out->well_formed = false;
         out->actor = in;
      }
   }
   else
   {
      // Attribute is unset
      out->present = false;
   }
}

void parser::insert_test_result(const std::string &in, test_result *out)
{
   assert(out != NULL);

   std::string result_str;

   std::string::const_iterator it = in.begin();
   // take everything up to end of string or comma
   while (it != in.end() && (*it != ','))
      result_str.push_back(tolower(*it++));
   // skip comma (if any)
   if (it != in.end() && (*it == ','))
      it++;
   // skip whitespace (if any)
   while (it != in.end() && isspace((unsigned char) *it))
      it++;
   // rest is comment (if any)
   while (it != in.end())
      out->comment.push_back(*it++);

   // check result class
   if (result_str.compare("passed") == 0)
      out->code = PASSED;
   else if (result_str.compare("failed") == 0)
      out->code = FAILED;
   else if (result_str.length() == 0)
      out->code = NOT_TESTED;
   else
      throw error(UNCLEAR_TEST_RESULT);
}

}
