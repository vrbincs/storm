#ifndef LOGGER_H
   #define LOGGER_H

#include <iostream>
#define LOGGER_INFO(msg) std::cout << msg << std::endl;
#define LOGGER_ERROR(msg) LOGGER_INFO(msg)
#define LOGGER_WARN(msg) LOGGER_INFO(msg)
#define LOGGER_TRACE(msg) LOGGER_INFO(msg)

#endif // LOGGER_H
