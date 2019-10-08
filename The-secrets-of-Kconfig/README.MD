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

syncconfig takes .config as input and outputs many other files, which fall into three categories:

- auto.conf & tristate.conf are used for makefile text processing. For example, you may see statements like this in a component's makefile: 

```bash
obj-$(CONFIG_GENERIC_CALIBRATE_DELAY) += calibrate.o
```
- autoconf.h is used in C-language source files.
- Empty header files under include/config/ are used for configuration-dependency tracking during kbuild, which is explained below.

After configuration, we will know which files and code pieces are not compiled.

--------------------------------------------------------------------------------

# kbuild

Component-wise building, called recursive make, is a common way for GNU make to manage a large project. Kbuild is a good example of recursive make. By dividing source files into different modules/components, each component is managed by its own makefile. When you start building, a top makefile invokes each component's makefile in the proper order, builds the components, and collects them into the final executive.

Kbuild refers to different kinds of makefiles:

- `Makefile` is the top makefile located in source root.
- `.config` is the kernel configuration file.
- `arch/$(ARCH)/Makefile` is the arch makefile, which is the supplement to the top makefile.
- `scripts/Makefile.*` describes common rules for all kbuild makefiles.
- Finally, there are about 500 `kbuild makefiles`.

The top makefile includes the arch makefile, reads the .config file, descends into subdirectories, invokes `make` on each component's makefile with the help of routines defined in `scripts/Makefile.*`, builds up each intermediate object, and links all the intermediate objects into vmlinux. Kernel document Documentation/kbuild/makefiles.txt describes all aspects of these makefiles.

As an example, let's look at how vmlinux is produced on x86-64:

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/The-secrets-of-Kconfig/wall/3.png)


All the `.o` files that go into vmlinux first go into their own built-in.a, which is indicated via variables `KBUILD_VMLINUX_INIT`, `KBUILD_VMLINUX_MAIN`, `KBUILD_VMLINUX_LIBS`, then are collected into the vmlinux file.

Take a look at how recursive make is implemented in the Linux kernel, with the help of simplified makefile code:


```
# In top Makefile vmlinux: scripts/link-vmlinux.sh $(vmlinux-deps) +$(call if_changed,link-vmlinux) # Variable assignments vmlinux-deps := $(KBUILD_LDS) $(KBUILD_VMLINUX_INIT) $(KBUILD_VMLINUX_MAIN) $(KBUILD_VMLINUX_LIBS) export KBUILD_VMLINUX_INIT := $(head-y) $(init-y) export KBUILD_VMLINUX_MAIN := $(core-y) $(libs-y2) $(drivers-y) $(net-y) $(virt-y) export KBUILD_VMLINUX_LIBS := $(libs-y1) export KBUILD_LDS := arch/$(SRCARCH)/kernel/vmlinux.lds init-y := init/ drivers-y := drivers/ sound/ firmware/ net-y := net/ libs-y := lib/ core-y := usr/ virt-y := virt/ # Transform to corresponding built-in.a init-y := $(patsubst %/, %/built-in.a, $(init-y)) core-y := $(patsubst %/, %/built-in.a, $(core-y)) drivers-y := $(patsubst %/, %/built-in.a, $(drivers-y)) net-y := $(patsubst %/, %/built-in.a, $(net-y)) libs-y1 := $(patsubst %/, %/lib.a, $(libs-y)) libs-y2 := $(patsubst %/, %/built-in.a, $(filter-out %.a, $(libs-y))) virt-y := $(patsubst %/, %/built-in.a, $(virt-y)) # Setup the dependency. vmlinux-deps are all intermediate objects, vmlinux-dirs # are phony targets, so every time comes to this rule, the recipe of vmlinux-dirs # will be executed. Refer "4.6 Phony Targets" of `info make` $(sort $(vmlinux-deps)): $(vmlinux-dirs) ; # Variable vmlinux-dirs is the directory part of each built-in.a vmlinux-dirs := $(patsubst %/,%,$(filter %/, $(init-y) $(init-m) \ $(core-y) $(core-m) $(drivers-y) $(drivers-m) \ $(net-y) $(net-m) $(libs-y) $(libs-m) $(virt-y))) # The entry of recursive make $(vmlinux-dirs): $(Q)$(MAKE) $(build)=$@ need-builtin=1
```

The recursive make recipe is expanded, for example:

```bash
make -f scripts/Makefile.build obj=init need-builtin=1
```


This means make will go into scripts/Makefile.build to continue the work of building each built-in.a. With the help of scripts/link-vmlinux.sh, the vmlinux file is finally under source root.
Understanding vmlinux vs. bzImage

Many Linux kernel developers may not be clear about the relationship between vmlinux and bzImage. For example, here is their relationship in x86-64:

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/The-secrets-of-Kconfig/wall/4.png)


