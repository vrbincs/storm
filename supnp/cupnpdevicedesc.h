#ifndef CUPNPDEVICEDESC_H
   #define CUPNPDEVICEDESC_H

#include <queue>
#include <string>

#include <rapidxml.hpp>

class CUPnPActionDesc;

class CValue
{
public:
   virtual ~CValue(){}
   
   virtual std::string toString() const = 0;
   virtual int32_t toInt() const = 0;
};

class CValueString : public CValue
{
public:
   CValueString(const std::string &value)
      : m_value(value)
   {
   }
   
   std::string toString() const override
   {
      return m_value;
   }
   
   int32_t toInt() const override
   {
      return 0;
   }
private:
   std::string m_value;
};

class CValueInt : public CValue
{
public:
   CValueInt(const int32_t &value)
      : m_value(value)
   {
   }
   
   std::string toString() const override
   {
      return std::to_string(m_value);
   }
   
   int32_t toInt() const override
   {
      return m_value;
   }
private:
   int32_t m_value;
};

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
