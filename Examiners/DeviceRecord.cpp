//! \file
//! \author Shujian Yang
//!
//! Implementation of class DeviceRecord.

#include <sstream>
#include "DeviceRecord.h"

namespace btrForensics {

    //! Constructor of DeviceRecord
    DeviceRecord::DeviceRecord(uint64_t id, uint64_t offset, UUID uuid, SuperBlock *supblk)
        :deviceId(id), deviceOffset(offset), devUUID(uuid), superBlk(supblk)
    {
    }

    //! Print device info.
    std::string DeviceRecord::devInfo() const
    {
        std::ostringstream oss;
        oss << "Device ID: " << deviceId << '\n';
        oss << "Device UUID: " << devUUID.encode() << '\n';

        return oss.str();
    }
}

