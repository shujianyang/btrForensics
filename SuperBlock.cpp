#include "SuperBlock.h"

SuperBlock::SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[])
{
    int arIndex(0);
    for(int i=0; i<32; i++, arIndex++){
        checksum[i] = arr[arIndex];
    }
    arIndex += 32;
    for(int i=0; i<8; i++, arIndex++){
        magic[i] = arr[arIndex];
    }
}

