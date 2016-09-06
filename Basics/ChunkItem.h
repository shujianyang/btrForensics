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
        ChunkData data;

    public:
        ChunkItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        std::string dataInfo() const override;
    };
}

#endif

