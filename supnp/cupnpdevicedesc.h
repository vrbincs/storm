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
protected:
   
private:
   std::string m_deviceType;  /*< Device type */
   std::queue<CUPnPActionDesc *> m_actionList;
};

#endif // CUPNPDEVICEDESC_H
