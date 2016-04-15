/** \file
  * Header file of GUID.
  */

#ifndef GUID_H
#define GUID_H

#include <string>
#include <tsk/libtsk.h>
#include <tsk/vs/tsk_gpt.h>


enum Variant{
    NETWORK_COMPUTING_SYSTEM,
    STANDARD,
    MS_COM,
    RESERVED
};

enum Version{
    MAC_DATE = 1,
    DCE,
    MD5_NAMESPACE,
    RANDOM,
    SHA_NAMESPACE,
    UNKNOWN
};


class GUID{
private:
    uint32_t data_1;
    uint16_t data_2;
    uint16_t data_3;
    uint8_t data_4[8];

private:
    uint16_t read16Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr);
    uint32_t read32Bit(TSK_ENDIAN_ENUM endian, uint8_t *arr);

    Variant getVariant();
    int getVersion();

public:
    GUID(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
    GUID(TSK_ENDIAN_ENUM endian, gpt_entry &entry);

    bool isUnused();
    bool match(uint32_t, uint16_t, uint16_t, uint64_t);
    
    std::string encode();
    std::string guidType();
    std::string variantInfo();
    std::string versionInfo();

    static const int BYTES_OF_GUID = 16;
    static const int LENGTH_OF_GUID_STRING = 36;
};


#endif
