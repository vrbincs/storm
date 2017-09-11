#ifndef CUPNPACTIONDESC_H
   #define CUPNPACTIONDESC_H

#include <string>
#include <map>

#include <rapidxml.hpp>

class CUPnPActionDesc
{
public:
   virtual ~CUPnPActionDesc(){}
   
   static CUPnPActionDesc *create();
   static void destroy(CUPnPActionDesc *obj);
   
   bool addArg(CUPnPActionArgumentDesc *arg);
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   const CUPnPActionArgumentDesc *findArg(const std::string &argName) const;
   
   inline void setActionName(const std::string &actionName)
   {
      m_actionName = actionName;
   }
   
   inline std::string getName() const
   {
      return m_actionName;
   }
protected:
   CUPnPActionDesc(const std::string &actionName = "");
   
private:
   std::string m_actionName;
   std::map<std::string, CUPnPActionArgumentDesc *> m_argumentMap;
};

#endif // CUPNPACTIONDESC_H
