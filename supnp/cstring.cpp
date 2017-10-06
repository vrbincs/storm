#include <cstdlib>

#include "logger.h"
#include "cstring.h"

int32_t CString::toInt32() const
{
   return atoi(c_str());
}

CString *CString::create(const std::string &str)
{
   return new CString(str);
}

bool CString::CStringPtrCompare::operator()(const CStringPtr &strPtr1,
                                            const CStringPtr &strPtr2)
{
   if(*strPtr1 < *strPtr2)
   {
      return true;
   }
   
   return false;
}
