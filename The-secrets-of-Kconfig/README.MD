# Kconfig
The Linux kernel config/build system, also known as Kconfig/kbuild, has been around for a long time, ever since the Linux kernel code migrated to Git. As supporting infrastructure, however, it is seldom in the spotlight; even kernel developers who use it in their daily work never really think about it.

To explore how the Linux kernel is compiled, this article will dive into the Kconfig/kbuild internal process, explain how the .config file and the vmlinux/bzImage files are produced, and introduce a smart trick for dependency tracking.

The first step in building a kernel is always configuration. Kconfig helps make the Linux kernel highly modular and customizable. Kconfig offers the user many config targets:

- Kconfig
```
config  - Update current config utilizing a line-oriented program
nconfig - Update current config utilizing a ncurses menu-based program
menuconfig  - Update current config utilizing a menu-based program
xconfig - Update current config utilizing a Qt-based frontend
gconfig - Update current config utilizing a GTK+ based frontend
oldconfig - Update current config utilizing a provided .config as base
localmodconfig  - Update current config disabling modules not loaded
localyesconfig  - Update current config converting local mods to core
defconfig - New config with default from Arch-supplied defconfig
savedefconfig - Save current config as ./defconfig (minimal config)
allnoconfig - New config where all options are answered with 'no'
allyesconfig  - New config where all options are accepted with 'yes'
allmodconfig  - New config selecting modules when possible
alldefconfig  - New config with all symbols set to default
randconfig  - New config with a random answer to all options
listnewconfig - List new options
olddefconfig  - Same as oldconfig but sets new symbols to their default value without prompting
kvmconfig - Enable additional options for KVM guest kernel support
xenconfig - Enable additional options for xen dom0 and guest kernel support
tinyconfig  - Configure the tiniest possible kernel
```
