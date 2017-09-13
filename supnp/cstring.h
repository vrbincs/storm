#ifndef CSTRING_H
   #define CSTRING_H

#include <string>

class CString : public std::string
{
public:
   CString(const char *str = "") : std::string(str) {}
   CString(const std::string &str) : std::string(str) {}
   
   int32_t toInt32() const;
};

#endif // CSTRING_H
