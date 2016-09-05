//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsKey

#include "BtrfsKey.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    ///! Constructor of key.
    //! 
    //! \param endian The endianess of byte array.
    //! \param arr Byte array storing key data.
    //!
    BtrfsKey::BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[])
    {
        int arIndex = 0;

        objId = read64Bit(endian, arr + arIndex);
        arIndex += 8;

        uint8_t type = arr[arIndex++];

        offset = read64Bit(endian, arr + arIndex);

        switch(type){
            case 0x01:
                itemType = ItemType::INODE_ITEM;
                break;
            case 0x0c:
                itemType = ItemType::INODE_REF;
                break;
            case 0x0d:
                itemType = ItemType::INODE_EXTREF;
                break;
            case 0x18:
                itemType = ItemType::XATTR_ITEM;
                break;
            case 0x30:
                itemType = ItemType::ORPHAN_ITEM;
                break;
            case 0x3c:
                itemType = ItemType::DIR_LOG_ITEM;
                break;
            case 0x48:
                itemType = ItemType::DIR_LOG_INDEX;
                break;
            case 0x54:
                itemType = ItemType::DIR_ITEM;
                break;
            case 0x60:
                itemType = ItemType::DIR_INDEX;
                break;
            case 0x6c:
                itemType = ItemType::EXTENT_DATA;
                break;
            case 0x80:
                itemType = ItemType::EXTENT_CSUM;
                break;
            case 0x84:
                itemType = ItemType::ROOT_ITEM;
                break;
            case 0x90:
                itemType = ItemType::ROOT_BACKREF;
                break;
            case 0x9c:
                itemType = ItemType::ROOT_REF;
                break;
            case 0xa8:
                itemType = ItemType::EXTENT_ITEM;
                break;
            case 0xb0:
                itemType = ItemType::TREE_BLOCK_REF;
                break;
            case 0xb2:
                itemType = ItemType::EXTENT_DATA_REF;
                break;
            case 0xb4:
                itemType = ItemType::EXTENT_REF_V0;
                break;
            case 0xb6:
                itemType = ItemType::SHARED_BLOCK_REF;
                break;
            case 0xb8:
                itemType = ItemType::SHARED_DATA_REF;
                break;
            case 0xc0:
                itemType = ItemType::BLOCK_GROUP_ITEM;
                break;
            case 0xcc:
                itemType = ItemType::DEV_EXTENT;
                break;
            case 0xd8:
                itemType = ItemType::DEV_ITEM;
                break;
            case 0xe4:
                itemType = ItemType::CHUNK_ITEM;
                break;
            case 0xfd:
                itemType = ItemType::STRING_ITEM;
                break;
            default:
                itemType = ItemType::UNKNOWN;
        }
    }



    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream &os, const BtrfsKey &key)
    {
        os << "Key - Object id: " << std::dec << key.objId << '\n';
        os << std::uppercase << std::hex;
        os << "Key - Item type: " << key.getItemTypeStr() << '\n';
        os << "Key - Offset: 0x" << key.offset << '\n';
    }


    //! Get type description of the item.
    //!
    //! \return Item type in string.
    //!
    const std::string BtrfsKey::getItemTypeStr() const
    {
        std::string type;
        switch(itemType){
            case ItemType::INODE_ITEM:
                type = "inode item";
                break;
            case ItemType::INODE_REF:
                type = "inode ref";
                break;
            case ItemType::INODE_EXTREF:
                type = "inode extref";
                break;
            case ItemType::XATTR_ITEM:
                type = "xattr item";
                break;
            case ItemType::ORPHAN_ITEM:
                type = "orphan item";
                break;
            case ItemType::DIR_LOG_ITEM:
                type = "dir log item";
                break;
            case ItemType::DIR_LOG_INDEX:
                type = "dir log index";
                break;
            case ItemType::DIR_ITEM:
                type = "dir item";
                break;
            case ItemType::DIR_INDEX:
                type = "dir index";
                break;
            case ItemType::EXTENT_DATA:
                type = "extent data";
                break;
            case ItemType::EXTENT_CSUM:
                type = "extent csum";
                break;
            case ItemType::ROOT_ITEM:
                type = "root item";
                break;
            case ItemType::ROOT_BACKREF:
                type = "root backref";
                break;
            case ItemType::ROOT_REF:
                type = "root ref";
                break;
            case ItemType::EXTENT_ITEM:
                type = "extent item";
                break;
            case ItemType::TREE_BLOCK_REF:
                type = "tree block ref";
                break;
            case ItemType::EXTENT_DATA_REF:
                type = "extent data ref";
                break;
            case ItemType::EXTENT_REF_V0:
                type = "extent ref v0";
                break;
            case ItemType::SHARED_BLOCK_REF:
                type = "shared block ref";
                break;
            case ItemType::SHARED_DATA_REF:
                type = "shared data ref";
                break;
            case ItemType::BLOCK_GROUP_ITEM:
                type = "block group item";
                break;
            case ItemType::DEV_EXTENT:
                type = "dev extent";
                break;
            case ItemType::DEV_ITEM:
                type = "dev item";
                break;
            case ItemType::CHUNK_ITEM:
                type = "chunk item";
                break;
            case ItemType::STRING_ITEM:
                type = "string item";
                break;
            default:
                type = "unknown";
                break;
        }

        return type;
    }
}
