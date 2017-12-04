/**
 * \file
 * \author Shujian Yang
 *
 * Header file of read integer functions.
 */

#ifndef READINT_H
#define READINT_H

#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{

    const uint16_t read16Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

    const uint32_t read32Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

    const uint64_t read64Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr);

    std::string humanSize(uint64_t bytes);
}

#endif
