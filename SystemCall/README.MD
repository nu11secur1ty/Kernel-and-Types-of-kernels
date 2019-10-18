What is a system call?

In the most literal sense, a system call (also called a "syscall") is an instruction, similar to the "add" instruction or the "jump" instruction. At a higher level, a system call is the way a user level program asks the operating system to do something for it. If you're writing a program, and you need to read from a file, you use a system call to ask the operating system to read the file for you.
System calls in detail

Here's how a system call works. First, the user program sets up the arguments for the system call. One of the arguments is the system call number (more on that later). Note that all this is done automatically by library functions unless you are writing in assembly. After the arguments are all set up, the program executes the "system call" instruction. This instruction causes an exception: an event that causes the processor to jump to a new address and start executing the code there.

The instructions at the new address save your user program's state, figure out what system call you want, call the function in the kernel that implements that system call, restores your user program state, and returns control back to the user program. A system call is one way that the functions defined in a device driver end up being called.

That was the whirlwind tour of how a system call works. Next, we'll go into minute detail for those who are curious about exactly how the kernel does all this. Don't worry if you don't quite understand all of the details - just remember that this is one way that a function in the kernel can end up being called, and that no magic is involved. You can trace the control flow all the way through the kernel - with difficulty sometimes, but you can do it.
