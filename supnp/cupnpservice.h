#ifndef CUPNPSERVICE_H
   #define CUPNPSERVICE_H

#include <string>
#include <map>

class CUPnPActionDesc;

class CUPnPService
{
public:
   CUPnPService(const std::string &type,
                const std::string &id);
   
   bool addAction(CUPnPActionDesc *action);
   
   inline std::string getType() const { return m_type; }
   inline std::string getId() const { return m_id; }
private:
   std::string m_type;
   std::string m_id;
   std::map<std::string, CUPnPActionDesc *> m_actionList;
};


#endif // CUPNPSERVICE_H
