#ifndef CRAPIDXMLHELPER_H
   #define CRAPIDXMLHELPER_H

class CRapidXmlHelper
{
public:
   CRapidXmlHelper(rapidxml::xml_node<> &node)
      m_node(node)
   {
   }
   
   inline xml_attribute *findAttribute(const std::string &name)
   {
      for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
      {
         if(attr->name() == name)
         {
            return attr;
         }
      }
      
      return NULL;
   }
}

#endif // CRAPIDXMLHELPER_H
