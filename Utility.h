/** \file
  * File containing useful functions.
  */

#include <tsk/libtsk.h>


/**
  * Read a 16-bit long number from byte array.
  *
  * \param endian The endianess of the array.
  * \param arr The array containg the bytes.
  *
  */
uint16_t read16Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr)
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
uint32_t read32Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr)
{
    uint32_t num(0);

    if(endian == TSK_LIT_ENDIAN) {
        num += (uint16_t)*(arr + 3);
        num = num << 8;
        num += (uint16_t)*(arr + 2);
        num = num << 8;
        num += (uint16_t)*(arr + 1);
        num = num << 8;
        num += (uint16_t)*arr;
    }
    else if(endian == TSK_BIG_ENDIAN) {
        num += (uint16_t)*arr;
        num += num << 8;
        num += (uint16_t)*(arr + 1);
        num += num << 8;
        num += (uint16_t)*(arr + 2);
        num += num << 8;
        num += (uint16_t)*(arr + 3);
    }

    return num;
}

