#ifndef CUPNPACTION_H
   #define CUPNPACTION_H

class xmlNode;

enum UPnPType
{
   UPnPType_UI4,
   UPnPType_String
};

class CUPnPAction
{
public:
   CUPnPAction(const std::string &actionName);
   bool addArg(const std::string &argumentName,
               UPnPType type,
               bool isDirectionIn);
   bool addArg(const CUPnPArg &arg);

   std::string getName() const;
   bool serialize(xml_node<> *xmlNode);
protected:
   CUPnPAction();
private:
   std::string m_actionName;
   std::map<std::string, CUPnPArg *> m_args;
};

#endif // CUPNPACTION_H
