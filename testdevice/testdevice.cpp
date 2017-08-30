#include <iostream>
#include <memory>
#include <unistd.h>

#include <logger.h>
#include <cupnpdevice.h>

int main(int argc, char *argv[])
{
   CUPnPDevice *upnp = CUPnPDevice::create("saso.html");
   
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
