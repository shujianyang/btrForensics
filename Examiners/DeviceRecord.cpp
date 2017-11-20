//! \file
//! \author Shujian Yang
//!
//! Implementation of class DeviceRecord.

#include "DeviceRecord.h"

namespace btrForensics {

    //! Constructor of DeviceRecord
    DeviceRecord::DeviceRecord(uint64_t id, uint64_t offset, UUID uuid)
        :deviceId(id), deviceOffset(offset), devUUID(uuid)
    {
    }
}

