#ifndef UPNPCTRL_H
   #define UPNPCTRL_H

#include <string>

class CUPnPCtrl
{
public:
   virtual ~CUPnPCtrl();
   static CUPnPCtrl *create(const std::string &host = "",
                            int port = 0);

   void search(const std::string &target);
   bool run();
protected:
   CUPnPCtrl(const std::string &host, int port);
   void search();
private:
   std::string m_descUrl;
   std::string m_host;
   int m_port;
   bool m_running;
   bool m_searching;
   std::string m_searchTarget;
};

#endif
