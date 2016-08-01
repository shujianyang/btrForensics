#ifndef READINT_H
#define READINT_H

/** \file
  * Header file of utility.
  */

#include <tsk/libtsk.h>


const uint16_t read16Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

const uint32_t read32Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

const uint64_t read64Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

#endif
