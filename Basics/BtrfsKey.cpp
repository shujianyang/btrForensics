#include "BtrfsKey.h"
#include "Utility/ReadInt.h"

BtrfsKey::BtrfsKey(TSK_ENDIAN_ENUM endian, uint8_t arr[])
{
    int arIndex = 0;

    objId = read64Bit(endian, arr + arIndex);
    arIndex += 8;

    itemType = arr[arIndex++];

    offset = read64Bit(endian, arr + arIndex);
}
