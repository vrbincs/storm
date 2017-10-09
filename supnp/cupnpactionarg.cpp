#include "crapidxmlhelper.h"

#include "cupnpactionarg.h"

CUPnPActionArg::CUPnPActionArg()
      : m_isOut(false)
{
}

CUPnPActionArg *CUPnPActionArg::create()
{
   return new CUPnPActionArg();
}

bool CUPnPActionArg::deserialize(rapidxml::xml_node<> *xmlNode)
{
   CRapidXmlHelper xmlHelper(xmlNode);
   
   if(xmlHelper.isNodeName("argument"))
   {
      xmlHelper.next();
      
      while(xmlHelper.isValid())
      {
         std::string tagName = xmlHelper.getNodeName();
         
         if(tagName == "name")
         {
            m_name = CStringPtr(CString::create(xmlHelper.getNodeValue()));
         }
         else if(tagName == "direction")
         {
            if(std::string(xmlHelper.getNodeValue()) == "out")
            {
               m_isOut = true;
            }
         }
         else if(tagName == "relatedStateVariable")
         {
            m_stateVar = CStringPtr(CString::create(xmlHelper.getNodeValue()));
         }
         else
         {
            return false;
         }
         
         xmlHelper.nextSibling();
      }
      
      return true;
   }
   else
   {
      return false;
   }
   
}
