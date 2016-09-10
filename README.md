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
Raw image of a btrfs partition, or a partition device file using btrfs.

###Usage:
```
btrfs [-o imgoffset] image 
```


-o imgoffset: Offset into image file (in sectors)

###Current capability:
1. Navigates to selected nodes in B-tree and prints information.
2. List all files in default filesystem tree.
3. Explor files and subdirectories in default root directory.
4. Switch to a subvolume or snapshot and exploere files within.

###Tools
There will be some stand alone programs in the Tools folder.
Most of them are simulations to tools in The Sleuth's Kit.
Give them a try.

###Note:
Reference of Btrfs structure can be found in [btrfs Wiki](https://btrfs.wiki.kernel.org/index.php/Main_Page).

Btrfs on-disk format: [Link](https://btrfs.wiki.kernel.org/index.php/On-disk_Format)

###License:
This software uses MIT License.

The Sleuth Kit library is employed.

You can find the Sleuth Kit from [sleuthkit/sleuthkit](https://github.com/sleuthkit/sleuthkit.git)

