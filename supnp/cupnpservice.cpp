#include "logger.h"

#include "cupnpactionargumentdesc.h"
#include "cupnpactiondesc.h"
#include "cupnpservice.h"
#include "crapidxmlhelper.h"

CUPnPService::CUPnPService(const std::string &type,
                           const std::string &id,
                           int verMajor,
                           int verMinor)
   : m_type(type),
     m_id(id),
     m_verMajor(verMajor),
     m_verMinor(verMinor),
     m_refreshScpd(true)
{
}

bool CUPnPService::addAction(CUPnPActionDesc *action)
{
   if(m_actionList.find(action->getName()) == m_actionList.end())
   {
      m_actionList.insert(std::pair<std::string, CUPnPActionDesc*>(action->getName(), action));
      m_refreshScpd = true;
      return true;
   }
   
   LOGGER_WARN("The action already added. actionName=" << action->getName());
   return false;
}

bool CUPnPService::deserialize(rapidxml::xml_node<> *xmlNode)
{
   CRapidXmlHelper xmlHelper(xmlNode);
   
   if(xmlHelper.getAttributeValue("xmlns") == "urn:schemas-upnp-org:service-1-0")
   {
      xmlHelper.next();
      
      while(xmlHelper.isValid())
      {
         if(xmlHelper.getNodeValue() == "specVersion")
         {
            CRapidXmlHelper xmlHelperVersion(xmlHelper);
            
            xmlHelperVersion.next();
            
            m_verMajor = xmlHelperVersion.getNodeValue("major").toInt32();
            m_verMinor = xmlHelperVersion.getNodeValue("minor").toInt32();
            
            // parse version
            LOGGER_INFO("parsing specVersion");
         }
         else if(xmlHelper.getNodeValue() == "actionList")
         {
            // parse action lists
            LOGGER_INFO("TODO: parsing actionList");
         }
         else if(xmlHelper.getNodeValue() == "serviceStateTable")
         {
            // parse state variables
            LOGGER_INFO("TODO: parsing serviceStateTable");
         }
         
         xmlHelper.nextSibling();
      }
      
      return true;
   }
   else
   {
      LOGGER_ERROR("Invalid XML structure.");
      return false;
   }
}

std::string CUPnPService::serialize()
{
   std::string scpd;
   
   CRapidXmlHelper helper;
   rapidxml::xml_node<> *node0 = helper.createNode("scpd");
   helper.appendAttribute("xmlns", "urn:schemas-upnp-org:service-1-0", node0);
   
   rapidxml::xml_node<> *node1 = helper.createNode("specVersion");
   rapidxml::xml_node<> *node2 = helper.createNode("major", "1");
   rapidxml::xml_node<> *node3 = helper.createNode("minor", "0");
   helper.appendNode(node1, node2);
   helper.appendNode(node1, node3);
   helper.appendNode(node0, node1);
   
   
   
   helper.appendNode(node0);
   return helper.toString();
}

CUPnPService *CUPnPService::create()
{
   return new CUPnPService();
}

const char *CUPnPService::getScpd()
{ 
   if(m_refreshScpd)
   {
      m_scpd = serialize();
      m_refreshScpd = false;
   }
   
   return m_scpd.data();
}
