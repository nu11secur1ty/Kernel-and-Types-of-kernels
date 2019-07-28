# Understanding disk inodes

You try creating a file on a server and see this error message:
```bash
No space left on device
```
..but you've got plenty of space:

```bash
df
Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/xvda1            10321208   3159012   6637908  33% /
```

Who is the invisible monster chewing up all of your space?

Why, the inode monster of course!

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/Inodes/wall/monster.jpg)

# What are inodes?
An index node (or inode) contains metadata information (file size, file type, etc.) for a file system object (like a file or a directory). There is one inode per file system object.

An inode doesn't store the file contents or the name: it simply points to a specific file or directory.

The problem with inodes
The total number of inodes and the space reserved for these inodes is set when the filesystem is first created. The inode limit can't be changed dynamically and every file system object must have an inode.

While it's unusual to run out of inodes before actual disk space, you are more likely to have inode shortages if:

- You are creating lots of directories, symlinks, and small files.
- You created your ext3 filesystem with smaller block sizes. The ext3 default block size is 4096 bytes. If you are using your filesystem for storing lots of very small files, you might create the filesystem with a block size of 1024 or 2048. This would let you use your disk space more efficiently, but raises the likelihood of running low on inodes.
- Your server is containerized (Docker, LXC, OpenVZ, etc). Containerized servers can often share the same filesystem as the host node. For stability and security purposes, the containers resources such as RAM, CPU and disk space and inodes are limited. In this situation the number of inodes allocated to your container is determined by the administrator of the host node. It is very common to run into into inode issues in containers with filesystems of this type


# Viewing inode usage

- Use the `-i` flag to view inode usage:
```bash
df -i
Filesystem            Inodes   IUsed   IFree IUse% Mounted on
/dev/xvda1           1310720  1310720        0  100% /
```

You can view more detailed inode information with the following command:

```bash
tune2fs -l /dev/xvda1 | grep -i inode
```

# Where are the small files?
If you are using up all of your inode capacity, the next step is figuring out where all of those little files are. This is a bit of a manual process.

The command below will output the number of files and directory from the top of your file system:

```bash
for i in /*; do echo $i; find $i |wc -l; done
```
- Example output:
```bash
/bin
119
/sys
9293
/tmp
1
/usr
10583
```
The counts above include all nested directories. If you have a single directory with many files, the command above may stall - that's a good hint that's the problem directory. Once you've narrowed it down to a specific directory, you can execute the previous command on that directory:


```bash
for i in /usr/*; do echo $i; find $i |wc -l; done
```

Delete all of the small files in that directory.

I can't delete those files. Can I increase the inode limit?
The bad news: on the ext* file systems, you can't simply increase the inode limit on an existing volume. You have two options:

1. - If the disk is an LVM, increase the size of the volume.

2. - Backup and create a new a new file system, specifying a higher inode limit:

```bash
mke2fs -N
```


# Are there filesystems that don't have inode limits?
Yes. Modern filesystems like `Btrfs` and `XFS` use dynamic inodes to avoid inode limits. `ZFS` does not use inodes.










