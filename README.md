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
btrfrsc [-o imgoffset] image 
```

-o imgoffset: Offset into image file (in sectors)

###Current capability:
1. Browse nodes derived from root tree and print information.
2. Browse nodes in filesystem tree and print information.
3. List all files in default filesystem tree.
4. Explor files and subdirectories in default root directory.
5. Switch to a subvolume or snapshot and exploere files within.
6. Read a file from image and save to current directory.

###Tools
There will be some stand alone programs built in Tools/ folder.

Most of them are simulations to tools in The Sleuth's Kit. Give them a try.

Current list:

**Tools/fsstat:** Print information about the file system.  
**Tools/fls:** List files and/or directories in a Btrfs partition image.  
**Tools/istat:** Print information about an inode.  
**Tools/icat:** Output the contents of file with provided inode number in Btrfs.  
**Tools/subls:** List subvolumes and snapshots in a Btrfs image.

###Note:
Reference of Btrfs structure can be found in [btrfs Wiki](https://btrfs.wiki.kernel.org/index.php/Main_Page).

Btrfs on-disk format: [Link](https://btrfs.wiki.kernel.org/index.php/On-disk_Format)

###License:
This software uses MIT License.

The Sleuth Kit library is employed.

You can find the Sleuth Kit from [sleuthkit/sleuthkit](https://github.com/sleuthkit/sleuthkit.git)

