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

   bool startService();
private:
   std::string m_pathUrl;
   std::string m_host;
   int m_port;
   bool m_running;
   
   std::string createUrl(const std::string &path);
};

#endif // UPNPDEVICE_H
