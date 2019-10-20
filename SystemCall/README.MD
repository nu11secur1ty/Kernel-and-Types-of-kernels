# What is a system call?

In the most literal sense, a system call (also called a "syscall") is an instruction, similar to the "add" instruction or the "jump" instruction. At a higher level, a system call is the way a user level program asks the operating system to do something for it. If you're writing a program, and you need to read from a file, you use a system call to ask the operating system to read the file for you.

-----------------------------------------------------------------------------------------------------------------------------
- System calls in detail

Here's how a system call works. First, the user program sets up the arguments for the system call. One of the arguments is the system call number (more on that later). Note that all this is done automatically by library functions unless you are writing in assembly. After the arguments are all set up, the program executes the "system call" instruction. This instruction causes an exception: an event that causes the processor to jump to a new address and start executing the code there.

The instructions at the new address save your user program's state, figure out what system call you want, call the function in the kernel that implements that system call, restores your user program state, and returns control back to the user program. A system call is one way that the functions defined in a device driver end up being called.

That was the whirlwind tour of how a system call works. Next, we'll go into minute detail for those who are curious about exactly how the kernel does all this. Don't worry if you don't quite understand all of the details - just remember that this is one way that a function in the kernel can end up being called, and that no magic is involved. You can trace the control flow all the way through the kernel - with difficulty sometimes, but you can do it.

-----------------------------------------------------------------------------------------------------------------------------
- A system call example

This is a good place to start showing code to go along with the theory. We'll follow the progress of a read() system call, starting from the moment the system call instruction is executed. The PowerPC architecture will be used as an example for the architecture specific part of the code. On the PowerPC, when you execute a system call, the processor jumps to the address 0xc00. The code at that location is defined in the file:

`arch/ppc/kernel/head.S`

It looks something like this:

```bash
/* System call */
        . = 0xc00
SystemCall:
        EXCEPTION_PROLOG
        EXC_XFER_EE_LITE(0xc00, DoSyscall)

/* Single step - not used on 601 */
        EXCEPTION(0xd00, SingleStep, SingleStepException, EXC_XFER_STD)
        EXCEPTION(0xe00, Trap_0e, UnknownException, EXC_XFER_EE)
```

What this code does is save some state and call another function called `DoSyscall`. Here's a more detailed explanation (feel free to skip this part):

`EXCEPTION_PROLOG` is a macro that handles the switch from user to kernel space, which requires things like saving the register state of the user process. `EXC_XFER_EE_LITE` is called with the address of this routine, and the address of the function `DoSyscall`. Eventually, some state will be saved and `DoSyscall` will be called. The next two lines save two exception vectors on the addresses`0xd00` and `0xe00`.

`EXC_XFER_EE_LITE` looks like this:

```bash
#define EXC_XFER_EE_LITE(n, hdlr)       \
        EXC_XFER_TEMPLATE(n, hdlr, n+1, COPY_EE, transfer_to_handler, \
                          ret_from_except)
```

`EXC_XFER_TEMPLATE` is another macro, and the code looks like this:

```bash
#define EXC_XFER_TEMPLATE(n, hdlr, trap, copyee, tfer, ret)     \
        li      r10,trap;                                       \
        stw     r10,TRAP(r11);                                  \
        li      r10,MSR_KERNEL;                                 \
        copyee(r10, r9);                                        \
        bl      tfer;                                           \
##n:                                                            \
        .long   hdlr;                                           \
        .long   ret
```

`li` stands for "load immediate", which means that a constant value known at compile time is stored in a register. First, `trap` is loaded into the register `r10`. On the next line, that value is stored on the address given by `TRAP(r11)`. `TRAP(r11)` and the next two lines do some hardware specific bit manipulation. After that we call the `tfer` function (i.e. the `transfer_to_handler` function), which does yet more housekeeping, and then transfers control to `hdlr` (i.e. `DoSyscall`). Note that `transfer_to_handler` loads the address of the handler from the link register, which is why you see `.long DoSyscall` instead of `bl DoSyscall`.

Now, let's look at `DoSyscall`. It's in the file:

`arch/ppc/kernel/entry.S`

Eventually, this function loads up the address of the syscall table and indexes into it using the system call number. The syscall table is what the OS uses to translate from a system call number to a particular system call. The system call table is named `sys_call_table` and defined in:

`arch/ppc/kernel/misc.S`

The syscall table contains the addresses of the functions that implement each system call. For example, the read() system call function is named `sys_read`. The `read()` system call number is 3, so the address of sys_read() is in the 4th entry of the system call table (since we start numbering the system calls with 0). We read the data from the address sys_call_table + (3 * word_size) and we get the address of `sys_read()`.

After `DoSyscall` has looked up the correct system call address, it transfers control to that system call. Let's look at where `sys_read()` is defined, in the file:
`fs/read_write.c`

This function finds the file struct associated with the fd number you passed to the `read()` function. That structure contains a pointer to the function that should be used to read data from that particular kind of file. After doing some checks, it calls that file-specific read function in order to actually read the data from the file, and then returns. This file-specific function is defined somewhere else - the socket code, filesystem code, or device driver code, for example. This is one of the points at which a specific kernel subsystem finally interfaces with the rest of the kernel. After our read function finishes, we return from the `sys_read()`, back to `DoSyscall()`, which switches control to `ret_from_except`, which is in defined in:

`arch/ppc/kernel/entry.S`

This checks for tasks that might need to be done before switching back to user mode. If nothing else needs to be done, we fall through to the `restore` function, which restores the user process's state and returns control back to the user program. There! Your `read()` call is done! If you're lucky, you even got your data back.

You can explore syscalls further by putting printks at strategic places. Be sure to limit the amount of output from these printks. For example, if you add a `printk` to `sys_read()` syscall, you should do something like this:

```bash
  static int mycount = 0;

  if (mycount < 10) {
           printk ("sys_read called\n");
	   mycount++;
  }
 ```
 # BR nu11secur1ty!
