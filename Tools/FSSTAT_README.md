# fsstat
Print information about the file system.

This is a simulation to The Sleuth Kit's fsstat program.

### Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

### Usage:
```
fsstat [-o offset1,offset2,offset3...] image
```

-o offset: Offset to the beginning of the partition (in sectors).
May have multiple values if the pool is made up by multiple partitions(devices).

### License:
This software uses MIT License.
