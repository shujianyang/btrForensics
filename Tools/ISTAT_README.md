#istat
Print information about an inode.

This is a simulation to The Sleuth Kit's istat program.

###Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

###Usage:
```
istat [-o imgoffset] image inode
```

-o imgoffset: Offset into image file (in sectors)

###Note:
Unable to distinguish files and directories yet.

###License:
This software uses MIT License.