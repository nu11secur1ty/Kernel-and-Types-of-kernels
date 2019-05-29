# Rpm based distro – Red Hat/CentOS/Fedora Core/Suse Linux

First find out all installed kernel version with following command:
```
# rpm -qa | grep kernel-smp
```
- or
```
# rpm -qa | grep kernel
```
- Output:
```
kernel-smp-2.6.9-42.EL
kernel-smp-2.6.9-42.0.3.EL
kernel-smp-2.6.9-42.0.8.EL

I’ve total 3 different kernel installed. To remove kernel-smp-2.6.9-42.EL type command:
# rpm -e kernel-smp-2.6.9-42.EL

OR
# rpm -vv -e kernel-smp-2.6.9-42.EL
