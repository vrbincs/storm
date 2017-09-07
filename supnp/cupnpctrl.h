#ifndef UPNPCTRL_H
   #define UPNPCTRL_H

#include <string>
#include <list>
#include <map>

class CUPnPService;

class CUPnPCtrl
{
public:
   virtual ~CUPnPCtrl();
   static CUPnPCtrl *create(const std::string &host = "",
                            int port = 0);

   void search(const std::string &target);
   
   bool run();
   
   bool registerDeviceService(const std::string &deviceType,
                              const std::string &deviceSCPD);
protected:
   CUPnPCtrl();
   CUPnPCtrl(const std::string &host, int port);
   void search();
private:
   std::string m_host;
   int m_port;
   bool m_running;
   bool m_searching;
   std::string m_searchTarget;
   
   std::map<std::string, std::list<CUPnPService *> > m_serviceList;
};

#endif
