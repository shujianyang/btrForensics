//! \file
//! \author Shujian Yang
//!
//! Implementation of class InternalNode.

#include <sstream>
#include "InternalNode.h"

namespace btrForensics{
    //! Constructor of btrfs leaf node.
    //!
    //! \param img Image file.
    //! \param imgOffset Offset to the partition.
    //! \param header Pointer to header of a node.
    //! \param endian The endianess of the array.
    //! \param startOffset Offset of the node, right after header.
    //!
    InternalNode::InternalNode(TSK_IMG_INFO *img, TSK_OFF_T imgOffset, const BtrfsHeader *header, 
            TSK_ENDIAN_ENUM endian, uint64_t startOffset)
        :BtrfsNode(header)
    {
        char *diskArr;
        uint64_t itemOffset(0);
        uint32_t itemNum = header -> getNumOfItems();

        for(uint32_t i=0; i<itemNum; ++i){
            diskArr = new char[KeyPtr::SIZE_OF_KEY_PTR]();
            tsk_img_read(img, imgOffset + startOffset + itemOffset,
                    diskArr, KeyPtr::SIZE_OF_KEY_PTR);

            keyPointers.push_back(new KeyPtr(endian, (uint8_t*)diskArr));
            delete [] diskArr;

            itemOffset += KeyPtr::SIZE_OF_KEY_PTR;
        }
    }


    //! Destructor
    InternalNode::~InternalNode()
    {
        for(auto ptr : keyPointers)
            delete ptr;
    }


    //! Print info about this node.
    const std::string InternalNode::info() const
    {
        std::ostringstream oss;

        oss << *nodeHeader << "\n\n";
        oss << "Item list:" << '\n';
        oss << std::string(30, '=') << "\n\n";

        for(auto ptr : keyPointers){
            oss << *ptr;
            oss << std::string(30, '=') << "\n\n";
        }

        return oss.str();
    }

}
