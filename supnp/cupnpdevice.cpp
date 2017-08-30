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

CUPnPDevice *CUPnPDevice::create(const std::string &descUrl,
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
      
      upnpDevice = new CUPnPDevice(descUrl, UpnpGetServerIpAddress(), UpnpGetServerPort());
   }
   else
   {
      LOGGER_INFO("An instance of CUPnPDevice has already initialized the UPnP library.");
   }
   
   return upnpDevice;
}

CUPnPDevice::CUPnPDevice(const std::string &descUrl, 
                       const std::string &host,
                       int port)
   : m_descUrl(descUrl),
     m_host(host),
     m_port(port)
{
   LOGGER_TRACE("New CUPnPDevice:'" << descUrl << "' is bind to the address:" << m_host << " at port:" << m_port);
   
   UpnpRegisterRootDevice(descUrl.c_str(),
                          deviceCallback,
                          NULL,
                          &deviceHandle);
   
   m_running = true;
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
      // poll some events
   }
   
   return m_running;
}

