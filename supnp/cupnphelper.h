#ifndef CUPNPHELPER_H
   #define CUPNPHELPER_H

class CUPnPHelper
{
public:
   /**
    * \brief Returns current UTC time in miliseconds
    */
   static uint32_t getCurrentLocalTime();
private:
   CUPnPHelper(){}
   virtual ~CUPnPHelper(){}
};

#endif // CUPNPHELPER_H
