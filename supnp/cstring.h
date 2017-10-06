#ifndef CSTRING_H
   #define CSTRING_H

#include <string>
#include <memory>
#include <iostream>

class CString;
typedef std::shared_ptr<CString> CStringPtr;

class CString : public std::string
{
public:
   CString(const char *str = "") : std::string(str) {}
   CString(const std::string &str) : std::string(str) {}
   
   int32_t toInt32() const;
   
   static CString *create(const std::string &str);
   
   class CStringPtrCompare
   {
   public:
      bool operator()(const CStringPtr &strPtr1, const CStringPtr &strPtr2);
   };
};


#endif // CSTRING_H
