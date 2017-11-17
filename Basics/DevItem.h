//! \file
//! \author Shujian Yang
//!
//! Header file of class DevItem

#ifndef DEV_ITEM_H
#define DEV_ITEM_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>
#include "BtrfsItem.h"
#include "DevData.h"

namespace btrForensics{
    //! Dev item data.
    class DevItem : public BtrfsItem {
    private:
        DevData data;

    public:
        DevItem(ItemHead* head, TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~DevItem() = default; //!< Destructor

        std::string dataInfo() const override;
    };
}

#endif

