//! \file
//! \author Shujian Yang
//!
//! Header file of DevData.

#ifndef DEV_DATA
#define DEV_DATA

#include "Utility/Uuid.h"
#include "BtrfsItem.h"

namespace btrForensics{
    //! Device item data.
    class DevData {
    public:
        uint64_t deviceId; //0x0
        uint64_t bytes;
        uint64_t bytesUsed;

        uint32_t optIOAlign;
        uint32_t optIOWidth;
        uint32_t minimalIOSize;

        uint64_t type;
        uint64_t generation;
        uint64_t offset;
        uint32_t devGroup;

        uint8_t seekSpeed; //0x40
        uint8_t bandWidth;

        UUID devUUID;
        UUID fsUUID;

    public:
        DevData(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~DevData() = default; //!< Destructor

        std::string dataInfo() const;
    };
}

#endif
