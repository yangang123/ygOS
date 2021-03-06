#ifndef _STDINT_H
#define _STDINT_H

#ifndef LINUX

#include <stdint.h>
#include <stdlib.h>

#else 

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef  char int8_t;
typedef  short int16_t;
typedef  int int32_t;

#endif 

#endif 
