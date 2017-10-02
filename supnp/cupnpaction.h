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
public:
   static CUPnPAction *create();
   bool deserialize(rapidxml::xml_node<> *xmlNode);
protected:
   CUPnPAction(const CUPnPActionDesc * = NULL);

   std::string getName() const;
   CValue *getValue() const;
private:
   const CUPnPActionDesc *m_upnpActionDesc;
   CValue *m_value;
   
   std::string m_name;
};

#endif // CUPNPACTION_H
