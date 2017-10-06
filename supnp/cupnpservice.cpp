#include <memory>

#include "logger.h"

#include "cupnpaction.h"
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

bool CUPnPService::addAction(CUPnPAction *action)
{
   if(m_actionList.find(*action->getName().get()) == m_actionList.end())
   {
      m_actionList.insert(std::pair<std::string, CUPnPAction *>(*action->getName().get(), action));
      m_refreshScpd = true;
      return true;
   }
   
   LOGGER_WARN("The action already added. actionName=" << action->getName());
   return false;
}

bool CUPnPService::deserialize(rapidxml::xml_node<> *xmlNode)
{
   CRapidXmlHelper xmlHelper(xmlNode);
   
   if(xmlHelper.getNodeName() == "scpd" &&
      xmlHelper.getAttributeValue("xmlns") == "urn:schemas-upnp-org:service-1-0")
   {
      xmlHelper.next();
      
      while(xmlHelper.isValid())
      {
         if(xmlHelper.getNodeName() == "specVersion")
         {
            CRapidXmlHelper xmlHelperVersion(xmlHelper);
            
            xmlHelperVersion.next();
            
            m_verMajor = xmlHelperVersion.getNodeValue("major").toInt32();
            m_verMinor = xmlHelperVersion.getNodeValue("minor").toInt32();
         }
         else if(xmlHelper.getNodeName() == "actionList")
         {
            // parse action lists
            LOGGER_INFO("TODO: parsing actionList");
            xmlHelper.next();
            
            while(xmlHelper.getNodeName() == "action")
            {
               CUPnPAction *action = CUPnPAction::create();
               if(action->deserialize(xmlHelper.node()))
               {
                  
               }
               else
               {
                  LOGGER_ERROR("Unable to serialize CUPnPAction.");
               }
               
               xmlHelper.nextSibling();
            }
         }
         else if(xmlHelper.getNodeName() == "serviceStateTable")
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

CUPnPService *CUPnPService::create(char *xmlbuf)
{
   std::unique_ptr<CUPnPService> instance = std::unique_ptr<CUPnPService>(new CUPnPService());
   
   if(xmlbuf != NULL)
   {
      try
      {
         // We need to copy the XML content before we pass it to the parser.
         instance->setScpd(xmlbuf);
         
         rapidxml::xml_document<> deviceDescXmlDoc;
         deviceDescXmlDoc.parse<0>(xmlbuf);
         rapidxml::xml_node<> *scpdNode = deviceDescXmlDoc.first_node("scpd");
         
         if(!instance->deserialize(scpdNode))
         {
            LOGGER_ERROR("Error while de-serialization of the XML.");
            return NULL;
         }
      }
      catch(const rapidxml::parse_error &err)
      {
         LOGGER_ERROR("XML parse error. what='" << err.what() << "'");
         return NULL;
      }
   }
   
   return instance.release();
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

void CUPnPService::setScpd(const char *scpd)
{
   m_scpd = scpd;
   m_refreshScpd = false;
}
