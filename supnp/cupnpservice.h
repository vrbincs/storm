#ifndef CUPNPSERVICE_H
   #define CUPNPSERVICE_H

#include <string>
#include <map>

#include <rapidxml.hpp>

class CUPnPActionDesc;

class CUPnPService
{
public:
   CUPnPService(const std::string &type = "",
                int verMajor = 1,
                int verMinor = 0);
   virtual ~CUPnPService() {}
   
   inline void setType(const std::string &type) { m_type = type; }
   
   inline std::string getType() const { return m_type; }
   inline int getVersionMajor() const { return m_verMajor; }
   inline int getVersionMinor() const { return m_verMinor; }
   
   bool addAction(CUPnPActionDesc *action);
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   
   static CUPnPService *create();
private:
   std::string m_type;
   int m_verMajor;
   int m_verMinor;
   
   std::map<std::string, CUPnPActionDesc *> m_actionList;
};


#endif // CUPNPSERVICE_H
