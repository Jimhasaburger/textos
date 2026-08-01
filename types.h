#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

// Replace standard definitions manually for a freestanding environment
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef __WINT_TYPE__      size_t; // Or simply use unsigned int / unsigned long

#endif