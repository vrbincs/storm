#include <cstdint>
#include <time.h>

#include "cupnphelper.h"

uint32_t CUPnPHelper::getCurrentTimeUtc()
{
   uint32_t ticks = 0;
   struct timespec tp;
   clock_gettime(CLOCK_REALTIME, &tp);
   
   ticks = ((uint32_t)tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);
   return ticks;
}
