## Architecture
![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/Architecture/architecture-of-linux.png)

1. Kernel:- The kernel is one of the core section of an operating system. It is responsible for each of the major actions of the Linux OS. This operating system contains distinct types of modules and cooperates with underlying hardware directly. The kernel facilitates required abstraction for hiding details of low-level hardware or application programs to the system. There are some of the important kernel types which are mentioned below:
-----------------------------------------
- Monolithic Kernel
- Micro kernels
- Exo kernels
- Hybrid kernels
-----------------------------------------

2. System Libraries:- These libraries can be specified as some special functions. These are applied for implementing the operating system's functionality and don't need code access rights of the modules of kernel.
-----------------------------------------
3. System Utility Programs:- It is responsible for doing specialized level and individual activities.
-----------------------------------------
4. Hardware layer:- Linux operating system contains a hardware layer that consists of several peripheral devices like CPU, HDD, and RAM.
-----------------------------------------
5. Shell:- It is an interface among the kernel and user. It can afford the services of kernel. It can take commands through the user and runs the functions of the kernel. The shell is available in distinct types of OSes. These operating systems are categorized into two different types, which are the graphical shells and command-line shells.
-----------------------------------------

The graphical line shells facilitate the graphical user interface, while the command line shells facilitate the command line interface. Thus, both of these shells implement operations. However, the graphical user interface shells work slower as compared to the command-line interface shells.

There are a few types of these shells which are categorized as follows:

- - - Korn shell
- - - Bourne shell
- - - C shell
- - - POSIX shell

## Linux Operating System Features
`Some of the primary features of Linux OS are as follows:`

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/Architecture/architecture-of-linux2.png)

- Portable: Linux OS can perform different types of hardware and the kernel of Linux supports the installation of any type of hardware environment.
- Open source: Linux operating system source code is available freely and for enhancing the capability of the Linux OS, several teams are performing in collaboration.
- Multiprogramming: Linux OS can be defined as a multiprogramming system. It means more than one application can be executed at the same time.
- Multi-user: Linux OS can also be defined as a multi-user system. It means more than one user can use the resources of the system such as application programs, memory, or RAM at the same time.
- Hierarchical file system: Linux OS affords a typical file structure where user files or system files are arranged.
- Security: Linux OS facilitates user security systems with the help of various features of authentication such as controlled access to specific files, password protection, or data encryption.
- Shell: Linux operating system facilitates a unique interpreter program. This type of program can be applied for executing commands of the operating system. It can be applied to perform various types of tasks such as call application programs and others.

## Drawbacks of Linux

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/Architecture/architecture-of-linux3.png)

- Hardware drivers: Most of the users of Linux face an issue while using Linux. Various companies of hardware prefer to build drivers for Mac or Windows due to they contain several users than Linux. Linux has small drivers for peripheral hardware than windows.
- Software alternative: Let's take the Photoshop example which is a famous tool for graphic editing. Photoshop exists for Windows; however, it is not available in Linux. Also, there are some other tools for photo editing but the Photoshop tool is more powerful as compare to others. Another example is MS office which is not present for Linux users.
- Learning curve: Linux isn't a very user-friendly operating system. Hence, it might be confusing for many beginners. Getting begun with Windows is efficient and easy for many beginners; however, understanding Linux working is complex.
We have to understand the command line interface and finding for newer software is a little bit complex as well. When we face any issue in the OS, the searching solution is very problematic. Also, there are various experts for Mac and Windows as compare to Linux.
- Games: Several games are developed for Windows but unfortunately not for Linux. Because the platform of Windows is used widely. So, the developers of the games are more interested in windows.

## Linux Operating System Applications

Linux is a billion-dollar corporation nowadays. Thousands of governments and companies are using Linux operating system across the world because of lower money, time, licensing fee, and affordability. Linux can be used within several types of electronic devices. These electronic devices are easily available for users worldwide. A few of the famous Linux-based electronic devices are listed below:

- Yamaha Motive Keyboard
- Volvo In-Car Navigation System
- TiVo Digital Video Recorder
- Sony Reader
- Sony Bravia Television
- One Laptop Per child XO2
- Motorola MotoRokr EM35 phone
- Lenovo IdeaPad S9
- HP Mini 1000
- Google Android Dev Phone 1
- Garmin Nuvi 860, 880, and 5000
- Dell Inspiron Mini 9 and 12

## Linux Distribution

It is an OS that is composed of a software-based collection on Linux kernel or we can say the distribution includes the Linux Kernel. It is supporting software and libraries. We can obtain Linux-based OS by downloading any Linux distribution. These types of distributions exists for distinct types of devices such as personal computers, embedded devices, etc. Around more than 600 Linux distributions are existed and a few of the famous Linux distributions are listed as follows:

- - - Deepin
- - - OpenSUSE
- - - Fedora
- - - Solus
- - - Debian
- - - Ubuntu
- - - Elementary
- - - Linux Mint
- - - Manjaro
- - - MX Linux

## Are Ubuntu and Linux Differ?

`YES`
The primary difference between window and Linux is that window is open source and free OS and its Linux distribution based on Debian, Whereas Linux is a large collection of open-source OSes that are working based on Linux kernel.

Besides, Ubuntu is a distribution of Linux and Linux is a core system. Ubuntu is integrated by Canonical Ltd. and published in 2004 and Linux is integrated by Linus Torvalds and published in 1991.


## User mode vs Kernel mode:

The code of kernel component runs in a unique privilege mood known as kernel mode along with complete access to every computer resource. This code illustrates an individual process, runs in an individual address space, and don't need the context switch. Hence, it is very fast and efficient.
Kernel executes all the processes and facilitates various services of a system to the processes. Also, it facilitates secured access to processes to hardware.
The support code that is not needed to execute in kernel mode is inside the system library. The user programs and other types of system programs are implemented in the user mode.
It includes no access to kernel mode and system hardware. User utilities/programs use the system libraries for accessing kernel functions to obtain low-level tasks of the system.

