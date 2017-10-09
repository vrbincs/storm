#include "crapidxmlhelper.h"

#include "logger.h"

#include "cupnpaction.h"

CUPnPAction::CUPnPAction()
{
}

CStringPtr CUPnPAction::getName() const
{
   return m_name;
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
            m_name = CStringPtr(CString::create(xmlHelper.getNodeValue()));
         }
         else if(xmlHelper.isNodeName("argumentList"))
         {
            xmlHelper.next();
            
            while(xmlHelper.isValid())
            {
               std::string tagName;
               if(xmlHelper.isNodeName("argument"))
               {
                  CUPnPActionArgPtr argPtr = CUPnPActionArgPtr(CUPnPActionArg::create());
                  if(argPtr->deserialize(xmlHelper.node()))
                  {
                     addArg(argPtr);
                  }
                  else
                  {
                     LOGGER_ERROR("Unable to add argument.");
                  }
               }
               else
               {
                  LOGGER_WARN("Unkown XML tag. tag=" << tagName);
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
      
      if(!m_name->empty())
      {
         return true;
      }
   }
   
   return false;
}

bool CUPnPAction::addArg(const CUPnPActionArgPtr &arg)
{
   auto arg_it = m_argumentMap.find(arg->getName());
   if(arg_it == m_argumentMap.end())
   {
      m_argumentMap.insert(std::pair<CStringPtr, CUPnPActionArgPtr>(arg->getName(), arg));
      LOGGER_TRACE("Added argument. actionName=" << *m_name << ", arg->getName() = " << *arg->getName());
      
      return true;
   }
   else
   {
      LOGGER_WARN("Unable to add argument. arg=" << arg->getName());
      return false;
   }
}
