#include <string>

#include <rapidxml.hpp>

#include "logger.h"
#include "cupnpactionargumentdesc.h"
#include "cupnpactiondesc.h"

CUPnPActionDesc *CUPnPActionDesc::create()
{
   return new CUPnPActionDesc();
}
   
void CUPnPActionDesc::destroy(CUPnPActionDesc *obj)
{
   delete obj;
}
   
bool CUPnPActionDesc::addArg(CUPnPActionArgumentDesc *arg)
{
   if(arg)
   {
      if(m_argumentMap.find(arg->getName()) == m_argumentMap.end())
      {
         m_argumentMap.insert(std::pair<std::string, CUPnPActionArgumentDesc *>(arg->getName(), arg) );
         return true;
      }
      else
      {
         LOGGER_WARN("The argument has already been registered. argName=" << arg->getName());
         delete arg;
         return false;
      }
   }
   else
   {
      LOGGER_WARN("Unable to add NULL argument.");
      return false;
   }
}

bool CUPnPActionDesc::deserialize(rapidxml::xml_node<> *xmlNode)
{
   for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
   {
      std::string tagName = childNode->name();
      
      if(tagName == "name")
      {
         setActionName(childNode->value());
      }
      else if(tagName == "argumentList")
      {
         for (rapidxml::xml_node<> *childNodeArg = childNode->first_node(); childNodeArg; childNodeArg = childNodeArg->next_sibling())
         {
            std::string tagName = childNodeArg->name();
            
            if(tagName == "argument")
            {
               CUPnPActionArgumentDesc *arg = CUPnPActionArgumentDesc::create();
               arg->deserialize(childNodeArg);
               addArg(arg);
            }
            else
            {
               LOGGER_WARN("Unkown XML tag. tag=" << tagName);
            }
         }
      }
   }
   
   return true;
}
   
const CUPnPActionArgumentDesc *CUPnPActionDesc::findArg(const std::string &argName) const
{
   auto it_arg = m_argumentMap.find(argName);
   
   if(it_arg == m_argumentMap.end())
   {
      return NULL;
   }
   else
   {
      return it_arg->second;
   }
}

CUPnPActionDesc::CUPnPActionDesc(const std::string &actionName)
   : m_actionName(actionName)
{
}
