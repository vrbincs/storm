#include <cstdlib>

#include "cstring.h"

int32_t CString::toInt32() const
{
   return atoi(c_str());
}
