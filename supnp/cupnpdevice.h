#ifndef UPNPDEVICE_H
   #define UPNPDEVICE_H

#include <string>

#include <rapidxml.hpp>

class CUPnPAction;
class IUPnPDeviceDelegate;

class CUPnPDevice
{
public:
   virtual ~CUPnPDevice();
   static CUPnPDevice *create(IUPnPDeviceDelegate *deviceDelegate,
                              const std::string &serviceDescPath = "",
                              const std::string &host = "",
                              uint32_t port = 90000);
   bool run();
protected:
   CUPnPDevice(const std::string &serviceDescPath,
               const std::string &host,
               uint32_t port,
               IUPnPDeviceDelegate *deviceDelegate);

   bool startServices();
private:
   std::string m_serviceDescPath;
   std::string m_host;
   uint32_t m_port;
   bool m_running;
   IUPnPDeviceDelegate *m_deviceDelegate;
   
   std::string m_descriptionXml;
   std::string m_baseURI;
   std::string createUrl(const std::string &path = "");
   
   bool createDescriptionXml();
};

#endif // UPNPDEVICE_H
