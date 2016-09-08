//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkItem

#ifndef CHUNK_ITEM_H
#define CHUNK_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsItem.h"
#include "ChunkData.h"

namespace btrForensics{
    //! Chunk item data.
    class ChunkItem : public BtrfsItem {
    public:
        ChunkData data; //!< Data part of chunk item.

    public:
        ChunkItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~ChunkItem() = default; //!< Destructor

        std::string dataInfo() const override;
    };
}

#endif

