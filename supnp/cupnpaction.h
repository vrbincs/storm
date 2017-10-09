#ifndef CUPNPACTION_H
   #define CUPNPACTION_H

#include <string>
#include <map>
#include <rapidxml.hpp>

#include <cstring.h>
#include <cupnpactionarg.h>

enum UPnPType
{
   UPnPType_UI4,
   UPnPType_String
};

class CUPnPAction
{
public:
   CStringPtr getName() const;
   
   bool addArg(const CUPnPActionArgPtr &arg);
   
   static CUPnPAction *create();
   bool deserialize(rapidxml::xml_node<> *xmlNode);
protected:
   CUPnPAction();
private:
   CStringPtr m_name;
   std::map<CStringPtr, CUPnPActionArgPtr, CString::CStringPtrCompare> m_argumentMap;
};

#endif // CUPNPACTION_H
