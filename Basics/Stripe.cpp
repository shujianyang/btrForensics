//! \file
//! \author Shujian Yang
//!
//! Stripe infomation.

#include <sstream>
#include "Stripe.h"
#include "Utility/ReadInt.h"

namespace btrForensics{
  //! Constructor of device item.
  //! 
  //! \param endian The endianess of the array.
  //! \param arr Byte array storing device item data.
  //!
  Stripe::Stripe(TSK_ENDIAN_ENUM endian, uint8_t arr[])
      :devUUID(endian, arr + 0x10)
  {
      int arIndex(0);

      deviceId = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;

      offset = read64Bit(endian, arr + arIndex);
      arIndex += 0x08;
  }

  //! Return infomation about the stripe data as string
  std::string Stripe::dataInfo() const
  {
      std::ostringstream oss;
      oss << std::dec << "Device ID: " << deviceId << '\n';
      oss << std::uppercase << std::hex;
      oss << "Stripe offset: " << offset << '\n';
      oss << "Device UUID: " << devUUID.encode() << '\n';
      return oss.str();
  }
}

