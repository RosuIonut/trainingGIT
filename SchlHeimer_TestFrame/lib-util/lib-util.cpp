#include <cassert>
#include <windows.h>

#include "lib-util.hpp"

namespace util
{
   std::string util::sMakeLower(const std::string &s)
   {
      std::string result;

      for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
         result.push_back(tolower(*it));

      return result;
   }

   // Parse a line with Input or Expected syntax
   bool boParseCommandLine(const std::string &line, std::string *cmd, std::vector<std::string> *args)
   {
       const char *p = line.c_str();

       // Skip white space at beginning of line             // 20170302 wti
       while (*p && isspace(*p))                            // 20170302 wti
           p++;                                             // 20170302 wti
       if ('#' == *p)                                       // 20170307 wti
       {                                                    // 20170307 wti
           cmd->push_back(*p);                              // 20170307 wti
       }                                                    // 20170307 wti
       else                                                 // 20170307 wti
       {                                                    // 20170307 wti
           // Get command (token before opening bracket)
           while (*p && *p != '(' && !isspace(*p))
           {
               cmd->push_back(*p);
               p++;
           }
       }                                                    // 20170307 wti
      // Skip white space between command and opening bracket
      while (*p && isspace(*p))
         p++;
      if (*p == '(')
      {
         p++;
         std::string arg;
         // Get arguments (tokens before closing bracket)
         while (*p && *p != ')')
         {
            // Skip separating whitespace and comma
            while (*p && (isspace(*p) || *p == ','))
               p++;
            arg.clear();
            // Read argument: characters up to the next space,
            // comma or closing bracket
            while (*p && !isspace(*p) && *p != ',' && *p != ')')
            {
               arg.push_back(*p);
               p++;
            }
            if (!arg.empty())
               args->push_back(arg);
         }

         if (*p != ')')
         {
            // Some malformed input?
            return false;
         }
      }
      else
      {
         // Its not a command with bracket!
         while (*p && (isspace(*p) || *p == ','))
            p++;
         std::string arg;
         while (*p)
         {
            arg.push_back(*p);
            p++;
         }
         args->push_back(arg);
      }

      return true;
   }

   bool boParseTraceLine(const std::string &line, std::string *type, std::vector<std::string> *args)
   {
      // Format of trace line:
      // Type = Name, Arg1 = Value1, Arg2 = Value2, ...

      const char *p = line.c_str();
      // skip leading white space of type
      while (*p && isspace(*p))
         p++;
      // get type
      while (*p && !isspace(*p))
         type->push_back(*p++);
      // skip trailing white space of type
      while (*p && isspace(*p))
         p++;
      // should now be '=', or syntax error
      if (*p != '=')
         return false;
      // skip '='
      p++;
      // skip leading white space of name
      while (*p && isspace(*p))
         p++;
      // get name
      std::string name;
      while (*p && !isspace(*p) && *p != ',')
         name.push_back(*p++);
      // name is args[0]
      args->push_back(name);
      // skip stuff after name
      while (*p && (isspace(*p) || (*p == ',')))
         p++;
      // end reached? Event without arguments
      if (*p == '\0')
         return true;
      // Get all argument values
      while (*p)
      {
         std::string arg;
         // Skip "Arg =" part
         while (*p && *p != '=')
            p++;
         // Skip '=' and leading white space of arg
         while (*p && ((*p == '=') || isspace(*p)))
            p++;
         // Everything is arg, until EOL, white space or comma
         while (*p && !isspace(*p) && (*p != ','))
            arg.push_back(*p++);
         if (arg.length() > 0)
            args->push_back(arg);
      }

      return true;
   }

   const char *basename(const char *s)
   {
      const char *p = strrchr(s, '\\');
      return p ? (p + 1) : s;
   }

   std::string get_login_name(void)
   {
      char name[1024];
      DWORD size = sizeof(name);
      GetUserName(name, &size);
      return name;
   }

   
   bool boCreatePath(const std::string &usPath)
   {
      std::string usCurrPath;
      std::string::const_iterator it = usPath.begin();

      bool boDone = false;
      bool boFailed = false;
      while (!boDone && !boFailed)
      {
         if (it == usPath.end() || *it == '/' || *it == '\\')
         {
            if (GetFileAttributes(usCurrPath.c_str()) == INVALID_FILE_ATTRIBUTES)
            {
               if (CreateDirectory(usCurrPath.c_str(), NULL) == FALSE)
               {
                  boFailed = true;
               }
            }
         }
         if (it == usPath.end())
            boDone = true;
         else
            usCurrPath.push_back(*it++);
      }
      
      return !boFailed;
   }

}
