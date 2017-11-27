//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsExaminer.

#include <iostream>
#include "BtrfsExaminer.h"

using namespace std;

namespace btrForensics {

    //! Constructor
    BtrfsExaminer::BtrfsExaminer(TSK_IMG_INFO *img, TSK_ENDIAN_ENUM end, vector<TSK_OFF_T> devOffsets)
        :image(img), endian(end)
    {
        uint64_t devCount(0);
        for(auto dev_off : devOffsets) {
            char *diskArr = new char[SuperBlock::SUPBLK_SIZE]();
            tsk_img_read(img, dev_off + SuperBlock::SUPBLK_ADDR, 
                    diskArr, SuperBlock::SUPBLK_SIZE);
            SuperBlock *supblk = new SuperBlock(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

            if(fsUUID.isUnused()) {
                fsUUID = supblk->fsUUID;
            }
            else if(fsUUID != supblk->fsUUID) {
                throw FsDeviceException("Found superblocks do not belong to the same pool.");
            }

            const DevData *dev = &(supblk->devItemData);
            deviceTable[dev->deviceId] = new DeviceRecord(dev->deviceId,
                                            dev_off, dev->devUUID, supblk);

            devCount = supblk->numDevices;
        }

        if(devOffsets.size() == devCount) {
            cout << "All devices accepted. Device number: " << devCount << std::endl;
            cout << "Pool UUID: " << fsUUID.encode() << endl;
            cout << "-----------------------------------" << endl;

            for(const auto dev : deviceTable) {
                cout << (dev.second)->devInfo() << endl;
            }
        }
        else
            throw FsDeviceException("Input incomplete: device(s) missing.");
        
        primarySupblk = deviceTable[1]->superBlk;

        initializeChunkTree();

        cout << *primarySupblk << endl;

        cout << chunkTree->chunkRoot->info() << endl;
    }


    //! Destructor
    BtrfsExaminer::~BtrfsExaminer()
    {
        if(primarySupblk != nullptr)
            delete primarySupblk;
        for(auto &record : deviceTable) {
            delete record.second;
            record.second = nullptr;
        }
    }


    //! Find device offset within the image in bytes
    //! 
    //! \param devId
    //!
    //! \return Device offset in bytes.
    //!
    uint64_t BtrfsExaminer::getDevOffset(const uint64_t devId)
    {
        return deviceTable[devId]->deviceOffset;
    }


    //! Caculate the physical address from a chunk item.
    //!
    //! \param logicalAddr Logical address to convert.
    //! \param key Key storing chunk logical address.
    //! \param chunkData Chunk item data storing corresponding physical address.
    //!
    //! \return Mapped physical address. 0 if not valid.
    //!
    vector<uint64_t> BtrfsExaminer::getAddrFromChunk(uint64_t logicalAddr,
            const BtrfsKey* key, const ChunkData* chunkData)
    {
        vector<uint64_t> physicalAddrs;
        uint64_t chunkLogical = key->offset; //Key offset stores logical address.

        //Input logical address should be larger than chunk logial address.
        if(logicalAddr < chunkLogical)
            throw FsDamagedException("Superblock chunk item error. Unable to map logical address to physical address.");

        //There should be at least one stripe within the chunk data.
        if(chunkData->numStripe == 0)
            throw FsDamagedException("Superblock chunk item error. No stripe found.");
        else if(chunkData->numStripe > 1)
            throw FsDamagedException("Test only: This chunk has more than 1 stripes!");

        for(const auto &stripe : chunkData->btrStripes) {
            //Get device offset from stripe device ID
            uint64_t deviceOffset = getDevOffset(stripe->deviceId);
            //Data offset stores relative physical address.
            uint64_t chunkPhysical = deviceOffset + stripe->offset; 
        
            //cout << "++++++++++++++++++" << endl;
            //cout << "Logical address: " << logicalAddr << endl;
            //cout << "Chunk logical: " << chunkLogical << endl;
            //cout << "Chunk physical: " << chunkPhysical << endl;
            //cout << "++++++++++++++++++" << endl;

            physicalAddrs.push_back(logicalAddr - chunkLogical + chunkPhysical);
        }

        return physicalAddrs;
    }


    uint64_t BtrfsExaminer::getTempAddrFromChunk(uint64_t logicalAddr,
            const BtrfsKey* key, const ChunkData* chunkData)
    {
        return getAddrFromChunk(logicalAddr, key, chunkData)[0];
    }


    //! Read data from image based on given logical address.
    //!
    //! \param data Array with data read from image
    //! \param logicalAddr Logical address of data.
    //! \param key Btrfs chunk item key.
    //! \param chunkData Btrfs chunk item data.
    //! \param size Size of data.
    //!
    //! \return Starting physcial address of data in the image.
    //! 
    uint64_t BtrfsExaminer::readData(char *data, const uint64_t logicalAddr,
            const BtrfsKey* key, const ChunkData* chunkData, const uint64_t size)
    {
        vector<uint64_t> physicalAddrs = getAddrFromChunk(logicalAddr, key, chunkData);

        uint64_t readSize(0);
        uint64_t length = chunkData->stripeLength;
        for(const auto &addr: physicalAddrs) {
            //cout << "------------------" << endl;
            //cout << "Physical addr: " << hex << addr << endl;
            //cout << "Total size: " << hex << size << endl;
            //cout << "Read size: " << hex << readSize << endl;
            //cout << "Stripe length: " << hex << length << endl;
            //cout << "------------------" << endl;
            if(size - readSize <= length) {
                tsk_img_read(image, addr, data + readSize, size);
                break;
            }
            else {
                tsk_img_read(image, addr, data + readSize, length);
                readSize += length;
            }
        }

        return physicalAddrs[0];
    }


    void BtrfsExaminer::initializeChunkTree()
    {
        chunkTree = new ChunkTree(this);
    }

}

