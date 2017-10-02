#include "crapidxmlhelper.h"

#include "logger.h"

#include "cupnpactionargumentdesc.h"
#include "cupnpactiondesc.h"
#include "cupnpaction.h"

CUPnPAction::CUPnPAction(const CUPnPActionDesc *actionDesc)
   : m_upnpActionDesc(actionDesc)
{
}

std::string CUPnPAction::getName() const
{
   if(m_upnpActionDesc)
   {
      return m_upnpActionDesc->getName();
   }
   else
   {
      LOGGER_WARN("Action description object is NULL.");
      return std::string();
   }
}

CUPnPAction *CUPnPAction::create()
{
   return new CUPnPAction();
}

bool CUPnPAction::deserialize(rapidxml::xml_node<> *xmlNode)
{
   CRapidXmlHelper xmlHelper(xmlNode);
   if(xmlHelper.isNodeName("action"))
   {
      xmlHelper.next();
      
      while(xmlHelper.isValid())
      {
         if(xmlHelper.isNodeName("name"))
         {
            m_name = xmlHelper.getNodeValue();
            LOGGER_INFO("name=" << m_name);
         }
         else if(xmlHelper.isNodeName("argumentList"))
         {
            xmlHelper.next();
            
            while(xmlHelper.isValid())
            {
               std::string tagName;
            
               if(tagName == "argument")
               {
                  //CUPnPActionArgumentDesc *arg = CUPnPActionArgumentDesc::create();
                  //arg->deserialize(childNodeArg);
                  //addArg(arg);
               }
               else
               {
                  //LOGGER_WARN("Unkown XML tag. tag=" << tagName);
               }
               xmlHelper.nextSibling();
            }
         }
         else
         {
            LOGGER_WARN("Unsuppored tag in the XML. nodeName=" << xmlHelper.getNodeValue());
         }
         
         xmlHelper.nextSibling();
      }
      
      if(!m_name.empty())
      {
         return true;
      }
   }
   
   return false;
}

