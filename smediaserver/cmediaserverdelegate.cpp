#include <vector>
#include <fstream>

#include <logger.h>
#include <iupnpdevicedelegate.h>
#include <cupnpservice.h>
#include <crapidxmlhelper.h>

#include "cmediaserverdelegate.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define UPNP_MEDIA_SERVER_DEVICE_TYPE "urn:schemas-upnp-org:device:MediaServer:1"

#define UPNP_MEDIA_SERVER_SERVICE_CDS "urn:schemas-upnp-org:service:ContentDirectory:1"
#define RESOURCE_MEDIA_SERVER_CDS     STR(RESOURCE_PATH)"/resources/mediaServerCDS.xml"
//#define RESOURCE_MEDIA_SERVER_ROOT STR(RESOURCE_PATH)"/resources/mediaServerRoot.xml"


static std::map<std::string, CUPnPService *> l_serviceList;

CMediaServerDelegate::CMediaServerDelegate(const std::string &uuid,
                                           const std::string &friendlyName,
                                           const std::string &manufacturer,
                                           const std::string &manufacturerUrl)
   : m_uuid(uuid),
     m_friendlyName(friendlyName),
     m_manufacturer(manufacturer),
     m_manufacturerUrl(manufacturerUrl)
{
   registerServices();
}

CMediaServerDelegate::~CMediaServerDelegate()
{
   auto service_it = l_serviceList.begin();
   
   for(;service_it != l_serviceList.end(); service_it++)
   {
      delete service_it->second;
   }
   
   l_serviceList.clear();
}

bool CMediaServerDelegate::onAction(const CUPnPAction &action)
{
   return false;
}

std::map<std::string, CUPnPService *> CMediaServerDelegate::getServiceList() const
{
   return l_serviceList;
}

bool CMediaServerDelegate::addService(CUPnPService *service)
{
   if(l_serviceList.find(service->getType()) == l_serviceList.end())
   {
      l_serviceList.insert(std::pair<std::string, CUPnPService *>(service->getType(), service));
      return true;
   }
   
   return false;
}

void CMediaServerDelegate::registerServices()
{
   // There are three services in the MediaServer:1
   // ContentDirectory:1.0 (required)
   // ConnectionManager:1.0 (required)
   // AVTransport:1.0 (optional)
   
   registerService(UPNP_MEDIA_SERVER_SERVICE_CDS,
                   RESOURCE_MEDIA_SERVER_CDS);
   //registerConnectionManager();
   //AVTransport();
}

bool CMediaServerDelegate::registerService(const std::string &type,
                                           const std::string &descrXmlPath)
{
   std::string xmlContent;
   
   if(loadFile(descrXmlPath, xmlContent))
   {
      try
      {
         rapidxml::xml_document<> deviceDescXmlDoc;
         
         deviceDescXmlDoc.parse<0>(&xmlContent[0]);
         rapidxml::xml_node<> *root_node = deviceDescXmlDoc.first_node("scpd");
         
         CUPnPService *service = CUPnPService::create();
         service->setType(type);
         
         return service->deserialize(root_node);
      }
      catch(const rapidxml::parse_error &err)
      {
         std::cout << "XML parse error. what='" << err.what() << "'" << std::endl;
      }
   }
   else
   {
      LOGGER_INFO("Error reading file. descrXmlPath=" << descrXmlPath);
   }
   
   return false;
}

bool CMediaServerDelegate::loadFile(const std::string &filePath,
                                    std::string &fileContent)
{
   std::ifstream istream(filePath);
   if(istream.is_open())
   {
      istream.seekg(0, std::ios::end);
      size_t strSize = (1 + istream.tellg());
      fileContent.reserve(strSize);
      istream.seekg(0, std::ios::beg);
      
      fileContent.assign(std::istreambuf_iterator<char>(istream),
                         std::istreambuf_iterator<char>());
      fileContent[strSize-1] = '\0';
      
      return true;
   }
   
   return false;
}
