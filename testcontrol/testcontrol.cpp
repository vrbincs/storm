#include <iostream>

#include <cupnpctrl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   CUPnPCtrl *upnpCtrl = CUPnPCtrl::create();
   
   if(upnpCtrl)
   {
      while(true)
      {
         usleep(100000);
      }
   }
   else
   {
      std::cout << "Unable to start UPnP service." << std::endl;
   }
   
   return 0;
}
