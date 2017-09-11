#include <string>

#include <rapidxml.hpp>

#include "logger.h"
#include "cupnpactionargumentdesc.h"

CUPnPActionArgumentDesc *CUPnPActionArgumentDesc::create()
{
   return new CUPnPActionArgumentDesc();
}

void CUPnPActionArgumentDesc::destroy(CUPnPActionArgumentDesc *obj)
{
   delete obj;
}

bool CUPnPActionArgumentDesc::deserialize(rapidxml::xml_node<> *xmlNode)
{
   for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
   {
      std::string tagName = childNode->name();
      
      if(tagName == "name")
      {
         m_name = childNode->value();
      }
      else if(tagName == "direction")
      {
         if(std::string(childNode->value()) == "out")
         {
            m_isOut = true;
         }
      }
   }
   
   return true;
}

CUPnPActionArgumentDesc::CUPnPActionArgumentDesc(const std::string &name,
                                                 bool isOut)
   : m_name(name),
     m_isOut(isOut)
{
}
