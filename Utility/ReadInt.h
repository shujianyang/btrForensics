#ifndef UTILITY_H
#define UTILITY_H

/** \file
  * Header file of utility.
  */

#include <tsk/libtsk.h>


uint16_t read16Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr);

uint32_t read32Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr);

uint64_t read64Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr);

#endif
