//!
//! \file
//! \author Shujian Yang
//!
//! SuperBlock implementation.

#include <sstream>
#include "SuperBlock.h"
#include "Utility/ReadInt.h"
#include "Examiners/Functions.h"

namespace btrForensics{
    
    //! Constructor of super block.
    //! 
    //! \param endian The endianess of the array.
    //! \param arr Byte array storing super block data.
    //! 
    SuperBlock::SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[])
        :fsUUID(endian, arr + 0x20), devItemData(endian, arr + 0xc9),
         chunkKey(endian, arr + 0x32b), chunkData(endian, arr + 0x33c)
    {
        int arIndex(0);
        for(int i=0; i<0x20; i++){
            checksum[i] = arr[arIndex++];
        }

        arIndex += 0x10; //fsUUID, initialized ahead.

        address = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        for(int i=0; i<0x08; i++){
            flags[i] = arr[arIndex++];
        }

        for(int i=0; i<0x08; i++, arIndex++){
            magic[i] = arr[arIndex];
        }

        generation = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;
        
        rootTrRootAddr = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        chunkTrRootAddr = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        logTrRootAddr = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        logRootTransid = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        totalBytes = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        bytesUsed = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        rootDirObjectid = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        numDevices = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        sectorSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        nodeSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        leafSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        stripeSize = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        n = read32Bit(endian, arr + arIndex);
        arIndex += 0x04;

        chunkRootGeneration = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        compatFlags = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        compatRoFlags = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        imcompatFlags = read64Bit(endian, arr + arIndex);
        arIndex += 0x08;

        csumType = read16Bit(endian, arr + arIndex);
        arIndex += 0x2;

        rootLevel = arr[arIndex++];
        chunkRootLevel = arr[arIndex++];
        logRootLevel = arr[arIndex++];

        arIndex += DEV_ITEM_SIZE;

        for(int i=0; i<LABEL_SIZE; i++){
            label[i] = arr[arIndex++];
        }
    }


    //! Get chunk tree root address from superblock.
    //!
    //! \return 8-byte chunk tree root physical address.
    //!
    const uint64_t SuperBlock::getChunkPhyAddr() const
    {
        return getChunkAddr(chunkTrRootAddr, &chunkKey, &chunkData);
    }


    //! Get root tree root address from superblock.
    //!
    //! \return 8-byte root tree root logical address.
    //!
    const uint64_t SuperBlock::getRootLogAddr() const
    {
        return rootTrRootAddr;
    }


    //! Get magic words of btrfs system.
    const std::string SuperBlock::printMagic() const
    {
        return std::string(magic, 8);
    }


    //! Get info about partition space usage.
    const std::string SuperBlock::printSpace() const
    {
        uint64_t total = totalBytes;
        std::string totalSfx;

        if(total >> 10 == 0){
            totalSfx = "B";
        }
        else if((total >>= 10) >> 10 == 0){
            totalSfx = "KB";
        }
        else if((total >>= 10) >> 10 == 0){
            totalSfx = "MB";
        }
        else{
            total >>= 10;
            totalSfx = "GB";
        }

        uint64_t used = bytesUsed;
        std::string usedSfx;

        if(used >> 10 == 0){
            usedSfx = "B";
        }
        else if((used >>= 10) >> 10 == 0){
            usedSfx = "KB";
        }
        else if((used >>= 10) >> 10 == 0){
            usedSfx = "MB";
        }
        else{
            used >>= 10;
            usedSfx = "GB";
        }

        std::ostringstream oss;
        oss << "Total size: " << total << totalSfx;
        oss << '\n';
        oss << "Used size: " << used << usedSfx;

        return oss.str();
    }


    //! Get super block label.
    const std::string SuperBlock::printLabel() const
    {
        std::ostringstream oss;
        for(int i=0; i<LABEL_SIZE; i++){
            if(label[i] == 0) break;
            oss << label[i];
        }
        return oss.str();
    }


    //! Overloaded stream operator.
    std::ostream &operator<<(std::ostream &os, SuperBlock &supb)
    {
        os << "File system UUID: " << supb.fsUUID.encode()
            << std::uppercase << std::hex;
        os << "\nRoot tree root address: 0x";
        os.fill('0');
        os.width(16);
        os << supb.rootTrRootAddr;
        os << "\nChunk tree root address: 0x";
        os.width(16);
        os << supb.chunkTrRootAddr;    
        os << "\nLog tree root address: 0x";
        os.width(16);
        os << supb.logTrRootAddr << '\n';
        os << std::dec;
        os << '\n' << "Unit size:" << '\n';
        os << "Sector\tNode\tLeaf\tStripe" << '\n';
        os << supb.sectorSize << "\t" << supb.nodeSize << "\t" << 
            supb.leafSize << "\t" << supb.stripeSize;
        os << std::endl;

        return os;
    }

}

