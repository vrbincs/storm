#ifndef CUPNPDEVICEDESC_H
   #define CUPNPDEVICEDESC_H

#include <queue>
#include <string>

#include <rapidxml.hpp>

class CUPnPActionDesc;

class CUPnPDeviceDesc
{
public:
   CUPnPDeviceDesc(const std::string &deviceType);
   virtual ~CUPnPDeviceDesc();
   
   bool addActionDesc(CUPnPActionDesc *serviceDesc);
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   
   const CUPnPActionDesc *findAction(const std::string &actionName) const;
   std::vector<CUPnPActionDesc *> getActionList() const;
protected:
   
private:
   std::string m_deviceType;  /*< Device type */
   std::map<std::string, CUPnPActionDesc *> m_actionList;
};

#endif // CUPNPDEVICEDESC_H
