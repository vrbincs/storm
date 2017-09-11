#include "logger.h"

#include "cupnpactionargumentdesc.h"
#include "cupnpactiondesc.h"
#include "cupnpservice.h"

CUPnPService::CUPnPService(const std::string &type,
                           const std::string &id)
   : m_type(type),
     m_id(id)
{
}

bool CUPnPService::addAction(CUPnPActionDesc *action)
{
   if(m_actionList.find(action->getName()) == m_actionList.end())
   {
      m_actionList.insert(std::pair<std::string, CUPnPActionDesc*>(action->getName(), action));
      return true;
   }
   
   LOGGER_WARN("The action already added. actionName=" << action->getName());
   return false;
}
