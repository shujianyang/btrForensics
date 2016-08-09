/** \file
  * SuperBlock implementation.
  */

#include "DevItem.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

  /**
    * Constructor of device item.
    * 
    * \param endian The endianess of the array.
    * \param arr Byte array storing device item data.
    * 
    */
  DevItem::DevItem(TSK_ENDIAN_ENUM endian, uint8_t arr[])
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


  /**
     * Return infomation about the item data as string.
     */
    std::string DevItem::info() const
    {
        return "";
    }
}
