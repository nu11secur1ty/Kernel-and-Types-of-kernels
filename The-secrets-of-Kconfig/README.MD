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
The menuconfig is the most popular of these targets. The targets are processed by different host programs, which are provided by the kernel and built during kernel building. Some targets have a GUI (for the user's convenience) while most don't. Kconfig-related tools and source code reside mainly under scripts/kconfig/ in the kernel source. As we can see from scripts/kconfig/Makefile, there are several host programs, including conf, mconf, and nconf. Except for conf, each of them is responsible for one of the GUI-based config targets, so, conf deals with most of them.

Logically, Kconfig's infrastructure has two parts: one implements a new language to define the configuration items (see the Kconfig files under the kernel source), and the other parses the Kconfig language and deals with configuration actions.

Most of the config targets have roughly the same internal process (shown below):
![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/The-secrets-of-Kconfig/wall/1.png)


- Note that all configuration items have a default value.

The first step reads the Kconfig file under source root to construct an initial configuration database; then it updates the initial database by reading an existing configuration file according to this priority:
```
    .config
    /lib/modules/$(shell,uname -r)/.config
    /etc/kernel-config
    /boot/config-$(shell,uname -r)
    ARCH_DEFCONFIG
    arch/$(ARCH)/defconfig
```
If you are doing GUI-based configuration via menuconfig or command-line-based configuration via oldconfig, the database is updated according to your customization. Finally, the configuration database is dumped into the .config file.

But the .config file is not the final fodder for kernel building; this is why the syncconfig target exists. syncconfig used to be a config target called silentoldconfig, but it doesn't do what the old name says, so it was renamed. Also, because it is for internal use (not for users), it was dropped from the list.

Here is an illustration of what syncconfig does:

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/The-secrets-of-Kconfig/wall/2.png)



