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

CUPnPDevice *CUPnPDevice::create(const std::string &desc,
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
      
      std::string descUrl = "http://";
      descUrl += UpnpGetServerIpAddress();
      descUrl += ":";
      descUrl += std::to_string(UpnpGetServerPort());
      descUrl += "/";
      descUrl += desc;
      
      upnpDevice = new CUPnPDevice("uuid:3432-sdfsdfsdf-fd", UpnpGetServerIpAddress(), UpnpGetServerPort());
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
   LOGGER_TRACE("New CUPnPDevice:'" << m_descUrl << "' is bind to the address:" << m_host << " at port:" << m_port);
   
   int err = UpnpRegisterRootDevice(m_descUrl.c_str(),
                                    deviceCallback,
                                    this,
                                    &deviceHandle);
   
   if(err != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to register root device. err=" << err);
   }
   else
   {
      m_running = true;
   }
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

