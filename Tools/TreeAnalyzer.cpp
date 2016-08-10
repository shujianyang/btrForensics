/** \file
 * Implementation of class TreeAnalyzer.
 */

#include <vector>
#include "TreeAnalyzer.h"

using namespace std;

namespace btrForensics {
    /**
     * Constructor of tree analyzer.
     *
     * \param img Image file.
     * \param rootNode Root node of the tree to be analyzed.
     * \param end The endianess of the array.
     *
     */
    TreeAnalyzer::TreeAnalyzer(TSK_IMG_INFO *img,
            BtrfsNode *rootNode, TSK_ENDIAN_ENUM end)
        :image(img), root(rootNode), endian(end)
    {}


    /**
     * List all dir items in this tree using resursion(depth first).
     *
     */
    void TreeAnalyzer::recursiveListDir(const BtrfsNode *node, ostream &os) const
    {
        if(node->nodeHeader->isLeafNode()){
            LeafNode *leaf = (LeafNode*)node;

            for(auto group : leaf->itemGroups){
                if(group->getItemType() == 0x54){
                    DirItem *dir = (DirItem*)(group->data);
                    os << dir->getDirName() << '\n';
                }
            }
            os << endl;
        }
        else {
            vector<uint64_t> nodeAddrs;
            InternalNode *internal = (InternalNode*)node;

            for(const auto &ptr : internal->keyPointers) {
                nodeAddrs.push_back(ptr.getBlkNum());
            }

            for(const auto &offset : nodeAddrs) {

                char *headerArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
                tsk_img_read(image, offset, headerArr, BtrfsHeader::SIZE_OF_HEADER);
                BtrfsHeader *header = new BtrfsHeader(TSK_LIT_ENDIAN, (uint8_t*)headerArr);
                delete [] headerArr; 

                uint64_t itemOffset = offset + BtrfsHeader::SIZE_OF_HEADER;

                BtrfsNode *newNode;
                if(header->isLeafNode()){
                    newNode = new LeafNode(image, header, endian, itemOffset);
                }
                else {
                    newNode = new InternalNode(image, header, endian, itemOffset);
                }

                recursiveListDir(newNode, os);
            }
        }
    }


    /** List all dir items in this tree. */
    void TreeAnalyzer::listDirItems(ostream &os) const
    {
        recursiveListDir(root, os);
    }
}
