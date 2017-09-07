#include <rapidxml.hpp>

#include "logger.h"

#include "cupnpdevicedesc.h"


static CUPnPActionArgumentDesc *deserializeArgumentList(rapidxml::xml_node<> *xmlNode)
{
   return NULL;
}

static CUPnPActionDesc *deserializeAction(rapidxml::xml_node<> *xmlNode)
{
   std::string actionName;
   
   for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
   {
      std::string tagName = childNode->name();
      
      if(tagName == "name")
      {
         actionName = childNode->value();
      }
      else if(tagName == "argumentList")
      {
         deserializeArgumentList(childNode);
      }
   }
   
   return NULL;
}

static bool deserializeActionList(rapidxml::xml_node<> *xmlNode, std::queue<CUPnPActionDesc *> &deviceDesc)
{
   for (rapidxml::xml_node<> *childNode = xmlNode->first_node(); childNode; childNode = childNode->next_sibling())
   {
      std::string tagName = childNode->name();
      
      if(tagName == "action")
      {
         CUPnPActionDesc *action = deserializeAction(childNode);
         
         if(action)
         {
            deviceDesc.push(action);
         }
         else
         {
            LOGGER_ERROR("Unable to create action.");
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
   m_actionList.push(serviceDesc);
   return true;
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
               deserializeActionList(childNode, m_actionList);
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
