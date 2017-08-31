#include <string>
#include <memory>

#include <upnp/upnp.h>

#include "logger.h"
#include "cupnpdevice.h"

static CUPnPDevice *upnpDevice = NULL;
static UpnpDevice_Handle deviceHandle;

static int deviceCallback(Upnp_EventType eventType,
                          void *event,
                          void *cookie)
{
   LOGGER_INFO("deviceCallback::" << eventType << ":" << event << ":" << cookie);
   return 0;
}

CUPnPDevice *CUPnPDevice::create(const std::string &urlPath,
                                 const std::string &host,
                                 int port)
{
   if(upnpDevice == NULL)
   {
      const char *host_c = NULL;
      if(!host.empty())
      {
         host_c = host.c_str();
      }
      
      int err = UpnpInit(host_c, port);
      if(err != UPNP_E_SUCCESS)
      {
         LOGGER_ERROR("Unable to initialize UPnP. err=" << err);
         return NULL;
      }
      
      upnpDevice = new CUPnPDevice(urlPath, UpnpGetServerIpAddress(), UpnpGetServerPort());
   }
   else
   {
      LOGGER_INFO("An instance of CUPnPDevice has already initialized the UPnP library.");
   }
   
   return upnpDevice;
}

CUPnPDevice::CUPnPDevice(const std::string &pathUrl, 
                         const std::string &host,
                         int port)
   : m_pathUrl(pathUrl),
     m_host(host),
     m_port(port)
{
   m_running = startService();
}

CUPnPDevice::~CUPnPDevice()
{
   if(UpnpFinish() == UPNP_E_SUCCESS)
   {
      LOGGER_TRACE("UPnP device successfully terminated");
   }
   else
   {
      LOGGER_WARN("UPnP is not active.");
   }
   
   upnpDevice = NULL;
}

bool CUPnPDevice::run()
{
   if(m_running)
   {
      int err = UpnpSendAdvertisement(deviceHandle, 1);
      
      if(err != UPNP_E_SUCCESS)
      {
         LOGGER_ERROR("Unable to send advertisement. err=" << err);
      }
      else
      {
         LOGGER_TRACE("ok");
      }
   }
   
   return m_running;
}

bool CUPnPDevice::startService()
{
   std::string url = createUrl(m_pathUrl);
   LOGGER_TRACE("New CUPnPDevice:'" << url.c_str() << "' is bind to the address:" << m_host << " at port:" << m_port << ", webserverEnabled=" << UpnpIsWebserverEnabled());
   
   // Start HTTP server first
   int err = 0;
   if((err = UpnpSetWebServerRootDir("./")) != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to set HTTP root directory. dir=" << m_pathUrl << ", err=" << err);
      return false;
   }
   
   if((err = UpnpRegisterRootDevice(url.c_str(),
                                    deviceCallback,
                                    this,
                                    &deviceHandle)) != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to register root device. err=" << err);
      return false;
   }

   return true;
}

std::string CUPnPDevice::createUrl(const std::string &path)
{
   std::string url;
   url.reserve(256);
   
   url += "http://";
   url += UpnpGetServerIpAddress();
   url += ":";
   url += std::to_string(UpnpGetServerPort());
   url += "/";
   url += path;
   
   return url;
}

