# Kernel-and-Types-of-kernels
![image](https://github.com/nu11secur1ty/pictures/blob/master/gV8hn.png)

- Last update from nu11secur1ty: https://github.com/nu11secur1ty/kernel-4.19.0V.Varbanovski_lp150.12.22_default

# Importance_of_Linux_partitions
[Read...](https://github.com/nu11secur1ty/Linux_Deployment_Administration_Hacks/tree/master/Importance_of_Linux_partitions)

# Kernel Space, User Space Interfaces
[Read...](https://www.nu11secur1ty.com/2019/01/kernel-space-user-space-interfaces.html)

-----------------------------------------------------------------------------------------------------------------
# Reference

![](https://github.com/nu11secur1ty/Kernel-and-Types-of-kernels/blob/master/scheme/kernels.png)

The kernel is the core of an operating system. It is the software responsible for running programs and providing secure access to the machine's hardware. Since there are many programs, and resources are limited, the kernel also decides when and how long a program should run. This is called scheduling. Accessing the hardware directly can be very complex, since there are many different hardware designs for the same type of component. Kernels usually implement some level of hardware abstraction (a set of instructions universal to all devices of a certain type) to hide the underlying complexity from applications and provide a clean and uniform interface. This helps application programmers to develop programs without having to know how to program for specific devices. The kernel relies upon software drivers that translate the generic command into instructions specific to that device.

An operating system kernel is not strictly needed to run a computer. Programs can be directly loaded and executed on the "bare metal" machine, provided that the authors of those programs are willing to do without any hardware abstraction or operating system support. This was the normal operating method of many early computers, which were reset and reloaded between the running of different programs. Eventually, small ancillary programs such as program loaders and debuggers were typically left in-core between runs, or loaded from read-only memory. As these were developed, they formed the basis of what became early operating system kernels. The "bare metal" approach is still used today on many video game consoles and embedded systems, but in general, newer systems use kernels and operating systems.

Four broad categories of kernels:

   - Monolithic kernels provide rich and powerful abstractions of the underlying hardware.
   - Microkernels provide a small set of simple hardware abstractions and use applications called servers to provide more functionality.
   - Exokernels provide minimal abstractions, allowing low-level hardware access. In exokernel systems, library operating systems provide the abstractions typically present in monolithic kernels.
   - Hybrid (modified microkernels) are much like pure microkernels, except that they include some additional code in kernelspace to increase performance.
   
   
   
  -------------------------------------------------------------------------------------------------------------
  # Monolitic
  
  The monolithic approach is to define a high-level virtual interface over the hardware, with a set of primitives or system calls to implement operating system services such as process management, concurrency, and memory management in several modules that run in supervisor mode.

Even if every module servicing these operations is separate from the whole, the code integration is very tight and difficult to do correctly, and, since all the modules run in the same address space, a bug in one module can bring down the whole system. However, when the implementation is complete and trustworthy, the tight internal integration of components allows the low-level features of the underlying system to be effectively utilized, making a good monolithic kernel highly efficient. Proponents of the monolithic kernel approach make the case that if code is incorrect, it does not belong in a kernel, and if it is, there is little advantage in the microkernel approach. More modern monolithic kernels such as Linux, FreeBSD and Solaris can load executable modules at runtime, allowing easy extension of the kernel's capabilities as required, while helping to keep the amount of code running in kernel-space to a minimum. It is alone in supervise mode. 

-----------------------------------------------------------------------------------------------------------------

# Microkernels
The microkernel approach is to define a very simple abstraction over the hardware, with a set of primitives or system calls to implement minimal OS services such as thread management, address spaces and interprocess communication. All other services, those normally provided by the kernel such as networking, are implemented in user-space programs referred to as servers. Servers are programs like any others, allowing the operating system to be modified simply by starting and stopping programs. For a small machine without networking support, for instance, the networking server simply isn't started. Under a traditional system this would require the kernel to be recompiled, something well beyond the capabilities of the average end-user. In theory the system is also more stable, because a failing server simply stops a single program, rather than causing the kernel itself to crash.

However, part of the system state is lost with the failing server, and it is generally difficult to continue execution of applications, or even of other servers with a fresh copy. For example, if a (theoretic) server responsible for TCP/IP connections is restarted, applications could be told the connection was "lost" and reconnect, going through the new instance of the server. However, other system objects, like files, do not have these convenient semantics, are supposed to be reliable, not become unavailable randomly and keep all the information written to them previously. So, database techniques like transactions, replication and checkpointing need to be used between servers in order to preserve essential state across single server restarts.

Microkernels generally underperform traditional designs, sometimes dramatically. This is due in large part to the overhead of moving in and out of the kernel, a context switch, in order to move data between the various applications and servers. It was originally believed that careful tuning could reduce this overhead dramatically, but by the mid-90s most researchers had given up. In more recent times newer microkernels, designed for performance first, have addressed these problems to a very large degree. Nevertheless the market for existing operating systems is so entrenched that little work continues on microkernel design. 

------------------------------------------------------------------------------------------------------------------

# Exokernel

- General
An exokernel is a type of operating system where the kernel is limited to extending resources to sub operating systems called LibOS's. Resulting in a very small, fast kernel environment. The theory behind this method is that by providing as few abstractions as possible programs are able to do exactly what they want in a controlled environment. Such as MS-DOS achieved through real mode, except with paging and other modern programming techniques.
- LibOS
LibOS's provide a way for the programmer of an exokernel type system to easily program cross-platform programs using familiar interfaces, instead of having to write his\her own. Moreover, they provide an additional advantage over monolithic kernels in that by having multiple LibOS's running at the same time, one can theoretically run programs from Linux, Windows, and Mac (Provided that there is a LibOS for that system) all at the same time, on the same OS, and with no performance issues. 

--------------------------------------------------------------------------------------------------------------------

# Hybrid 

A hybrid kernel is one that combines aspects of both micro and monolithic kernels, but there is no exact definition. Often, "hybrid kernel" means that the kernel is highly modular, but all runs in the same address space. This allows the kernel avoid the overhead of a complicated message passing system within the kernel, while still retaining some microkernel-like features. 


