#ifndef UPNPDEVICE_H
   #define UPNPDEVICE_H

#include <string>

class CUPnPDevice
{
public:
   virtual ~CUPnPDevice();
   static CUPnPDevice *create(const std::string &desc = "GenCUPnPDevice",
                             const std::string &host = "",
                             int port = 90000);

   bool run();
protected:
   CUPnPDevice(const std::string &descUrl,
               const std::string &host,
               int port);
private:
   std::string m_descUrl;
   std::string m_host;
   int m_port;
   bool m_running;
};

#endif // UPNPDEVICE_H
