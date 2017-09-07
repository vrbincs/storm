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
   switch(eventType)
   {
      case UPNP_DISCOVERY_SEARCH_RESULT:
      {
         Upnp_Discovery *eventDiscovery = reinterpret_cast<Upnp_Discovery *>(event);
         LOGGER_INFO(eventDiscovery->DeviceId);
         //LOGGER_INFO("UPNP_DISCOVERY_SEARCH_RESULT");
      }
      break;
      default:
         LOGGER_INFO("controlPointCallback::" << eventType << ":" << event << ":" << cookie);
      break;
   }
   
   return 0;
}

static const int SearchTimeout = 5;

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

CUPnPCtrl::CUPnPCtrl()
{
}

CUPnPCtrl::CUPnPCtrl(const std::string &host,
                     int port)
   : m_host(host),
     m_port(port),
     m_running(false),
     m_searching(false)
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

void CUPnPCtrl::search(const std::string &target)
{
   m_searchTarget = target;
}

bool CUPnPCtrl::run()
{
   search();
   
   return true;
}

void CUPnPCtrl::search()
{
   if(!m_searchTarget.empty() && !m_searching)
   {
      LOGGER_TRACE("seaching ... [" << m_searchTarget << "]");
      
      int err = UpnpSearchAsync(upnpControlPointHnd,
                                SearchTimeout,
                                m_searchTarget.c_str(),
                                this);
      
      if(err != UPNP_E_SUCCESS)
      {
         LOGGER_TRACE("Error while search for devices. err=" << err);
      }
      else
      {
         m_searching = true;
      }
   }
}

bool CUPnPCtrl::registerDeviceService(const std::string &deviceType,
                                      const std::string &deviceSCPD)
{
   return false;
}
