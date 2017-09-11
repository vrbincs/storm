#include <supnp.h>

#include "cmediaserverdelegate.h"

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define RESOURCE_MEDIA_SERVER_ROOT STR(RESOURCE_PATH)"/resources/mediaServerRoot.xml"
#define RESOURCE_MEDIA_SERVER_SCPD STR(RESOURCE_PATH)"/resources/mediaServerSCPD.xml"


int main(int argc, char *argv[])
{
   CUPnPDevice mediaServerDevice();
   return 0;
}
