#include <string>
#include <memory>
#include <cstring>
#include <vector>

#include <upnp/upnp.h>

#include "logger.h"
#include "cupnpdevice.h"
#include "crapidxmlhelper.h"
#include "iupnpdevicedelegate.h"
#include "cupnpservice.h"
#include "cupnpwww.h"
#include "cupnphelper.h"

static CUPnPDevice *l_upnpDevice = NULL;
static UpnpDevice_Handle deviceHandle;

static const uint32_t l_aliveAdvertTime = 1800;

static bool sendUPnPAdvertisement()
{
   int err = UpnpSendAdvertisement(deviceHandle, l_aliveAdvertTime);
   
   if(err != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to send advertisement. err=" << err);
      return false;
   }
   else
   {
      LOGGER_INFO("Alive successfully advertised.");
   }
   
   return true;
}

static int deviceCallback(Upnp_EventType eventType,
                          void *event,
                          void *cookie)
{
   LOGGER_INFO("deviceCallback::" << eventType << ":" << event << ":" << cookie);
   return 0;
}

CUPnPDevice *CUPnPDevice::create(IUPnPDeviceDelegate *deviceDelegate,
                                 const std::string &serviceDescPath,
                                 const std::string &host,
                                 uint32_t port)
{
   if(l_upnpDevice == NULL)
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
      
      l_upnpDevice = new CUPnPDevice(serviceDescPath, 
                                     UpnpGetServerIpAddress(),
                                     UpnpGetServerPort(),
                                     deviceDelegate);
   }
   else
   {
      LOGGER_INFO("An instance of CUPnPDevice has already initialized the UPnP library.");
   }
   
   return l_upnpDevice;
}

CUPnPDevice::CUPnPDevice(const std::string &serviceDescPath,
                         const std::string &host,
                         uint32_t port,
                         IUPnPDeviceDelegate *deviceDelegate)
   : m_serviceDescPath(serviceDescPath),
     m_host(host),
     m_port(port),
     m_deviceDelegate(deviceDelegate)
{
   m_running = startServices();
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
   
   l_upnpDevice = NULL;
}

bool CUPnPDevice::run()
{
   if(m_running)
   {
   }
   
   return m_running;
}

bool CUPnPDevice::startServices()
{
   m_baseURI = createUrl();
   
   // create descriptionXML
   if(!createDescriptionXml())
   {
      LOGGER_ERROR("Error in creation of description XML.");
      return false;
   }
   
   LOGGER_INFO("Device description:" << m_descriptionXml);
   LOGGER_TRACE("New CUPnPDevice:'" << m_baseURI << "' is bind to the address:" << m_host << " at port:" << m_port << ", webserverEnabled=" << UpnpIsWebserverEnabled());
   
   if(!startWebService())
   {
      return false;
   }
   
   int err = 0;
   if((err = UpnpRegisterRootDevice2(UPNPREG_BUF_DESC,
                                     m_descriptionXml.c_str(),
                                     m_descriptionXml.size(),
                                     1,
                                     deviceCallback,
                                     this,
                                     &deviceHandle)) != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to register root device. err=" << err);
      return false;
   }
   
   sendUPnPAdvertisement();

   return true;
}

bool CUPnPDevice::startWebService()
{
   int err = 0;
   
   CUPnPWWW *wwwInst = CUPnPWWW::instance();
   
   // install www directories
   auto serviceList = m_deviceDelegate->getServiceList();
   auto serviceListIt = serviceList.begin();
   for(; serviceListIt != serviceList.end(); serviceListIt++)
   {
      const char *scpdXml = serviceListIt->second->getScpd();
      wwwInst->addFileWithPath(serviceListIt->second->getSCPDPath(),
                               scpdXml,
                               strlen(scpdXml));
   }
   
   if((err = UpnpSetWebServerRootDir("/")) != UPNP_E_SUCCESS)
   {
      LOGGER_ERROR("Unable to set HTTP root directory. err=" << err);
      return false;
   }
   
   return true;
}

std::string CUPnPDevice::createUrl(const std::string &path)
{
   std::string url;
   url.reserve(100);
   
   if(!path.empty() && path.at(0) != '/')
   {
      LOGGER_ERROR("Path should begin with '/'");
      return url;
   }
   
   
   url.reserve(256);
   
   url += "http://";
   url += UpnpGetServerIpAddress();
   url += ":";
   url += std::to_string(UpnpGetServerPort());
   
   
   url += path;
   
   return url;
}

bool CUPnPDevice::createDescriptionXml()
{
   try
   {
      CRapidXmlHelper xmlHelper;
      rapidxml::xml_node<> *nodeRoot = NULL;
      rapidxml::xml_node<> *node = NULL;
      rapidxml::xml_node<> *nodeChild1 = NULL;
      rapidxml::xml_node<> *nodeChild2 = NULL;
      rapidxml::xml_node<> *nodeChild3 = NULL;
      
      nodeRoot = xmlHelper.createNode("root");
      xmlHelper.appendAttribute("xmlns", "urn:schemas-upnp-org:device-1-0", nodeRoot);
      
      // Add specVersion
      node = xmlHelper.createNode("specVersion");
      nodeChild1 = xmlHelper.createNode("major", "0");
      xmlHelper.appendNode(node, nodeChild1);
      nodeChild1 = xmlHelper.createNode("minor", "1");
      xmlHelper.appendNode(node, nodeChild1);
      xmlHelper.appendNode(nodeRoot, node);
      
      // Add URLBase
      node = xmlHelper.createNode("URLBase", m_baseURI.c_str());
      xmlHelper.appendNode(nodeRoot, node);
      
      // Add device
      node = xmlHelper.createNode("device");
      nodeChild1 = xmlHelper.createNode("deviceType", m_deviceDelegate->getDeviceType());
      xmlHelper.appendNode(node, nodeChild1);
      nodeChild1 = xmlHelper.createNode("friendlyName", m_deviceDelegate->getFriendlyName());
      xmlHelper.appendNode(node, nodeChild1);
      nodeChild1 = xmlHelper.createNode("manufacturer", m_deviceDelegate->getManufacturer());
      xmlHelper.appendNode(node, nodeChild1);
      nodeChild1 = xmlHelper.createNode("manufacturerURL", m_deviceDelegate->getManufacturerUrl());
      xmlHelper.appendNode(node, nodeChild1);
      nodeChild1 = xmlHelper.createNode("UDN", m_deviceDelegate->getUuid());
      xmlHelper.appendNode(node, nodeChild1);
      
      // Add services
      auto serviceList = m_deviceDelegate->getServiceList();
      LOGGER_INFO("Service list size:" << serviceList.size());
      
      nodeChild1 = xmlHelper.createNode("serviceList");

      auto serviceListIt = serviceList.begin();
      for(; serviceListIt != serviceList.end(); serviceListIt++)
      {
         char *controlUrl = xmlHelper.getDocument()->allocate_string(createUrl(serviceListIt->second->getControlPath()).c_str());
         char *scpdUrl    = xmlHelper.getDocument()->allocate_string(createUrl(serviceListIt->second->getSCPDPath()).c_str());
         
         nodeChild2 = xmlHelper.createNode("service");
         nodeChild3 = xmlHelper.createNode("serviceType", serviceListIt->second->getType());
         xmlHelper.appendNode(nodeChild2, nodeChild3);
         nodeChild3 = xmlHelper.createNode("serviceId", serviceListIt->second->getId());
         xmlHelper.appendNode(nodeChild2, nodeChild3);
         nodeChild3 = xmlHelper.createNode("controlURL", controlUrl);
         xmlHelper.appendNode(nodeChild2, nodeChild3);
         nodeChild3 = xmlHelper.createNode("eventSubURL", serviceListIt->second->getEventUrl());
         xmlHelper.appendNode(nodeChild2, nodeChild3);

         nodeChild3 = xmlHelper.createNode("SCPDURL", scpdUrl);
         xmlHelper.appendNode(nodeChild2, nodeChild3);
         xmlHelper.appendNode(nodeChild1, nodeChild2);
      }
      xmlHelper.appendNode(node, nodeChild1);
      xmlHelper.appendNode(nodeRoot, node);
      
      xmlHelper.appendNode(nodeRoot);
      
      m_descriptionXml = xmlHelper.toString();
      return true;
   }catch(...)
   {
      LOGGER_WARN("ERROR while creating XML.");
      return false;
   }
}
