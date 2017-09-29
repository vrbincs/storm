#include <cstdint>
#include <sys/time.h>
#include <iostream>

#include "cupnphelper.h"

uint32_t CUPnPHelper::getCurrentLocalTime()
{
   struct timeval tp;
   gettimeofday(&tp, NULL);
   uint32_t time_now_sec = tp.tv_sec + ( tp.tv_usec / 1000000 );
   return time_now_sec;
}
