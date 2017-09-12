#include <supnp.h>

#include "cmediaserverdelegate.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define RESOURCE_MEDIA_SERVER_ROOT STR(RESOURCE_PATH)"/resources/mediaServerRoot.xml"
#define RESOURCE_MEDIA_SERVER_SCPD STR(RESOURCE_PATH)"/resources/mediaServerSCPD.xml"


int main(int argc, char *argv[])
{
   CMediaServerDelegate *mediaServerDelegate = new CMediaServerDelegate("bc610556-3559-413b-9953-853f1dec8cfa",
                                                                        "S Media Server",
                                                                        "saso.vrbinc@gmail.com",
                                                                        "https://github.com/vrbincs");

   ((void *) mediaServerDelegate);
   /*CUPnPDevice mediaServerDevice(const std::string &serviceDescPath = "",
                                 const std::string &host = "",
                                 uint32_t port = 90000
                                 mediaServerDelegate);*/
   
   
   
   return 0;
}
