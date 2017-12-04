/**
 * \file
 * \author Shujian Yang
 *
 * File containing integer reading functions.
 */

#include <sstream>
#include "ReadInt.h"

namespace btrForensics {

/**
  * Read a 16-bit long number from byte array.
  *
  * \param endian The endianess of the array.
  * \param arr The array containg the bytes.
  *
  */
const uint16_t read16Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr)
{
    uint16_t num(0);

    if(endian == TSK_LIT_ENDIAN) {
        num += (uint16_t)*(arr + 1);
        num = num << 8;
        num += (uint16_t)*arr;
    }
    else if(endian == TSK_BIG_ENDIAN) {
        num += (uint16_t)*arr;
        num += num << 8;
        num += (uint16_t)*(arr + 1);
    }

    return num;
}

/**
  * Read a 32-bit long number from byte array.
  *
  * \param endian The endianess of the array.
  * \param arr The array containg the bytes.
  *
  */
const uint32_t read32Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr)
{
    uint32_t num(0);

    if(endian == TSK_LIT_ENDIAN) {
        for(int i=3; i>=0; i--){
            num <<= 8;
            num += (uint32_t)*(arr + i);
        }
    }
    else if(endian == TSK_BIG_ENDIAN) {
        for(int i=0; i<=3; i--){
            num <<= 8;
            num += (uint32_t)*(arr + i);
        }
    }

    return num;
}

/**
  * Read a 64-bit long number from byte array.
  *
  * \param endian The endianess of the array.
  * \param arr The array containg the bytes.
  *
  */
const uint64_t read64Bit(TSK_ENDIAN_ENUM endian, const uint8_t *arr)
{
    uint64_t num(0);

    if(endian == TSK_LIT_ENDIAN) {
        for(int i=7; i>=0; i--){
            num <<= 8;
            num += (uint64_t)*(arr + i);
        }
    }
    else if(endian == TSK_BIG_ENDIAN) {
        for(int i=0; i<=7; i--){
            num <<= 8;
            num += (uint64_t)*(arr + i);
        }
    }

    return num;
}


//! Convert bytes into human-friendly string format.
//!
//! \param bytes Byte number to convert.
//!
//! \return String of size in B, KB, MB...
//!
std::string humanSize(uint64_t bytes)
{
    std::string totalSfx;

    if(bytes >> 10 == 0){
        totalSfx = "B";
    }
    else if((bytes >>= 10) >> 10 == 0){
        totalSfx = "KB";
    }
    else if((bytes >>= 10) >> 10 == 0){
        totalSfx = "MB";
    }
    else {
        bytes >>= 10;
        totalSfx = "GB";
    }

    std::ostringstream oss;
    oss << bytes << totalSfx;
    return oss.str();
}

}

