//! \file
//! \author Shujian Yang
//!
//! Header file of class ItemData

#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{
    //! Item data, this is an abstract class.
    class ItemData {
    public:
        virtual ~ItemData() = default;
        
        //! Overloaded stream operator.
        friend std::ostream &operator<<(std::ostream &os, const ItemData &itmData)
        {
            os << itmData.info() << '\n';
            return os;
        }
        

        //! Return infomation about the item data.
        //! Virtual function to be overridden by derived classes.
        virtual std::string info() const = 0;
    };
}

#endif

