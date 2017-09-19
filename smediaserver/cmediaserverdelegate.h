#ifndef CMEDIASERVERDELEGATE_H
   #define CMEDIASERVERDELEGATE_H

#include <vector>
#include <iupnpdevicedelegate.h>

class CMediaServerDelegate : public IUPnPDeviceDelegate
{
public:
   CMediaServerDelegate(const std::string &uuid,
                        const std::string &friendlyName,
                        const std::string &manufacturer,
                        const std::string &manufacturerUrl);

   ~CMediaServerDelegate();
   
   const char *getDeviceType() const override;
   const char *getFriendlyName() const override;
   const char *getManufacturer() const override;
   const char *getManufacturerUrl() const override;
   const char *getUuid() const override;
protected:
   bool onAction(const CUPnPAction &action) override;
   std::map<std::string, CUPnPService *> getServiceList() const override;
   
   bool addService(CUPnPService *service);
   bool registerService(const std::string &type,
                        const std::string &id,
                        const std::string &url,
                        const std::string &descrXmlPath);


   bool loadFile(const std::string &filePath,
                 std::string &fileContent);
private:
   std::string m_uuid;
   std::string m_friendlyName;
   std::string m_manufacturer;
   std::string m_manufacturerUrl;
   
   std::map<std::string, std::string *> m_serviceSCPD;
   
   void registerServices();
};


#endif // CMEDIASERVERDELEGATE_H
