//! \file
//! \author Shujian Yang
//!
//! Implementation of class BtrfsPool.

#include <map>
#include <sstream>
#include <iomanip>
#include "BtrfsPool.h"
#include "Functions.h"

using namespace std;

namespace btrForensics {

//! Constructor of Btrfs pool
//!
//! \param img Image file.
//! \param end The endianess of the array.
//! \param devOffsets Offsets of different devices
//! \param fsRootId Id of root of Filesystem Tree.
//!
BtrfsPool::BtrfsPool(TSK_IMG_INFO *img, TSK_ENDIAN_ENUM end,
        vector<TSK_OFF_T> devOffsets, uint64_t fsRootId)
    :image(img), endian(end)
{
    uint64_t devCount(0);
    for(auto dev_off : devOffsets) {
        char *diskArr = new char[SuperBlock::SUPBLK_SIZE]();
        tsk_img_read(img, dev_off + SuperBlock::SUPBLK_ADDR, diskArr, SuperBlock::SUPBLK_SIZE);
        SuperBlock *supblk = new SuperBlock(TSK_LIT_ENDIAN, (uint8_t*)diskArr);

        if(fsUUID.isUnused()) {
            fsUUID = supblk->fsUUID;
        }
        else if(fsUUID != supblk->fsUUID) {
            ostringstream oss;
            oss << "Found superblocks do not belong to the same pool.\n";
            oss << "The following different filesystem UUIDs were found:\n";
            oss << supblk->fsUUID.encode() << '\n';
            oss << fsUUID.encode() << '\n';
            throw FsDeviceException(oss.str());
        }

        const DevData *dev = &(supblk->devItemData);
        deviceTable[dev->deviceId] = new DeviceRecord(dev->deviceId,
                                        dev_off, dev->devUUID, supblk);

        devCount = supblk->numDevices;
    }

    if(devOffsets.size() != devCount) {
        ostringstream oss;
        oss << "Input incomplete: device(s) missing.\n";
        oss << std::dec;
        oss << "Expected device number: " << devCount << '\n';
        oss << "Input device number: " << devOffsets.size() << '\n'; 
        throw FsDeviceException(oss.str());
    }
    
    primarySupblk = deviceTable[1]->superBlk;

    initializeChunkTree();
    initializeRootTree();
    initializeFileTree(fsRootId);
}


//! Destructor
BtrfsPool::~BtrfsPool()
{
    if(primarySupblk != nullptr)
        delete primarySupblk;
    for(auto &record : deviceTable) {
        delete record.second;
        record.second = nullptr;
    }
}


//! List devices of the pool
//!
//! \return String of device description
//!
string BtrfsPool::devInfo() const
{
    ostringstream oss;
    oss << "All devices accepted. Device number: " << deviceTable.size() << "\n";
    oss << "Pool UUID: " << fsUUID.encode() << "\n";
    oss << "-----------------------------------" << "\n";

    for(const auto dev : deviceTable) {
        oss << (dev.second)->devInfo() << "\n";
    }
    
    return oss.str();
}


//! Find device offset within the image in bytes
//! 
//! \param devId
//!
//! \return Device offset in bytes.
//!
uint64_t BtrfsPool::getDevOffset(uint64_t devId) const
{
    return deviceTable.at(devId)->deviceOffset;
}


//! Get the physical addresses from a chunk item.
//!
//! \param logicalAddr Logical address to convert.
//! \param key Key storing chunk logical address.
//! \param chunkData Chunk item data storing corresponding physical address.
//!
//! \return Mapped physical address. 0 if not valid.
//!
vector<uint64_t> BtrfsPool::getAddrFromChunk(uint64_t logicalAddr,
        const BtrfsKey* key, const ChunkData* chunkData) const
{
    vector<uint64_t> physicalAddrs;
    uint64_t chunkLogical = key->offset; //Key offset stores logical address.

    //Input logical address should be larger than chunk logial address.
    if(logicalAddr < chunkLogical)
        throw FsDamagedException("Superblock chunk item error."
                "Unable to map logical address to physical address.");

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
    
        physicalAddrs.push_back(logicalAddr - chunkLogical + chunkPhysical);
    }

    return physicalAddrs;
}


//! Read data from image based on given logical address and chunk.
//!
//! \param[out] data Array with data read from image
//! \param logicalAddr Logical address of data.
//! \param key Btrfs chunk item key.
//! \param chunkData Btrfs chunk item data.
//! \param size Size of data.
//!
//! \return Starting physcial address of data in the image.
//! 
uint64_t BtrfsPool::readChunkData(char *data, uint64_t logicalAddr,
        const BtrfsKey* key, const ChunkData* chunkData, const uint64_t size) const
{
    vector<uint64_t> physicalAddrs = getAddrFromChunk(logicalAddr, key, chunkData);

    uint64_t readSize(0);
    uint64_t length = chunkData->stripeLength;
    for(const auto &addr: physicalAddrs) {
        while(size - readSize > length) {
            tsk_img_read(image, addr + readSize, data + readSize, length);
            readSize += length;
        }
        tsk_img_read(image, addr + readSize, data + readSize, size - readSize);
    }

    return physicalAddrs[0];
}


//! Read data from image based on given logical address.
//!
//! \param[out] data Array with data read from image
//! \param logicalAddr Logical address of data.
//! \param size Size of data.
//!
//! \return Starting physcial address of data in the image.
//! 
uint64_t BtrfsPool::readData(char *data, uint64_t logicalAddr,
                            const uint64_t size) const
{
    const ChunkItem* chunk = chunkTree->getChunkItem(logicalAddr);
    return readChunkData(data, logicalAddr, &(chunk->itemHead->key), &(chunk->data), size);
}


//! Initialize the chunk tree of the pool
void BtrfsPool::initializeChunkTree()
{
    chunkTree = new ChunkTree(this);
}


//! Initialize root of Root Tree.
void BtrfsPool::initializeRootTree()
{
    uint64_t rootTreeLogAddr = primarySupblk->getRootLogAddr();

    char* diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
    uint64_t rootTreePhyAddr = readData(diskArr, rootTreeLogAddr, BtrfsHeader::SIZE_OF_HEADER);

    BtrfsHeader* rootHeader = new BtrfsHeader(endian, (uint8_t*)diskArr);
    delete [] diskArr;

    uint64_t itemListStart = rootTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;
    if(rootHeader->isLeafNode())
        rootTree = new LeafNode(image, rootHeader, endian, itemListStart);
    else
        rootTree = new InternalNode(image, rootHeader, endian, itemListStart);
}


//! Initialize file system tree.
//!
//! \param fsRootId Subvolume id
//!
void BtrfsPool::initializeFileTree(uint64_t fsRootId)
{
    if(fsRootId == 0){
        uint64_t defaultId = getDefaultFsId();

        fsTree = new FilesystemTree(rootTree, defaultId, this);
        fsTreeDefault = fsTree;
    }
    else{
        const BtrfsItem* foundItem;
        if(treeSearchById(rootTree, fsRootId,
            [&foundItem](const LeafNode* leaf, uint64_t targetId)
            { return searchForItem(leaf, targetId, ItemType::ROOT_BACKREF, foundItem); })) {
            fsTree = new FilesystemTree(rootTree, fsRootId, this);
            fsTreeDefault = fsTree;
        }
        else
            throw runtime_error("The argument of -s option is not a valid subvolume id.");
    }
}


//! Obatin root item id of default volume (filesystem tree).
uint64_t BtrfsPool::getDefaultFsId()
{
    //Technically, the default volume id should be found in offset 0x80 in superblock.
    //Currently, it is set as 6 by default.
    uint64_t defaultDirId(6);

    uint64_t defaultId(0);
    const BtrfsItem* foundItem;
    if(treeSearchById(rootTree, defaultDirId,
            [&foundItem](const LeafNode* leaf, uint64_t targetId)
            { return searchForItem(leaf, targetId, ItemType::DIR_ITEM, foundItem); })) {
        const DirItem* dir = static_cast<const DirItem*>(foundItem);
        defaultId = dir->targetKey.objId; //This is id of root item to filesystem tree.
    }
    else 
        throw FsDamagedException("Default directory dir item not found.");

    return defaultId;
}


//! Navigate to selected node and print information.
//!
//! \param root Starting root node.
//! \param os Output stream where the infomation is printed.
//! \param is Input stream telling which node is the one to be read.
//!
void BtrfsPool::navigateNodes(const BtrfsNode* root,
        std::ostream& os, std::istream& is) const
{
    const BtrfsNode *node = root;
    const BtrfsHeader *header;
    while(true) {
        header = node->nodeHeader;
        os << node->info() << endl;

        uint64_t offset(0);
        map<uint64_t, uint64_t> nodeAddrs;
        if(header->isLeafNode()){
            const LeafNode *leaf = static_cast<const LeafNode*>(node);

            for(auto item : leaf->itemList){
                if(item->getItemType() == ItemType::ROOT_ITEM){
                    const RootItem *rootItm = static_cast<const RootItem*>(item);
                    nodeAddrs[item->getId()]
                        = rootItm->getBlockNumber();
                }
            }
        }
        else {
            const InternalNode* internal = static_cast<const InternalNode*>(node);

            for(auto ptr : internal->keyPointers) {
                nodeAddrs[ptr->key.objId] = ptr->getBlkNum();
            }
        }

        if(nodeAddrs.size() == 0) {
            os << "This is a leaf node with no root items." << endl;
            break;
        }

        bool quit(false);
        string input;
        uint64_t inputId;
        while(true) {
            os << "Child nodes or tree roots with following object ids are found." << endl;
            for(auto addr : nodeAddrs) {
                if(header->isLeafNode())
                    os << dec << addr.first << " ";
                else
                    os << dec << "[" << addr.first << "] ";
            }
                
            os << endl;
            os << "To read a child node or a tree root, please enter its object id in the list: ";
            os << "(Enter 'q' to quit.)" << endl;
            is >> input;
            
            if(input == "q") return;

            stringstream(input) >> inputId;
            if(nodeAddrs.find(inputId) != nodeAddrs.end()) break;
            os << "Wrong object id, please enter a correct one.\n" << endl;
        }
        os << endl;

        bool nodeExisted(false);
        using ConstNodePtr = const BtrfsNode*;
        ConstNodePtr* childPtr(nullptr);
        if(!header->isLeafNode()) {
            const InternalNode* internal = static_cast<const InternalNode*>(node);

            for(auto ptr : internal->keyPointers) {
                if(ptr->key.objId == inputId && ptr->childNode != nullptr) {
                    node = ptr->childNode;
                    nodeExisted = true;
                    break;
                }
                else if(ptr->key.objId == inputId) {
                    childPtr = &ptr->childNode;
                    break;
                }
            }
        }
        if(nodeExisted) continue; //The child node has already been built.

        offset = nodeAddrs[inputId];
        char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        uint64_t physicalAddr = readData(headerArr, offset, BtrfsHeader::SIZE_OF_HEADER);
        header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
        delete [] headerArr;

        uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;

        if(header->isLeafNode()){
            node = new LeafNode(image, header, endian, itemOffset);
        }
        else {
            node = new InternalNode(image, header, endian, itemOffset);
        }

        if(childPtr!=nullptr)
            *childPtr = node;
    }
}


//! Switch to a subvolume or snapshot and exploere files within.
//!
//! \param os Output stream where the infomation is printed.
//! \param is Input stream telling which node is the one to be read.
//! \return True if switched.
//!
bool BtrfsPool::switchFsTrees(ostream& os, istream& is)
{
    vector<const BtrfsItem*> foundRootRefs;
    treeTraverse(rootTree, [&foundRootRefs](const LeafNode* leaf)
            { return filterItems(leaf, ItemType::ROOT_BACKREF, foundRootRefs); });
    
    if(foundRootRefs.size() == 0) {
        os << "\nNo subvolumes or snapshots are found.\n" << endl;
        return false;
    }

    uint64_t selectedId(0);
    while(true) {
        os << "The following subvolumes or snapshots are found:" << endl;
        int index(0);
        for(auto item : foundRootRefs) {
            const RootRef* ref = static_cast<const RootRef*>(item);
            os << "[" << dec << setfill(' ') << setw(2) << ++index << "] "
                << setw(7) << ref->getId() << "   " << ref->getDirName() << '\n';
        }
        os << endl;

        string input;
        os << "To visit a subvolume or snapshot, please enter its index in the list:\n";
        os << "(Enter ''q' to quit.)" << endl;
        is >> input;

        if(input == "q") return false;
        int inputIndex;
        stringstream(input) >> inputIndex;
        if(inputIndex > 0 && inputIndex <= foundRootRefs.size()) {
            selectedId = foundRootRefs[inputIndex-1]->getId();
            break;
        }
        os << "Wrong index, please enter a correct one.\n\n\n" << endl;
    }
    
    fsTree = new FilesystemTree(rootTree, selectedId, this);
    os << "\n" << std::string(60, '=') << "\n";
    os << endl;
    return true;
}


//! Recursively traverse child nodes and process it if it is a leaf node.
//!
//! \param node Node being processed.
//! \param readOnlyFunc A function type which accepts a LeafNode* 
//!        and a vector<uint64_t>& parameters and returns void.
//!
void BtrfsPool::treeTraverse(const BtrfsNode *node,
        function<void(const LeafNode*)> readOnlyFunc) const
{
    if(node->nodeHeader->isLeafNode()){
        const LeafNode* leaf = static_cast<const LeafNode*>(node);
        readOnlyFunc(leaf);
    }
    else {
        const InternalNode* internal = static_cast<const InternalNode*>(node);
        const BtrfsNode *newNode;

        for(auto ptr : internal->keyPointers) {
            if(ptr->childNode != nullptr) {
                newNode = ptr->childNode;
            }
            else {
                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                uint64_t physicalAddr = readData(headerArr, ptr->getBlkNum(),
                    BtrfsHeader::SIZE_OF_HEADER);
                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;

                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                ptr->childNode = newNode;
            }

            if(newNode != nullptr)
                treeTraverse(newNode, readOnlyFunc);
        }
    }
}


//! Recursively traverse nodes in chunk tree and search if it is a leaf node.
//!
//! \param node Node being processed.
//! \param searchFunc A function type which accepts a LeafNode* 
//!        parameter and returns true if certain object is found.
//! \return True if target is found in leaf node.
//!
bool BtrfsPool::chunkTreeSearch(const BtrfsNode *node,
        function<bool(const LeafNode*)> searchFunc) const
{
    if(node->nodeHeader->isLeafNode()){
        const LeafNode *leaf = static_cast<const LeafNode*>(node);
        return searchFunc(leaf);
    }
    else {
        const InternalNode *internal = static_cast<const InternalNode*>(node);
        const BtrfsNode *newNode;

        for(auto ptr : internal->keyPointers) {
            if(ptr->childNode != nullptr) {
                newNode = ptr->childNode;
            }
            else {
                const SuperBlock *supBlk = primarySupblk;
                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                uint64_t physicalAddr = readChunkData(headerArr, ptr->getBlkNum(),
                    &(supBlk->chunkKey), &(supBlk->chunkData),
                    BtrfsHeader::SIZE_OF_HEADER);

                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;
                
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                ptr->childNode = newNode;
            }

            if(newNode != nullptr && treeSearch(newNode, searchFunc))
                return true;
        }
        return false;
    }
}


//! Recursively traverse child nodes and search if it is a leaf node.
//!
//! \param node Node being processed.
//! \param searchFunc A function type which accepts a LeafNode* 
//!        parameter and returns true if certain object is found.
//! \return True if target is found in leaf node.
//!
bool BtrfsPool::treeSearch(const BtrfsNode *node,
        function<bool(const LeafNode*)> searchFunc) const
{
    if(node->nodeHeader->isLeafNode()){
        const LeafNode *leaf = static_cast<const LeafNode*>(node);
        return searchFunc(leaf);
    }
    else {
        const InternalNode *internal = static_cast<const InternalNode*>(node);
        const BtrfsNode *newNode;

        for(auto ptr : internal->keyPointers) {
            if(ptr->childNode != nullptr) {
                newNode = ptr->childNode;
            }
            else {
                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                uint64_t physicalAddr = readData(headerArr, ptr->getBlkNum(),
                    BtrfsHeader::SIZE_OF_HEADER);

                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;
                
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                ptr->childNode = newNode;
            }

            if(newNode != nullptr && treeSearch(newNode, searchFunc))
                return true;
        }
        return false;
    }
}


//! Recursively traverse child nodes and search if it is a leaf node.
//!
//! \param node Node being processed.
//! \param targetId Object id of target to search for.
//! \param searchFunc A function type which accepts a LeafNode* 
//!        parameter and returns true if certain object is found.
//! \return True if target is found in leaf node.
//!
bool BtrfsPool::treeSearchById(const BtrfsNode *node, uint64_t targetId,
        function<bool(const LeafNode*, uint64_t)> searchFunc) const
{
    if(node->nodeHeader->isLeafNode()){
        const LeafNode *leaf = static_cast<const LeafNode*>(node);
        return searchFunc(leaf, targetId);
    }
    else {
        const InternalNode *internal = static_cast<const InternalNode*>(node);
        const BtrfsNode *newNode;

        const auto &vecPtr = internal->keyPointers;
        for(int i=0; i<vecPtr.size();++i) {
            auto ptr = vecPtr[i];
            if(ptr->key.objId > targetId)
                return false;
            if(ptr->key.objId<targetId && i!=vecPtr.size()-1
                    && vecPtr[i+1]->key.objId<targetId)
                continue;

            if(ptr->childNode != nullptr) {
                newNode = ptr->childNode;
            }
            else {
                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                uint64_t physicalAddr = readData(headerArr, ptr->getBlkNum(),
                    BtrfsHeader::SIZE_OF_HEADER);
                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = physicalAddr + BtrfsHeader::SIZE_OF_HEADER;
                
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                ptr->childNode = newNode;
            }

            if(newNode != nullptr && treeSearchById(newNode, targetId, searchFunc))
                return true;
        }
        return false;
    }
}

}

