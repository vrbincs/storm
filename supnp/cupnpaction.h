#ifndef CUPNPACTION_H
   #define CUPNPACTION_H

#include <string>
#include <map>
#include <rapidxml.hpp>

#include <cstring.h>

enum UPnPType
{
   UPnPType_UI4,
   UPnPType_String
};

class CUPnPActionArg
{
public:
   CStringPtr getName() const
   {
      return CStringPtr(new CString("testAction"));
   }
};

typedef std::shared_ptr<CUPnPActionArg> CUPnPActionArgPtr;

class CUPnPAction
{
public:
   CStringPtr getName() const;
   
   bool addArg(CUPnPActionArg *arg);
   
   static CUPnPAction *create();
   bool deserialize(rapidxml::xml_node<> *xmlNode);
protected:
   CUPnPAction();
private:
   CStringPtr m_name;
   std::map<CStringPtr, CUPnPActionArgPtr, CString::CStringPtrCompare> m_argumentMap;
};

#endif // CUPNPACTION_H
