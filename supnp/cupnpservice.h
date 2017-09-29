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
                const std::string &id = "",
                int verMajor = 1,
                int verMinor = 0);
   virtual ~CUPnPService() {}
   
   inline void setType(const std::string &type) { m_type = type; }
   inline void setId(const std::string &id) { m_id = id; }
   inline void setSCPDPath(const std::string &path) { m_scpdPath = path; }
   inline void setControlUrl(const std::string &url) { m_ctrlUrl = url; }
   inline void setEventUrl(const std::string &url) { m_eventUrl = url; }
   
   inline const char * getType() const { return m_type.data(); }
   inline const char * getId() const { return m_id.data(); }
   inline const char * getSCPDPath() const { return m_scpdPath.data(); }
   inline const char * getControlUrl() const { return m_ctrlUrl.data(); }
   inline const char * getEventUrl() const { return m_eventUrl.data(); }
   const char *getScpd();
   
   inline int getVersionMajor() const { return m_verMajor; }
   inline int getVersionMinor() const { return m_verMinor; }
   
   bool addAction(CUPnPActionDesc *action);
   
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   std::string serialize();
   
   static CUPnPService *create();
private:
   std::string m_type;
   std::string m_id;
   std::string m_scpdPath;
   std::string m_ctrlUrl;
   std::string m_eventUrl;
   std::string m_scpd;
   
   int m_verMajor;
   int m_verMinor;
   
   bool m_refreshScpd;
   
   std::map<std::string, CUPnPActionDesc *> m_actionList;
};


#endif // CUPNPSERVICE_H
