#ifndef CRAPIDXMLHELPER_H
   #define CRAPIDXMLHELPER_H

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

#include <cstring.h>
#include <cstring>

class CRapidXmlHelper
{
public:
   CRapidXmlHelper()
      :  m_document(NULL),
         m_node(NULL)
   {
      m_document = new rapidxml::xml_document<>();
      
      rapidxml::xml_node<> *decl = m_document->allocate_node(rapidxml::node_declaration);
      
      appendAttribute("version", "1.0", decl);
      appendAttribute("encoding", "UTF-8", decl);
      
      m_document->append_node(decl);
   }
   
   CRapidXmlHelper(rapidxml::xml_node<> *node)
      : m_document(NULL),
        m_node(node)
   {
   }
   
   virtual ~CRapidXmlHelper()
   {
      delete m_document;
   }
   
   inline rapidxml::xml_attribute<> *findAttribute(const std::string &name)
   {
      if(m_node)
      {
         for (rapidxml::xml_attribute<> *attr = m_node->first_attribute(); attr; attr = attr->next_attribute())
         {
            if(attr->name() == name)
            {
               return attr;
            }
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
   
   inline std::string getNodeName()
   {
      if(m_node)
      {
         return m_node->name();
      }
      
      return std::string();
   }
   
   inline rapidxml::xml_node<> *next()
   {
      if(m_node)
      {
         m_node = m_node->first_node();
      }
      return m_node;
   }
   
   inline rapidxml::xml_node<> *node()
   {
      return m_node;
   }
   
   inline rapidxml::xml_node<> *nextSibling()
   {
      if(m_node)
      {
         m_node = m_node->next_sibling();
      }
      return m_node;
   }
   
   inline bool isValid() const
   {
      return m_node;
   }
   
   inline CString getNodeValue(const std::string &nodeName)
   {
      return CString(m_node->value());
   }
   
   inline std::string getNodeValue()
   {
      return m_node->value();
   }
   
   inline void appendAttribute(const char *name,
                               const char *value,
                               rapidxml::xml_node<> *node)
   {
      if(node && m_document)
      {
         node->append_attribute(m_document->allocate_attribute(name, value));
      }
   }
   
   inline rapidxml::xml_node<> *createNode(const char *name, const char *value = NULL)
   {
      if(m_document)
      {
         return m_document->allocate_node(rapidxml::node_element, name, value);
      }
      
      return NULL;
   }
   
   inline void appendNode(rapidxml::xml_node<> *node)
   {
      m_document->append_node(node);
   }
   
   inline void appendNode(rapidxml::xml_node<> *parent, rapidxml::xml_node<> *child)
   {
      parent->append_node(child);
   }
   
   inline std::string toString() const
   {
      std::string xml_as_string;
      
      if(m_document)
      {
         rapidxml::print(std::back_inserter(xml_as_string), *m_document);
      }
      
      return xml_as_string;
   }
   
   inline bool isNodeName(const char *name)
   {
      if(m_node && (strcmp(m_node->name(), name) == 0))
      {
         return true;
      }
      else
      {
         return false;
      }
   }
private:
   rapidxml::xml_document<> *m_document;
   rapidxml::xml_node<> *m_node;
};

#endif // CRAPIDXMLHELPER_H
