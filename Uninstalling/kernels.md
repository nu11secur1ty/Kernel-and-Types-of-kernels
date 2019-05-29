# Rpm based distro – Red Hat/CentOS/Fedora Core/Suse Linux

First find out all installed kernel version with following command:
```bash
# rpm -qa | grep kernel-smp
```
- or
```bash
# rpm -qa | grep kernel
```
- Output:
```bash
kernel-smp-x.x.x-x.x.x.EL
kernel-smp-x.x.x-x.x.x.EL
kernel-smp-x.x.x-x.x.x.EL
```
I’ve total 3 different kernel installed. To remove kernel-smp-2.6.9-42.EL type command:
```bash
# rpm -e kernel-smp-x.x.x-x.x.x.EL
```
- OR
```bash
# rpm -vv -e kernel-smp-x.x.x-x.x.x.EL
```
