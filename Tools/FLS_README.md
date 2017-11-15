# fls
List files and/or directories in a Btrfs partition image.

This is a simulation to The Sleuth Kit's fls program.

### Input File:
Raw image of a btrfs partition, or a partition device file using btrfs.

### Usage:
```
fls [-rDF] [-o imgoffset] [-s subvolumeid] image [inode]
```

If [inode] is not given, the root directory is used.

-o imgoffset: Offset to the beginning of the partition (in sectors).

-s subvolumeid: The id of subvolume or snapshot. List can be found by using subls tool.

-r: Recurse on directory entries

-D: Display only directories

-F: Display only files

### Note:
Unable to list deleted files yet.

### License:
This software uses MIT License.
