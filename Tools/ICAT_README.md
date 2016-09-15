#icat
Output the contents of file with provided inode number in Btrfs.

This is a simulation to The Sleuth Kit's icatat program.

###Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

###Usage:
```
icat [-o imgoffset] [-s subvolumeid] image inode
```

-o imgoffset: Offset into image file (in sectors)

-s subvolumeid: The id of subvolume or snapshot. List can be found by using subls tool.

###Note:
Unlike icat in The Sleuth Kit, this program write file with original file name to directory.

###License:
This software uses MIT License.