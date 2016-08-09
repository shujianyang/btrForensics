/** \file
  * Header file of class ItemData
  */

#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <iostream>
#include <string>
#include <tsk/libtsk.h>

namespace btrForensics{
    /** Item data, this is an abstract class. */
    class ItemData{
    public:
        //ItemData(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        
        friend std::ostream &operator<<(std::ostream &os, const ItemData &itmData)
        {
            os << itmData.info() << '\n';
            return os;
        }
        

        virtual std::string info() const = 0;

        virtual ~ItemData() = default;
    };
}

#endif

