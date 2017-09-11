#ifndef CUPNPACTIONARGUMENTDESC_H
   #define CUPNPACTIONARGUMENTDESC_H

#include <string>

#include <rapidxml.hpp>

class CUPnPActionArgumentDesc
{
public:
   static CUPnPActionArgumentDesc *create();
   static void destroy(CUPnPActionArgumentDesc *obj);
   
   bool deserialize(rapidxml::xml_node<> *xmlNode);
   
   inline void setName(const std::string &name)
   {
      m_name = name;
   }
   
   inline std::string getName() const
   {
      return m_name;
   }
   
   inline bool isOut() const
   {
      return m_isOut;
   }
   
   friend class CUPnPActionDesc;
protected:
   CUPnPActionArgumentDesc(const std::string &name = "",
                           bool isOut = false);
   
   virtual ~CUPnPActionArgumentDesc() {}
private:
   std::string m_name;
   bool m_isOut;
};

#endif // CUPNPACTIONARGUMENTDESC_H
