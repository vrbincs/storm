#ifndef IUPNPDEVICEDELEGATE_H
   #define IUPNPDEVICEDELEGATE_H

#include <map>
#include <string>

class CUPnPAction;
class CUPnPService;

class IUPnPDeviceDelegate
{
public:
   virtual ~IUPnPDeviceDelegate(){}
protected:
   virtual bool onAction(const CUPnPAction &action) = 0;
   virtual std::map<std::string, CUPnPService *> getServiceList() const = 0;
};

#endif //IUPNPDEVICEDELEGATE_H
