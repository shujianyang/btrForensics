/** \file
  * Header file of DevItem.
  */

#ifndef DEV_ITEM
#define DEV_ITEM

#include "Utility/Uuid.h"
#include "ItemData.h"

namespace btrForensics{
    /** Device item data. */
    class DevItem: public ItemData {
    private:
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
        DevItem(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        std::string info() const override;
    };
}

#endif
