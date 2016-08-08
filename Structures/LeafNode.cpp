/** \file
 * Implementation of class LeafNode.
 */

#include "LeafNode.h"

namespace BtrfsForensics{
    /**
     * Constructor of btrfs leaf node.
     *
     * \param header Pointer to header of a node.
     * \param endian The endianess of the array.
     * \param arr Byte array storing internal node data.
     *
     */
    LeafNode::LeafNode(BtrfsHeader *header, 
            TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :BtrfsNode(header)
    {
        uint32_t itemNum = header -> getNumOfItems();
        int arIndex(0);

        for(uint32_t i=0; i<itemNum; ++i){
            keyPointers.push_back(KeyPtr(endian, arr[arIndex]));
            arIndex += KeyPtr::SIZE_OF_KEY_PTR;
        }
    }
}
