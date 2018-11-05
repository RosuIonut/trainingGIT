#ifndef LIB_NAMES_HPP
#define LIB_NAMES_HPP

#include <string>

namespace names
{
   // Takes an enum item name and tries to resolve it into a numeric value
   bool boFindEnumValue(const std::string &sEnum, const std::string &sName, int *piValue);
   // Takes a numeric value and tries to find an enum name for it
   bool boFindEnumName(const std::string &sEnum, int iValue, std::string *sName);
}

#endif
