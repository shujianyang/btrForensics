//! \file
//! \author Shujian Yang
//!
//! Header file of class DeviceRecord.

#ifndef DEVICE_RECORD_H
#define DEVICE_RECORD_H

#include <string>
#include "Utility/Uuid.h"
#include "Trees/SuperBlock.h"

namespace btrForensics {

    //! Device registered for a btrfs filesystem.
    class DeviceRecord {
    public:
        uint64_t deviceId; //!< ID of the device.
        uint64_t deviceOffset; //!< Offset of the device from the beginning of the image in bytes.
        UUID devUUID; //!< Device UUID
        SuperBlock *superBlk; //!< SuperBlock stored in thi device.

    public:
        std::string devInfo() const;

        DeviceRecord(uint64_t, uint64_t, UUID, SuperBlock*);
        ~DeviceRecord() = default; //!< Destructor
    };
}

#endif

