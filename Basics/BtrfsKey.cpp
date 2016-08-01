/** \file
  * Implementation of class BtrfsKey
  */

#include "BtrfsKey.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    /**
     * Constructor of key.
     * 
     * \param endian The endianess of byte array.
     * \param arr Byte array storing key data.
     * 
     */
    BtrfsKey::BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[])
    {
        int arIndex = 0;

        objId = read64Bit(endian, arr + arIndex);
        arIndex += 8;

        itemType = arr[arIndex++];

        offset = read64Bit(endian, arr + arIndex);
    }



    /**
     * Overloaded stream operator.
     *
     */
    std::ostream &operator<<(std::ostream &os, const BtrfsKey &key)
    {
        os << std::uppercase << std::hex;
        os << "Key - Object id: " << key.objId << '\n';
        os << "Key - Item type: " << key.getItemTypeStr() << '\n';
        os << "Key - Offset: " << key.offset << '\n';
    }


    /**
     * Get type code of the item.
     *
     * \return Item type code.
     *
     */
    const uint8_t BtrfsKey::getItemType() const
    {
        return itemType;
    }


    /**
     * Get type description of the item.
     *
     * \return Item type in string.
     *
     */
    const std::string BtrfsKey::getItemTypeStr() const
    {
        std::string type;
        switch(itemType){
            case 0x01:
                type = "inode item";
                break;
            case 0x0c:
                type = "inode ref";
                break;
            case 0x0d:
                type = "inode extref";
                break;
            case 0x18:
                type = "xattr item";
                break;
            case 0x30:
                type = "orphan item";
                break;
            case 0x3c:
                type = "dir log item";
                break;
            case 0x48:
                type = "dir log index";
                break;
            case 0x54:
                type = "dir item";
                break;
            case 0x60:
                type = "dir index";
                break;
            case 0x6c:
                type = "extent data";
                break;
            case 0x80:
                type = "extent csum";
                break;
            case 0x84:
                type = "root item";
                break;
            case 0x90:
                type = "root backref";
                break;
            case 0x9c:
                type = "root ref";
                break;
            case 0xa8:
                type = "extent item";
                break;
            case 0xb0:
                type = "tree block ref";
                break;
            case 0xb2:
                type = "extent data ref";
                break;
            case 0xb4:
                type = "extent ref v0";
                break;
            case 0xb6:
                type = "shared block ref";
                break;
            case 0xb8:
                type = "shared data ref";
                break;
            case 0xc0:
                type = "block group item";
                break;
            case 0xcc:
                type = "dev extent";
                break;
            case 0xd8:
                type = "dev item";
                break;
            case 0xe4:
                type = "chunk item";
                break;
            case 0xfd:
                type = "string item";
                break;
            default:
                type = "unknown";
                break;
        }

        return type;
    }
}
