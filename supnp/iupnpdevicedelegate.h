#ifndef IUPNPDEVICEDELEGATE_H
   #define IUPNPDEVICEDELEGATE_H

#include <map>
#include <string>

class CUPnPAction;
class CUPnPService;
class CUPnPDevice;

class IUPnPDeviceDelegate
{
public:
   virtual ~IUPnPDeviceDelegate(){}
   
   virtual const char *getDeviceType() const = 0;
   virtual const char *getFriendlyName() const = 0;
   virtual const char *getManufacturer() const = 0;
   virtual const char *getManufacturerUrl() const = 0;
   virtual const char *getUuid() const = 0;
   virtual const char *getSCPD(const std::string &serviceType) const = 0;
protected:
   virtual bool onAction(const CUPnPAction &action) = 0;
   virtual std::map<std::string, CUPnPService *> getServiceList() const = 0;
   
   friend class CUPnPDevice;
};

#endif //IUPNPDEVICEDELEGATE_H
