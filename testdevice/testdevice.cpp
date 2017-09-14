#include <iostream>
#include <memory>
#include <unistd.h>

#include <logger.h>
#include <cupnpdevice.h>
#include <cmediaserverdelegate.h>

int main(int argc, char *argv[])
{
   CMediaServerDelegate mediaServerDelegate("uuid:1a2c717b-2f9a-4ff6-9f6c-8ad115508b99",
                                            "TestDeviceSaso",
                                            "Sasho Vrbinc",
                                            "https://github.com/vrbincs/");
   
   CUPnPDevice *upnp = CUPnPDevice::create(&mediaServerDelegate, "/");
   
   if(upnp)
   {
      while(upnp->run())
      {
         usleep(100000);
      }
      
      delete upnp;
   }
   else
   {
      std::cout << "Unable to start UPnP service." << std::endl;
   }
   
   
   return 0;
}
