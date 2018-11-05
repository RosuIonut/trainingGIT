#ifndef LIB_UTIL_HPP
#define LIB_UTIL_HPP

#include <string>
#include <vector>

#include "log.hpp"

#define ARRAY_SIZE(a)   (sizeof(a)/sizeof((a)[0]))

namespace util
{
   std::string sMakeLower(const std::string &s);
   bool boParseCommandLine(const std::string &line, std::string *cmd, std::vector<std::string> *args);
   bool boParseTraceLine(const std::string &line, std::string *type, std::vector<std::string> *args);
   const char *basename(const char *s);
   std::string get_login_name(void);
   bool boCreatePath(const std::string &usPath);
   
}

#endif
