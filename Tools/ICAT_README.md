#icat
Output the contents of file with provided inode number in Btrfs.

This is a simulation to The Sleuth Kit's icatat program.

###Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

###Usage:
```
icat [-o imgoffset] image [inode]
```

-o imgoffset: Offset into image file (in sectors)

###Note:
Unlike icat in The Sleuth Kit, this program write file with original file name to directory.

###License:
This software uses MIT License.