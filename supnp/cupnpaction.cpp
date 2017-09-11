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

