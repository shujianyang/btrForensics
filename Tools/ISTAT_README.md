# istat
Print information about an inode.

This is a simulation to The Sleuth Kit's istat program.

### Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

### Usage:
```
istat [-o offset1,offset2,offset3...] [-s subvolumeid] image inode
```

-o offset: Offset to the beginning of the partition (in sectors).
May have multiple values if the pool is made up by multiple partitions(devices).

-s subvolumeid: The id of subvolume or snapshot. List can be found by using subls tool.

### Note:
Unable to distinguish files and directories yet.

### License:
This software uses MIT License.
