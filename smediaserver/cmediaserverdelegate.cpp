#include <vector>
#include <fstream>

#include <logger.h>
#include <iupnpdevicedelegate.h>
#include <cupnpservice.h>
#include <crapidxmlhelper.h>

#include "cmediaserverdelegate.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define UPNP_MEDIA_SERVER_DEVICE_TYPE  "urn:schemas-upnp-org:device:MediaServer:1"

#define UPNP_MEDIA_SERVER_SERVICE_CDS  "urn:schemas-upnp-org:service:ContentDirectory:1"
#define UPNP_MEDIA_SERVER_SERVICE_ID   "urn:upnp-org:serviceId:ContentDirectory"
#define RESOURCE_MEDIA_SERVER_CDS_PATH STR(RESOURCE_PATH)"/resources/mediaServerCDS.xml"
#define MEDIA_SERVER_CDS_PATH          "/service/mediaServerCDS.xml"
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

const char *CMediaServerDelegate::getDeviceType() const
{
   return UPNP_MEDIA_SERVER_DEVICE_TYPE;
}

const char *CMediaServerDelegate::getFriendlyName() const
{
   return m_friendlyName.data();
}

const char *CMediaServerDelegate::getManufacturer() const
{
   return m_manufacturer.data();
}

const char *CMediaServerDelegate::getManufacturerUrl() const
{
   return m_manufacturerUrl.data();
}

const char *CMediaServerDelegate::getUuid() const
{
   return m_uuid.data();
}

const char *CMediaServerDelegate::getSCPD(const std::string &serviceType) const
{
   auto service_it = m_serviceSCPD.find(serviceType);
   
   if(service_it == m_serviceSCPD.end())
   {
      return NULL;
   }
   else
   {
      return service_it->second.data();
   }
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
                   UPNP_MEDIA_SERVER_SERVICE_ID,
                   MEDIA_SERVER_CDS_PATH,
                   RESOURCE_MEDIA_SERVER_CDS_PATH);
   //registerConnectionManager();
   //AVTransport();
}

bool CMediaServerDelegate::registerService(const std::string &type,
                                           const std::string &id,
                                           const std::string &scpdServerPath,
                                           const std::string &descrXmlPath)
{
   std::string xmlContent;
   
   if(loadFile(descrXmlPath, xmlContent))
   {
      m_serviceSCPD[type] = xmlContent;
      try
      {
         CUPnPService *service = CUPnPService::create();
         service->setType(type);
         service->setId(id);
         service->setSCPDPath(scpdServerPath);
         
         rapidxml::xml_document<> deviceDescXmlDoc;
         deviceDescXmlDoc.parse<0>(&(xmlContent)[0]);
         rapidxml::xml_node<> *root_node = deviceDescXmlDoc.first_node("scpd");
         
         if(service->deserialize(root_node))
         {
            return addService(service);
         }
         else
         {
            LOGGER_ERROR("Error while parsing device services.");
         }
      }
      catch(const rapidxml::parse_error &err)
      {
         LOGGER_ERROR("XML parse error. what='" << err.what() << "'");
      }
   }
   else
   {
      LOGGER_ERROR("Error reading file. descrXmlPath=" << descrXmlPath);
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
