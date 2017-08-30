#ifndef UPNPCTRL_H
   #define UPNPCTRL_H

#include <string>

class CUPnPCtrl
{
public:
   virtual ~CUPnPCtrl();
   static CUPnPCtrl *create(const std::string &host = "",
                            int port = 0);
protected:
   CUPnPCtrl(const std::string &host,
             int port);
private:
   std::string m_descUrl;
   std::string m_host;
   int m_port;
   bool m_running;
};

#endif
