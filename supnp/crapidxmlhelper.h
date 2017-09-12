#ifndef CRAPIDXMLHELPER_H
   #define CRAPIDXMLHELPER_H

#include <rapidxml.hpp>

class CRapidXmlHelper
{
public:
   CRapidXmlHelper(rapidxml::xml_node<> *node)
      : m_node(node)
   {
   }
   
   inline rapidxml::xml_attribute<> *findAttribute(const std::string &name)
   {
      for (rapidxml::xml_attribute<> *attr = m_node->first_attribute(); attr; attr = attr->next_attribute())
      {
         if(attr->name() == name)
         {
            return attr;
         }
      }
      
      return NULL;
   }
   
   inline std::string getAttributeValue(const std::string &name)
   {
      std::string str_t;
      
      rapidxml::xml_attribute<> *attr = findAttribute(name);
      if(attr)
      {
         str_t = attr->value();
      }
      
      return str_t;
   }
private:
   rapidxml::xml_node<> *m_node;
};

#endif // CRAPIDXMLHELPER_H
