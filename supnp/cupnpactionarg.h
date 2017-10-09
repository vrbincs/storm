#ifndef CUPNPACTIONARG_H
   #define CUPNPACTIONARG_H

#include <memory>
#include <cstring.h>

#include <rapidxml/rapidxml.hpp>

class CUPnPActionArg;
typedef std::shared_ptr<CUPnPActionArg> CUPnPActionArgPtr;

class CUPnPActionArg
{
public:
   CUPnPActionArg();
   
   inline CStringPtr getName() const
   {
      return m_name;
   }
   
   static CUPnPActionArg *create();
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   
   CStringPtr m_name;
   CStringPtr m_stateVar;
   bool m_isOut;
};

#endif // CUPNPACTIONARG_H
