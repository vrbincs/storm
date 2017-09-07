#ifndef CUPNPACTIONDESC_H
   #define CUPNPACTIONDESC_H

class CUPnPActionDesc
{
public:
   CUPnPActionDesc();
   virtual ~CUPnPActionDesc();
   
   bool deserialize(rapidxml::xml_node<> *xmlNode);
};

#endif // CUPNPACTIONDESC_H
