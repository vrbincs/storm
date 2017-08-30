#include <string>
#include <memory>

#include <upnp/upnp.h>

#include "logger.h"
#include "cupnpctrl.h"

static CUPnPCtrl *upnpControl = NULL;
static UpnpClient_Handle upnpControlPointHnd;

static int controlPointCallback(Upnp_EventType eventType,
                                void *event,
                                void *cookie)
{
   LOGGER_INFO("controlPointCallback::" << eventType << ":" << event << ":" << cookie);
   return 0;
}

CUPnPCtrl *CUPnPCtrl::create(const std::string &host,
                             int port)
{
   if(!upnpControl)
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
      
      upnpControl = new CUPnPCtrl(UpnpGetServerIpAddress(), UpnpGetServerPort());
   }
   else
   {
      LOGGER_INFO("An instance of upnpControl has already initialized the UPnP library.");
   }
   
   return upnpControl;
}

CUPnPCtrl::CUPnPCtrl(const std::string &host,
                     int port)
   : m_host(host),
     m_port(port)
{
   LOGGER_TRACE("New CUPnPCtrl is bind to the address:" << m_host << " at port:" << m_port);
   
   UpnpRegisterClient(controlPointCallback,
                      NULL,
                      &upnpControlPointHnd);
   m_running = true;
}

CUPnPCtrl::~CUPnPCtrl()
{
   if(UpnpFinish() == UPNP_E_SUCCESS)
   {
      LOGGER_TRACE("UPnP control point successfully terminated");
   }
   else
   {
      LOGGER_WARN("UPnP is not active.");
   }
   
   upnpControl = NULL;
}
