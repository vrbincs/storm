#ifndef CMEDIASERVERDELEGATE_H
   #define CMEDIASERVERDELEGATE_H

#include <vector>

class CMediaServerDelegate : public IUPnPDeviceDelegate
{
public:
   CMediaServerDelegate(const std::string &uuid,
                        const std::string &friendlyName,
                        const std::string &manufacturer,
                        const std::string &manufacturerUrl);

   ~CMediaServerDelegate();
protected:
   bool onAction(const CUPnPAction &action);
   std::map<std::string, CUPnPActionDesc *> getServiceList() const;
   
   bool addService(CUPnPActionDesc *service);
   bool registerService(const std::string &type,
                        const std::string &descrXmlPath);


   bool loadFile(const std::string &filePath,
                 std::string &fileContent);
private:
   std::string m_uuid;
   std::string m_friendlyName;
   std::string m_manufacturer;
   std::string m_manufacturerUrl;
   
   void registerServices();
};


#endif // CMEDIASERVERDELEGATE_H
