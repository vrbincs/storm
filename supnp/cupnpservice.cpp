#include "logger.h"
#include "cupnpservice.h"

CUPnPService::CUPnPService(const std::string &serviceType)
   m_serviceType(serviceType)
{
}

bool CUPnPService::addAction(CUPnPAction *action)
{
   if(m_actionList.find(action->getName()) == m_actionList.end())
   {
      m_actionList->insert();
      return true;
   }
   
   LOGGER_WARN("The action already added. actionName=" << action->getName());
   return false;
}

std::string CUPnPService::getServiceType() const
{
   return m_serviceType;
}

bool isValid
