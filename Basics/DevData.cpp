//! \file
//! \author Shujian Yang
//!
//! Dev item infomation.

#include <sstream>
#include "DevData.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

  //! Constructor of device item.
  //! 
  //! \param endian The endianess of the array.
  //! \param arr Byte array storing device item data.
  //!
  DevData::DevData(TSK_ENDIAN_ENUM endian, uint8_t arr[])
      :devUUID(endian, arr + 0x42), fsUUID(endian, arr + 0x52)
  {
      int arIndex(0);

      deviceId = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      bytes = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      bytesUsed = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      optIOAlign = read32Bit(endian, arr + arIndex);
      arIndex += 0x04;

      optIOWidth = read32Bit(endian, arr + arIndex);
      arIndex += 0x04;

      minimalIOSize = read32Bit(endian, arr + arIndex);
      arIndex += 0x04;

      type = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      generation = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      offset = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      devGroup = read32Bit(endian, arr + arIndex);
      arIndex += 0x04;

      seekSpeed = arr[arIndex++];
      bandWidth = arr[arIndex++];
  }

  //! Return infomation about the item data as string
  std::string DevData::dataInfo() const
  {
      std::ostringstream oss;
      oss << "Device ID: " << deviceId << '\n';
      oss << "Device UUID: " << devUUID.encode() << '\n';
      oss << std::dec;
      oss << "Total bytes: " << humanSize(bytes) << '\n';
      oss << "Bytes used: " << humanSize(bytesUsed) << '\n';
      oss << "File System UUID: " << fsUUID.encode() << '\n';
      return oss.str();
  }
}
