#btrForensics
Forensic Analysis Tool for Btrfs File System.

###Platform:
Linux

###Prerequisite:
Install the Sleuth Kit library --> [Link](https://github.com/sleuthkit/sleuthkit.git)

###Build:
```
mkdir build

cd build

cmake ..

make
```

###Input File:
Raw image of a btrfs partition, or a partition using btrfs.

###Current capability:
Reads nodes in B-tree manually and prints information.

###Note:
Reference of Btrfs structure can be found in [btrfs Wiki](https://btrfs.wiki.kernel.org/index.php/Main_Page).

Btrfs on-disk format: [Link](https://btrfs.wiki.kernel.org/index.php/On-disk_Format)

###License:
This software uses MIT License.

The Sleuth Kit library is employed.

You can find the Sleuth Kit from [sleuthkit/sleuthkit](https://github.com/sleuthkit/sleuthkit.git)

