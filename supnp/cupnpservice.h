#ifndef CUPNPSERVICE_H
   #define CUPNPSERVICE_H

#include <string>
#include <deque>

class CUPnPAction;

class CUPnPService
{
public:
   CUPnPService(const std::string &serviceType);
   
   bool addAction(CUPnPAction *action);
   std::string getServiceType() const;
   
   
private:
   std::string m_serviceType;
   std::map<std::string, CUPnPAction *> m_actionList;
};


#endif // CUPNPSERVICE_H
