/** \file
 * Implementation of class DirIndex
 */

#include <sstream>
#include "DirIndex.h"
#include "Utility/ReadInt.h"

namespace btrForensics{

    /**
     * Constructor of dir index.
     *
     * \param endian The endianess of the array.
     * \param arr Byte array storing dir index data.
     *
     */
    DirIndex::DirIndex(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :key(endian, arr)
    {
        int arIndex(BtrfsKey::SIZE_OF_KEY); //Key initialized already.
        transId = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        dataSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        nameSize = read16Bit(endian, arr + arIndex);
        arIndex += 0x02;
        
        childType = arr[arIndex++];

        dirName = new char[nameSize]();
        for(int i=0; i < nameSize; ++i){
            dirName[i] = arr[arIndex++];
        }

        //Directory data not processed yet.
    }


    /**
     * Return name of the directory.
     */
    std::string DirIndex::getDirName() const
    {
        return std::string(dirName, nameSize);
    }


    /**
     * Return infomation about the item data as string.
     */
    std::string DirIndex::info() const
    {
        std::ostringstream oss;
        oss << key;
        oss << "Type: " << (int)childType << '\n';
        oss << "Name: " << getDirName();
        return oss.str();
    }
}

