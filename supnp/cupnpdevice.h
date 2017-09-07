#ifndef UPNPDEVICE_H
   #define UPNPDEVICE_H

#include <string>

class CUPnPDevice
{
public:
   virtual ~CUPnPDevice();
   static CUPnPDevice *create(const std::string &serviceDescPath = "",
                              const std::string &host = "",
                              uint32_t port = 90000);

   bool run();
protected:
   CUPnPDevice(const std::string &serviceDescPath,
               const std::string &host,
               uint32_t port);

   bool startServices();
   
  // virtual bool onAction(const CUPnPAction &action);
private:
   std::string m_serviceDescPath;
   std::string m_host;
   uint32_t m_port;
   bool m_running;
   
   std::string createUrl(const std::string &path);
};

#endif // UPNPDEVICE_H
