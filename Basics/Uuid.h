/** \file
  * Header file of UUID.
  */

#ifndef UUID_H
#define UUID_H

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


class UUID{
private:
    uint32_t data_1;
    uint16_t data_2;
    uint16_t data_3;
    uint8_t data_4[8];

private:

    Variant getVariant();
    int getVersion();

public:
    UUID(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
    UUID(TSK_ENDIAN_ENUM endian, gpt_entry &entry);

    bool isUnused();
    bool match(uint32_t, uint16_t, uint16_t, uint64_t);
    
    std::string encode();
    std::string guidType();
    std::string variantInfo();
    std::string versionInfo();

    static const int BYTES_OF_UUID = 16;
    static const int LENGTH_OF_UUID_STRING = 36;
};


#endif
