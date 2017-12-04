//! \file
//! \author Shujian Yang
//!
//! UUID processing.

#include <sstream>
#include <bitset>
#include "Uuid.h"
#include "Utility/ReadInt.h"

namespace btrForensics {

//! Default constructor of UUID
UUID::UUID()
{
    data_1 = 0;
    data_2 = 0;
    data_3 = 0;

    for(int i=0; i<DATA_4_SIZE; i++)
        data_4[i] = 0;
}


//! Constructor of UUID
//!
//! \param endian The endianess of the array.
//! \param arr Byte array storing UUID.
//!
UUID::UUID(TSK_ENDIAN_ENUM endian, uint8_t arr[])
{
    data_1 = read32Bit(endian, arr);
    data_2 = read16Bit(endian, arr + 4);
    data_3 = read16Bit(endian, arr + 6);

    for(int i=0; i<DATA_4_SIZE; i++){
        data_4[i] = *(arr + DATA_4_SIZE + i);
    }
}


//! Constructor of UUID
//!
//! \param endian The endianess of the array.
//! \param entry TSK gpt entry.
//!
UUID::UUID(TSK_ENDIAN_ENUM endian, gpt_entry &entry)
{
    data_1 = read32Bit(endian, entry.type_guid);
    data_2 = read16Bit(endian, entry.type_guid + 4);
    data_3 = read16Bit(endian, entry.type_guid + 6);

    for(int i=0; i<DATA_4_SIZE; i++){
        data_4[i] = *(entry.type_guid + DATA_4_SIZE + i);
    }

    std::bitset<8> d4(data_4[0]);
    if(!d4.test(7))
        uuidVar = NETWORK_COMPUTING_SYSTEM;
    if(d4.test(7) && !d4.test(6))
        uuidVar = STANDARD;
    if(d4.test(7) && d4.test(6) && !d4.test(5))
        uuidVar = MS_COM;
}


//! Copy constructor of UUID
UUID::UUID(const UUID &origin)
{
    data_1 = origin.data_1;
    data_2 = origin.data_2;
    data_3 = origin.data_3;

    for(int i=0; i<DATA_4_SIZE; i++){
        data_4[i] = origin.data_4[i];
    }
}


//! Check if a UUID is empty.
//!
//! \return True if the UUID is all 0s.
//!
const bool UUID::isUnused() const
{
    return match(0, 0, 0, 0);
}

//! Encode UUID as a string
//!
//! \return Encoded UUID string.
//!
const std::string UUID::encode() const
{
    if(match(0, 0, 0, 0))
        return "";
        
    std::ostringstream guidOSS;
    guidOSS.fill('0');
    guidOSS << std::uppercase << std:: hex;

    guidOSS.width(8);
    guidOSS << data_1 << '-';
    guidOSS.width(4);
    guidOSS << data_2 << '-';
    guidOSS.width(4);
    guidOSS << data_3 << '-';

    const uint8_t *part = data_4;
    guidOSS.width(2);
    guidOSS << (int)*part++;
    guidOSS.width(2);
    guidOSS << (int)*part++ << '-';

    for(int i=0; i<6; ++i){
        guidOSS.width(2);
        guidOSS << (int)*part++;
    }
    
    return guidOSS.str();
}


//!  Check if UUID matches a given value.
//!
//! \param d1 Part 1 of UUID
//! \param d2 Part 2 of UUID
//! \param d3 Part 3 of UUID
//! \param d4 Part 4 of UUID
//!
//! \return True if UUID matches the given pattern.
//!
const bool UUID::match(uint32_t d1, uint16_t d2, uint16_t d3, uint64_t d4) const
{
    if(data_1 != d1 || data_2 != d2 || data_3 != d3)
        return false;
        
    uint64_t p4 = (uint64_t)data_4[0];
    int i;
    for(i=1; i<DATA_4_SIZE; ++i){
        p4 = p4 << 8;
        p4 += (uint64_t)data_4[i];
    }
    
    if(p4 != d4)
        return false;
    else
        return true;
}


//! Get partition type based on UUID value.
//!
//! \return String of partition type the UUID represents.
//!
const std::string UUID::guidType() const
{
    std::string type;
    if(match(0, 0, 0, 0))
        type = "Unused entry";
    else if(match(0x024DEE41, 0x33E7, 0x11D3, 0x9D690008C781F39F))
        type = "MBR partition scheme";
    else if(match(0xC12A7328, 0xF81F, 0x11D2, 0xBA4B00A0C93EC93B))
        type = "EFI System partition";
    else if(match(0x21686148, 0x6449, 0x6E6F, 0x744E656564454649))
        type = "BIOS Boot partition";
    else if(match(0xD3BFE2DE, 0x3DAF, 0x11DF, 0xBA40E3A556D89593))
        type = "Intel Fast Flash partition";
    else if(match(0xF4019732, 0x066E, 0x4E12, 0x8273346C5641494F))
        type = "Sony boot partition";
    else if(match(0xBFBFAFE7, 0xA34F, 0x448A, 0x9A5B6213EB736C22))
        type = "Lenovo boot partition";

    else if(match(0xE3C9E316, 0x0B5C, 0x4DB8, 0x817DF92DF00215AE))
        type = "Microsoft Reserved Partition";
    else if(match(0xDE94BBA4, 0x06D1, 0x4D40, 0xA16ABFD50179D6AC))
        type = "Windows Recovery Environment";
    else if(match(0xEBD0A0A2, 0xB9E5, 0x4433, 0x87C068B6B72699C7))
        type = "Basic data partition";
    else if(match(0x5808C8AA, 0x7E8F, 0x42E0, 0x85D2E1E90434CFB3))
        type = "Logical Disk Manager (LDM) metadata partition";
    else if(match(0xAF9B60A0, 0x1431, 0x4F62, 0xBC683311714A69AD))
        type = "Logical Disk Manager data partition";
    else if(match(0xDE94BBA4, 0x06D1, 0x4D40, 0xA16ABFD50179D6AC))
        type = "Windows Recovery Environment";
    else if(match(0x37AFFC90, 0xEF7D, 0x4E96, 0x91C32D7AE055B174))
        type = "IBM General Parallel File System (GPFS) partition";
    else if(match(0xE75CAF8F, 0xF680, 0x4CEE, 0xAFA3B001E56EFC2D))
        type = "Storage Spaces partition";

    else if(match(0x75894C1E, 0x3AEB, 0x11D3, 0xB7C17B03A0000000))
        type = "HP-UX Data partition";
    else if(match(0xE2A1E728, 0x32E3, 0x11D6, 0xA6827B03A0000000))
        type = "HP-UX Service Partition";

    else if(match(0x0FC63DAF, 0x8483, 0x4772, 0x8E793D69D8477DE4))
        type = "Linux filesystem data";
    else if(match(0xA19D880F, 0x05FC, 0x4D3B, 0xA006743F0F84911E))
        type = "Linux RAID partition";
    else if(match(0x44479540, 0xF297, 0x41B2, 0x9AF7D131D5F0458A))
        type = "Linux Root partition (x86)";
    else if(match(0x4F68BCE3, 0xE8CD, 0x4DB1, 0x96E7FBCAF984B709))
        type = "Linux Root partition (x86-64)";
    else if(match(0x69DAD710, 0x2CE4, 0x4E3C, 0xB16C21A1D49ABED3))
        type = "Linux Root partition (32-bit ARM)";
    else if(match(0xB921B045, 0x1DF0, 0x41C3, 0xAF444C6F280D3FAE))
        type = "Linux Root partition (64-bit ARM/AArch64)";
    else if(match(0x0657FD6D, 0xA4AB, 0x43C4, 0x84E50933C84B4F4F))
        type = "Linux swap partition";
    else if(match(0xE6D6D379, 0xF507, 0x44C2, 0xA23C238F2A3DF928))
        type = "Logical Volume Manager (LVM) partition";
    else if(match(0x933AC7E1, 0x2EB4, 0x4F13, 0xB8440E14E2AEF915))
        type = "Linux /home partition";
    else if(match(0x3B8F8425, 0x20E0, 0x4F3B, 0x907F1A25A76F98E8))
        type = "/srv (server data) partition";
    else if(match(0x7FFEC5C9, 0x2D00, 0x49B7, 0x89413EA10A5586B7))
        type = "Plain dm-crypt partition";
    else if(match(0xCA7D7CCB, 0x63ED, 0x4C53, 0x861C1742536059CC))
        type = "LUKS partition";
    else if(match(0x8DA63339, 0x0007, 0x60C0, 0xC436083AC8230908))
        type = "Reserved";

    else if(match(0x83BD6B9D, 0x7F41, 0x11DC, 0xBE0B001560B84F0F))
        type = "FreeBSD Boot partition";
    else if(match(0x516E7CB4, 0x6ECF, 0x11D6, 0x8FF800022D09712B))
        type = "FreeBSD Data partition";
    else if(match(0x516E7CB5, 0x6ECF, 0x11D6, 0x8FF800022D09712B))
        type = "FreeBSD Swap partition";
    else if(match(0x516E7CB6, 0x6ECF, 0x11D6, 0x8FF800022D09712B))
        type = "FreeBSD Unix File System (UFS) partition";
    else if(match(0x516E7CB8, 0x6ECF, 0x11D6, 0x8FF800022D09712B))
        type = "FreeBSD Vinum volume manager partition";
    else if(match(0x516E7CBA, 0x6ECF, 0x11D6, 0x8FF800022D09712B))
        type = "FreeBSD ZFS partition";

    else if(match(0x48465300, 0x0000, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Hierarchical File System Plus (HFS+) partition";
    else if(match(0x55465300, 0x0000, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple UFS";
    else if(match(0x6A898CC3, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "OS X ZFS";
    else if(match(0x52414944, 0x0000, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple RAID partition";
    else if(match(0x52414944, 0x5F4F, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple RAID partition, offline";
    else if(match(0x426F6F74, 0x0000, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple Boot partition (Recovery HD)";
    else if(match(0x4C616265, 0x6C00, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple Label";
    else if(match(0x5265636F, 0x7665, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple TV Recovery partition";
    else if(match(0x53746F72, 0x6167, 0x11AA, 0xAA1100306543ECAC))
        type = "OS X Apple Core Storage (i.e. Lion FileVault) partition";

    else if(match(0x6A82CB45, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Boot partition";
    else if(match(0x6A85CF4D, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Root partition";
    else if(match(0x6A87C46F, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Swap partition";
    else if(match(0x6A8B642B, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Backup partition";
    else if(match(0x6A898CC3, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris /usr partition";
    else if(match(0x6A8EF2E9, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris /var partition";
    else if(match(0x6A90BA39, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris /home partition";
    else if(match(0x6A9283A5, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Alternate sector";
    else if(match(0x6A945A3B, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Reserved partition";
    else if(match(0x6A9630D1, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Reserved partition";
    else if(match(0x6A980767, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Reserved partition";
    else if(match(0x6A96237F, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Reserved partition";
    else if(match(0x6A8D2AC7, 0x1DD2, 0x11B2, 0x99A6080020736631))
        type = "Solaris Reserved partition";

    else if(match(0x49F48D32, 0xB10E, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD Swap partition";
    else if(match(0x49F48D5A, 0xB10E, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD FFS partition";
    else if(match(0x49F48D82, 0xB10E, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD LFS partition";
    else if(match(0x49F48DAA, 0xB10E, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD RAID partition";
    else if(match(0x2DB519C4, 0xB10F, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD Concatenated partition";
    else if(match(0x2DB519EC, 0xB10F, 0x11DC, 0xB99B0019D1879648))
        type = "NetBSD Encrypted partition";

    else if(match(0xFE3A2A5D, 0x4F32, 0x41A7, 0xB725ACCC3285A309))
        type = "ChromeOS kernel";
    else if(match(0x3CB8E202, 0x3B7E, 0x47DD, 0x8A3C7FF2A13CFCEC))
        type = "ChromeOS rootfs";
    else if(match(0x2E0A753D, 0x9E48, 0x43B0, 0x8337B15192CB1B5E))
        type = "ChromeOS future use";

    else if(match(0x42465331, 0x3BA3, 0x10F1, 0x802A4861696B7521))
        type = "Haiku BFS";

    else if(match(0x85D5E45E, 0x237C, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD Boot partition";
    else if(match(0x85D5E45A, 0x237C, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD Data partition";
    else if(match(0x85D5E45B, 0x237C, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD Swap partition";
    else if(match(0x0394EF8B, 0x237E, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD Unix File System (UFS) partition";
    else if(match(0x85D5E45C, 0x237C, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD Vinum volume manager partition";
    else if(match(0x85D5E45D, 0x237C, 0x11E1, 0xB4B3E89A8F7FC3A7))
        type = "MidnightBSD ZFS partition";

    else if(match(0x45B0969E, 0x9B03, 0x4F30, 0xB4C6B4B80CEFF106))
        type = "Ceph Journal";
    else if(match(0x45B0969E, 0x9B03, 0x4F30, 0xB4C65EC00CEFF106))
        type = "Ceph dm-crypt Encrypted Journal";
    else if(match(0x4FBD7E29, 0x9D25, 0x41B8, 0xAFD0062C0CEFF05D))
        type = "Ceph OSD";
    else if(match(0x4FBD7E29, 0x9D25, 0x41B8, 0xAFD05EC00CEFF05D))
        type = "Ceph dm-crypt OSD";
    else if(match(0x89C57F98, 0x2FE5, 0x4DC0, 0x89C1F3AD0CEFF2BE))
        type = "Ceph disk in creation";
    else if(match(0x89C57F98, 0x2FE5, 0x4DC0, 0x89C15EC00CEFF2BE))
        type = "Ceph dm-crypt disk in creation";

    else if(match(0x824CC7A0, 0x36A8, 0x11E3, 0x890A952519AD3F61))
        type = "OpenBSD Data partition";

    else if(match(0xCEF5A9AD, 0x73BC, 0x4601, 0x89F3CDEEEEE321A1))
        type = "QNX Power-safe (QNX6) file system";

    else if(match(0xC91818F9, 0x8025, 0x47AF, 0x89D2F030D7000C2C))
        type = "Plan 9 partition";
    else if(match(0x9D275380, 0x40AD, 0x11DB, 0xBF97000C2911D1B8))
        type = "vmkcore (coredump partition)";
    else if(match(0xAA31E02A, 0x400F, 0x11DB, 0x9590000C2911D1B8))
        type = "VMFS filesystem partition";
    else if(match(0x9198EFFC, 0x31C0, 0x11DB, 0x8F78000C2911D1B8))
        type = "VMware Reserved";

    else if(match(0x2568845D, 0x2332, 0x4675, 0xBC398FA5A4748D15))
        type = "Android-IA Bootloader";
    else if(match(0x114EAFFE, 0x1552, 0x4022, 0xB26E9B053604CF84))
        type = "Android-IA Bootloader2";
    else if(match(0x49A4D17F, 0x93A3, 0x45C1, 0xA0DEF50B2EBE2599))
        type = "Android-IA Boot";
    else if(match(0x4177C722, 0x9E92, 0x4AAB, 0x864443502BFD5506))
        type = "Android-IA Recovery";
    else if(match(0xEF32A33B, 0xA409, 0x486C, 0x91419FFB711F6266))
        type = "Android-IA Misc";
    else if(match(0x20AC26BE, 0x20B7, 0x11E3, 0x84C56CFDB94711E9))
        type = "Android-IA Metadata";
    else if(match(0x38F428E6, 0xD326, 0x425D, 0x91406E0EA133647C))
        type = "Android-IA System";
    else if(match(0xA893EF21, 0xE428, 0x470A, 0x9E550668FD91A2D9))
        type = "Android-IA Cache";
    else if(match(0xDC76DDA9, 0x5AC1, 0x491C, 0xAF42A82591580C0D))
        type = "Android-IA Data";
    else if(match(0xEBC597D0, 0x2053, 0x4B15, 0x8B64E0AAC75F4DB1))
        type = "Android-IA Persistent";
    else if(match(0x8F68CC74, 0xC5E5, 0x48DA, 0xBE91A0C8C15E9C80))
        type = "Android-IA Factory";
    else if(match(0x767941D0, 0x2085, 0x11E3, 0xAD3B6CFDB94711E9))
        type = "Android-IA Fastboot / Tertiary";
    else if(match(0xAC6D7924, 0xEB71, 0x4DF8, 0xB48DE267B27148FF))
        type = "Android-IA OEM";

    else if(match(0x7412F7D5, 0xA156, 0x4B13, 0x81DC867174929325))
        type = "ONIE Boot";
    else if(match(0xD4E6E2CD, 0x4469, 0x46F3, 0xB5CB1BFF57AFC149))
        type = "ONIE Config";

    else if(match(0x9E1A2D38, 0xC612, 0x4316, 0xAA268B49521E5A8B))
        type = "PowerPC PReP boot";

    else if(match(0xBC13C2FF, 0x59E6, 0x4262, 0xA352B275FD6F7172))
        type = "Freedesktop Extended Boot Partition ($BOOT)";
        
    else
        type = "[Unkown type]";
        
    return type;
}


//! Get UUID version.
//!
//! \return Version of the UUID.
//!
const int UUID::getVersion() const
{
    uint16_t ver = data_3 >> 12;

    return ver;
}


//! Get UUID variant string.
//!
//! \return String of variant infomation.
//!
const std::string UUID::variantInfo() const
{
    std::string variant("");
    switch(uuidVar){
        case NETWORK_COMPUTING_SYSTEM: 
            variant = "Network Computing System";
            break;
        case STANDARD:
            variant = "RFC 4122 Standard";
            break;
        case MS_COM:
            variant = "Microsoft COM";
            break;
        case RESERVED:
            variant = "Reserved";
            break;
        default:
            variant = "Unknown";
    }

    return variant;
}


//! Get UUID version string.
//!
//! \return String of version infomation.
//!
const std::string UUID::versionInfo() const
{
    std::string version("");
    if(uuidVar != STANDARD)
        return "Invalid";

    switch(getVersion()){
        case 1:
            version = "Ver 1: MAC address & date-time";
            break;
        case 2:
            version = "Ver 2: DCE security";
            break;
        case 3:
            version = "Ver 3: MD5 hash & namespace";
            break;
        case 4:
            version = "Ver 4: Random number";
            break;
        case 5:
            version = "Ver 5: SHA-1 hash & namespace";
            break;
        default:
            version = "Unknown";
    }

    return version;
}


//! Overloaded equality operator
bool operator==(const UUID &lhs, const UUID &rhs)
{
    if(lhs.data_1 != rhs.data_1 || lhs.data_2 != rhs.data_2
            || lhs.data_3 != rhs.data_3)
        return false;

    for(int i=0; i<UUID::DATA_4_SIZE; i++)
        if(lhs.data_4[i] != rhs.data_4[i])
            return false;

    return true;
}


//! Overloaded copy assignment operator
UUID& UUID::operator=(const UUID& rhs)
{
    data_1 = rhs.data_1;
    data_2 = rhs.data_2;
    data_3 = rhs.data_3;

    for(int i=0; i<DATA_4_SIZE; i++)
        data_4[i] = rhs.data_4[i];

    return *this;
}


//! Overloaded equality operator
bool operator!=(const UUID &lhs, const UUID &rhs)
{
    return !(lhs == rhs);
}

}

