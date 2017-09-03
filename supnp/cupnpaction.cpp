#include "cupnpaction.h"

CUPnPAction::CUPnPAction()
{
}

CUPnPAction::CUPnPAction(const std::string &actionName)
   m_actionName(actionName)
{
}

bool CUPnPAction::addArg(const std::string &argumentName,
                         UPnPType type,
                         bool isDirectionIn)
{
}

std::string CUPnPAction::getName() const
{
}

bool CUPnPAction::serialize(xml_node<> *xmlNode)
{
}
