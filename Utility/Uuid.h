//! \file
//! \author Shujian Yang
//!
//! Header file of UUID.

#ifndef UUID_H
#define UUID_H

#include <string>
#include <tsk/libtsk.h>
#include <tsk/vs/tsk_gpt.h>

namespace btrForensics {

//! Universally Unique IDentifier (UUID).
class UUID{
public:
    //! Types of UUID
    enum Variant {
        NETWORK_COMPUTING_SYSTEM, //!< Network Computing System
        STANDARD, //!< RFC 4122 Standard
        MS_COM, //!< Microsoft COM
        RESERVED //!< Reserved
    };

    //! Different versions of UUID.
    enum Version {
        MAC_DATE = 1, //!< Time-based version
        DCE,  //!< DEC Security version
        MD5_NAMESPACE,  //!< Name-based version using MD5
        RANDOM,  //!< Randomly or pseudo-randomly generated version
        SHA_NAMESPACE,  //!< Name-based version using SHA-1
        UNKNOWN
    };
    static constexpr int DATA_4_SIZE = 8; //!< Size of data 4 array.

private:
    uint32_t data_1;
    uint16_t data_2;
    uint16_t data_3;
    uint8_t data_4[DATA_4_SIZE];
    Variant uuidVar;
    Version uuidVer;
    
private:
    const int getVersion() const;

public:
    UUID();
    UUID(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
    UUID(TSK_ENDIAN_ENUM endian, gpt_entry &entry);
    UUID(const UUID &origin);
    ~UUID() = default; //!< Destructor

    const bool isUnused() const;
    const bool match(uint32_t, uint16_t, uint16_t, uint64_t) const;
    
    const std::string encode() const;
    const std::string guidType() const;
    const std::string variantInfo() const;
    const std::string versionInfo() const;

    friend bool operator==(const UUID &lhs, const UUID &rhs);
    friend bool operator!=(const UUID &lhs, const UUID &rhs);

    UUID& operator=(const UUID& rhs);

    static const int BYTES_OF_UUID = 16; //!< UUID byte length when stored in machine,
    static const int LENGTH_OF_UUID_STRING = 36; //!< String length used to represent a UUID.
};

}

#endif
