#ifndef CUPNPDMEDIASERVERDEVICE_H
   #define CUPNPDMEDIASERVERDEVICE_H

#include <cupnpdevice.h>

class CUPnPMediaServerDevice : CUPnPDevice
{
public:
   CUPnPMediaServerDevice(const std::string &uuid,
                          const std::string &friendlyName,
                          uint32_t port);

   
   
};


#endif // CUPNPDMEDIASERVERDEVICE_H
