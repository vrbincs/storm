#ifndef CUPNPACTION_H
   #define CUPNPACTION_H

#include <string>
#include <rapidxml.hpp>

class CUPnPActionDesc;
class CValue;

enum UPnPType
{
   UPnPType_UI4,
   UPnPType_String
};

class CUPnPAction
{
protected:
   CUPnPAction(const CUPnPActionDesc *);

   std::string getName() const;
   CValue *getValue() const;
private:
   const CUPnPActionDesc *m_upnpActionDesc;
   CValue *m_value;
};

#endif // CUPNPACTION_H
