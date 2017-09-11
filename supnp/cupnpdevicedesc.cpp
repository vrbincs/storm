#include <string>
#include <map>

#include <rapidxml.hpp>

#include "logger.h"

#include "cupnpactionargumentdesc.h"
#include "cupnpactiondesc.h"
#include "cupnpdevicedesc.h"

static bool deserializeActions(rapidxml::xml_node<> *xmlNode, 
                               std::map<std::string, CUPnPActionDesc *> &actionList)
{
   for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
   {
      std::string tagName = childNode->name();
      
      if(tagName == "action")
      {
         CUPnPActionDesc *action = CUPnPActionDesc::create();
         
         if(action->deserialize(childNode))
         {
            actionList.insert(std::pair<std::string, CUPnPActionDesc *>(action->getName(), action));
         }
         else
         {
            CUPnPActionDesc::destroy(action);
            LOGGER_ERROR("Unable to deserialize action.");
            return false;
         }
      }
      else
      {
         LOGGER_ERROR("XML structure not valid.");
         return false;
      }
   }
   
   return true;
}


CUPnPDeviceDesc::CUPnPDeviceDesc(const std::string &deviceType)
   : m_deviceType(deviceType)
{
}

CUPnPDeviceDesc::~CUPnPDeviceDesc()
{
   
}

bool CUPnPDeviceDesc::addActionDesc(CUPnPActionDesc *serviceDesc)
{
   return false;
}

bool CUPnPDeviceDesc::deserialize(rapidxml::xml_node<> *xmlNode)
{
   if(xmlNode)
   {
      try
      {
         for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
         {
            std::string tagName = childNode->name();
            
            if(tagName == "specVersion")
            {
            }
            else if(tagName == "actionList")
            {
               deserializeActions(childNode, m_actionList);
            }
            else if(tagName == "serviceStateTable")
            {
            }
         }
         
         return true;
      }
      catch(const rapidxml::parse_error &err)
      {
         LOGGER_ERROR("Error while parsing XML. what=" << err.what());
      }
   }
   else
   {
      LOGGER_ERROR("Cannot serialize XML on NULL object");
   }
   
   return false;
}

const CUPnPActionDesc *CUPnPDeviceDesc::findAction(const std::string &actionName) const
{
   auto action = m_actionList.find(actionName);
   
   if(action == m_actionList.end())
   {
      return NULL;
   }
   else
   {
      return action->second;
   }
}

std::vector<CUPnPActionDesc *> CUPnPDeviceDesc::getActionList() const
{
   std::vector<CUPnPActionDesc *> t_actionList;
   
   t_actionList.reserve(m_actionList.size());
   auto it = m_actionList.begin();
   
   for(;it != m_actionList.end(); it++)
   {
      t_actionList.push_back(it->second);
   }
   
   return t_actionList;
}
