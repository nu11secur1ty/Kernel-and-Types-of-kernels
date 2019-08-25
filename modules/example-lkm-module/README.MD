# Katil-lkm is a loadable kernel module

- Usage:

- 1 Install the necessary development headers
```bash
apt update -y
apt install -y libelf-dev linux-headers-`uname -r`
```
- 2 build the kernel module
```bash
make
```
- 3 load the module
```bash
insmod hello.ko
```
- 4 check that the module has been inserted
```bash
lsmod | grep hello
```
- 5 check that the module has been really loaded
```bash
dmesg
```
- 6 remove the module
```bash
rmmod hello.ko
```
