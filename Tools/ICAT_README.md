# icat
Output the contents of file with provided inode number in Btrfs.

This is a simulation to The Sleuth Kit's icat program.

### Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

### Usage:
```
icat [-o offset1,offset2,offset3...] [-s subvolumeid] image inode
```

-o offset: Offset to the beginning of the partition (in sectors).
May have multiple values if the pool is made up by multiple partitions(devices).

-s subvolumeid: The id of subvolume or snapshot. List can be found by using subls tool.

### Note:
Unlike icat in The Sleuth Kit, this program write file with original file name to directory.

### License:
This software uses MIT License.

