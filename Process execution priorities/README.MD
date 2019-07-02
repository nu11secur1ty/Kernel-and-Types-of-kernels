# Learn Linux,Process execution priorities

- Keeping your eye on what's going on!!!

Overview

This tutorial grounds you in the basic Linux techniques for managing execution process priorities. Learn to:

   - Understand process priorities
   - Set process priorities
   - Change process priorities

This tutorial helps you prepare for Objective 103.6 in Topic 103 of the Linux Server Professional (LPIC-1) exam 101. The objective has a weight of 2.
Linux task priorities

Linux, like most modern operating systems, can run multiple processes. It does this by sharing the CPU and other resources among the processes. If one process can use 100 percent of the CPU, then other processes may become unresponsive. We’ll introduce you to the way Linux assigns priorities for tasks.


- Prerequisites

To get the most from the tutorials in this series, you should have a basic knowledge of Linux and a working Linux system on which you can practice the commands covered in this tutorial. Sometimes different versions of a program will format output differently, so your results may not always look exactly like the listings and figures shown here. The results in the examples shown here were obtained on a Ubuntu 15.04 distribution.

- Knowing your priorities

If you run the `top` command, its default is to display processes in decreasing order according to their CPU usage, as shown in Listing. If we ran that process, it probably wouldn’t make it onto the output list from top because the process spends most of its time not using the CPU.


# Listing 1. Typical output from top on a Linux workstation

```bash

top ‑ 22:47:44 up 1 day, 12:44,  3 users,  load average: 0.00, 0.01, 0.05
Tasks: 188 total,   1 running, 187 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.2 us,  0.0 sy,  0.0 ni, 99.8 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   8090144 total,  2145616 used,  5944528 free,    81880 buffers
KiB Swap:  4095996 total,   100660 used,  3995336 free.  1464920 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND       
 2215 ian       20   0 1549252 162644  79992 S   0.7  2.0  16:11.61 compiz        
    9 root      20   0       0      0      0 S   0.3  0.0   0:08.04 rcuos/0       
 4918 ian       20   0   29184   3120   2612 R   0.3  0.0   0:00.47 top           
    1 root      20   0  182732   5392   3648 S   0.0  0.1   0:03.74 systemd       
    2 root      20   0       0      0      0 S   0.0  0.0   0:00.02 kthreadd      
    3 root      20   0       0      0      0 S   0.0  0.0   0:00.10 ksoftirqd/0   
    5 root       0 ‑20       0      0      0 S   0.0  0.0   0:00.00 kworker/0:0H  
    7 root      20   0       0      0      0 S   0.0  0.0   0:08.42 rcu_sched     
    8 root      20   0       0      0      0 S   0.0  0.0   0:00.00 rcu_bh        
   10 root      20   0       0      0      0 S   0.0  0.0   0:00.00 rcuob/0       
   11 root      rt   0       0      0      0 S   0.0  0.0   0:00.00 migration/0   
   12 root      rt   0       0      0      0 S   0.0  0.0   0:00.77 watchdog/0    
   13 root      rt   0       0      0      0 S   0.0  0.0   0:00.78 watchdog/1    
   14 root      rt   0       0      0      0 S   0.0  0.0   0:00.18 migration/1   
   15 root      20   0       0      0      0 S   0.0  0.0   0:00.11 ksoftirqd/1   
   17 root       0 ‑20       0      0      0 S   0.0  0.0   0:00.00 kworker/1:0H  
   18 root      20   0       0      0      0 S   0.0  0.0   0:00.46 rcuos/1       
```

Your system may have many commands that are capable of using lots of CPU. Examples include movie editing tools, and programs to convert between different image types or between different sound encoding, such as mp3 to ogg.

When you only have one or a limited number of CPUs, you need to decide how to share those limited CPU resources among several competing processes. This is generally done by selecting one process for execution and letting it run for a short period (called a timeslice), or until it needs to wait for some event, such as IO to complete. To ensure that important processes don’t get starved out by CPU hogs, the selection is done based on a scheduling priority. The NI column in Listing 1 above, shows the scheduling priority or niceness of each process. Niceness generally ranges from -20 to 19, with -20 being the most favorable or highest priority for scheduling and 19 being the least favorable or lowest priority.


# Using ps to find niceness

In addition to the top command, you can also display niceness values using the ps command. You can either customize the output as you saw in the tutorial the output of ps -lps -l is shown in Listing 2. As with top, look for the niceness value in the NI column.

- Listing 2. Using ps to find niceness

```bash
                    
:~$  ps ‑l
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  3850  3849  0  80   0 ‑  6726 wait   pts/5    00:00:00 bash
0 R  1000  4924  3850  0  80   0 ‑  3561 ‑      pts/5    00:00:00 ps
```

# Default niceness

You may have guessed from Listing 1 or Listing 2 that the default niceness, at least for processes started by regular users, is 0. This is usually the case on current Linux systems. You can verify the value for your shell and system by running the nice command with no parameters as shown in Listing 3.


- Listing 3. Checking default niceness

```bash
:~$ nice
0
```

# Setting priorities

Before we look at how to set or change niceness values, let’s build a little CPU-intensive script that will show how niceness really works.

- A CPU-intensive script

We’ll create a small script that just uses CPU and does little else. The script takes two inputs, a count and a label. It prints the label and the current date and time, then spins, decrementing the count till it reaches 0, and finally prints the label and the date again. This script shown in Listing 4 has no error checking and is not very robust, but it illustrates our point.


- Listing 4. CPU-intensive script
```bash
   
:~$ echo 'x="$1"'>count1.sh
:~$ echo 'echo "$2" $(date)'>>count1.sh
:~$ echo 'while [ $x ‑gt 0 ]; do x=$(( x‑1 ));done'>>count1.sh
:~$ echo 'echo "$2" $(date)'>>count1.sh
:~$ cat count1.sh
x="$1"
echo "$2" $(date)
while [ $x ‑gt 0 ]; do x=$(( x‑1 ));done
echo "$2" $(date)
```

If you run this on your own system, you might see output similar to Listing 5. Depending on the speed of your system, you may have to increase the count value to even see a difference in the times. This script uses lots of CPU, as we’ll see in a moment. If your default shell is not Bash, and if the script does not work for you, then use the second form of calling shown below. If you are not using your own workstation, make sure that it is okay to use lots of CPU before you run the script.

- Listing 5. Running count1.sh

```bash
                    
:~$ sh count1.sh 10000 A
A Thu Jul 16 23:13:07 EDT 2015
A Thu Jul 16 23:13:07 EDT 2015
:~$ bash count1.sh 99000 A
A Thu Jul 16 23:13:53 EDT 2015
A Thu Jul 16 23:13:54 EDT 2015
```



So far, so good. Now let’s create a command list to run the script in background and launch the top command to see how much CPU the script is using. The command list is shown in Listing 6 and the output from top in Listing 7.

- Listing 6. Running count1.sh and top

```bash
:~$ (sh count1.sh 5000000 A&);top
```

- Listing 7. Using lots of CPU

```bash
                    
top ‑ 23:19:30 up 1 day, 13:16,  3 users,  load average: 0.15, 0.06, 0.05
Tasks: 190 total,   2 running, 188 sleeping,   0 stopped,   0 zombie
%Cpu(s): 25.1 us,  0.0 sy,  0.0 ni, 74.9 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   8090144 total,  2145600 used,  5944544 free,    82024 buffers
KiB Swap:  4095996 total,   100644 used,  3995352 free.  1464940 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND       
 4952 ian       20   0    4472    736    652 R 100.0  0.0   0:06.18 sh            
 2043 ian       20   0  552900  34544  24464 S   0.3  0.4   0:15.14 unity‑panel‑+ 
 2215 ian       20   0 1549252 162644  79992 S   0.3  2.0  16:28.20 compiz        
    1 root      20   0  182732   5392   3648 S   0.0  0.1   0:03.76 systemd       
    2 root      20   0       0      0      0 S   0.0  0.0   0:00.02 kthreadd      
    3 root      20   0       0      0      0 S   0.0  0.0   0:00.10 ksoftirqd/0   
```
Not bad. We are using 100 percent of one of the CPUs on this system with just a simple script. If you want to stress multiple CPUs, you can add an extra invocation of count1.sh to the command list. If we had a long running job such as this, we might find that it interfered with our ability (or the ability of other users) to do other work on our system.

# Using nice to set priorities

Now that we can keep a CPU busy for a while, we’ll see how to set a priority for a process. To summarize what we’ve learned so far:

   - Linux and UNIX® systems use a priority system with 40 priorities, ranging from -20 (highest priority) to 19 (lowest priority.
   - Processes started by regular users usually have priority 0.
   - The ps command can display the priority (nice, or NI, level, for example) using the -l option.
   - The nice command displays our default priority.

The nice command can also be used to start a process with a different priority. You use the -n or (--adjustment) option with a positive value to increase the priority value and a negative value to decrease it. Remember that processes with the lowest priority value run at highest scheduling priority, so think of increasing the priority value as being nice to other processes. Note that you usually need to be the superuser (root) to specify negative priority adjustments. In other words, regular users can usually only make their processes nicer.

To demonstrate the use of nice to set priorities, let’s start two copies of the count1.sh script in different subshells at the same time, but give one the maximum niceness of 19. After a second we’ll use ps -lps -l to display the process status, including niceness. Finally, we’ll add an arbitrary 30-second sleep to ensure the command sequence finishes after the two subshells do. That way, we won’t get a new prompt while we’re still waiting for output. The result is shown in Listing 8.

- Listing 8. Using nice to set priorities for a pair of processes

```bash 
                   
:~$  (sh count1.sh 2000000 A&);(nice ‑n 19 sh count1.sh 2000000 B&);> sleep 1;ps ‑l;sleep 10
A Fri Jul 17 17:10:33 EDT 2015
B Fri Jul 17 17:10:33 EDT 2015
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  3850  3849  0  80   0 ‑  6726 wait   pts/5    00:00:00 bash
0 R  1000  5614     1 99  80   0 ‑  1118 ‑      pts/5    00:00:01 sh
0 R  1000  5617     1 99  99  19 ‑  1118 ‑      pts/5    00:00:01 sh
0 R  1000  5620  3850  0  80   0 ‑  3561 ‑      pts/5    00:00:00 ps
A Fri Jul 17 17:10:36 EDT 2015
B Fri Jul 17 17:10:36 EDT 2015
```

Are you surprised that the two jobs finished at the same time? What happened to our priority setting? Remember that the script occupied one of our CPUs. This particular system runs on an Intel(R) Core(TM) i7 processor, which is very lightly loaded, so each core ran one process, and there wasn’t any need to prioritize them.

So let’s try starting four processes at four different niceness levels (0, 6, 12, and18) and see what happens. We’ll increase the busy count parameter for each so they run a little longer. Before you look at Listing 9, think about what you might expect, given what you’ve already seen.

- Listing 9. Using nice to set priorities for four of processes

```bash
                   
:~$ (sh count1.sh 5000000 A&);(nice ‑n 6 sh count1.sh 5000000 B&);
> (nice ‑n 12 sh count1.sh 5000000 C&);(nice ‑n 18 sh count1.sh 5000000 D&);
> sleep 1;ps ‑l;sleep 30
A Fri Jul 17 17:13:05 EDT 2015
C Fri Jul 17 17:13:05 EDT 2015
B Fri Jul 17 17:13:05 EDT 2015
D Fri Jul 17 17:13:05 EDT 2015
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  3850  3849  0  80   0 ‑  6726 wait   pts/5    00:00:00 bash
0 R  1000  5626     1 99  80   0 ‑  1118 ‑      pts/5    00:00:01 sh
0 R  1000  5629     1 99  86   6 ‑  1118 ‑      pts/5    00:00:01 sh
0 R  1000  5631     1 90  92  12 ‑  1118 ‑      pts/5    00:00:00 sh
0 R  1000  5633     1 58  98  18 ‑  1118 ‑      pts/5    00:00:00 sh
0 R  1000  5638  3850  0  80   0 ‑  3561 ‑      pts/5    00:00:00 ps
A Fri Jul 17 17:13:15 EDT 2015
B Fri Jul 17 17:13:21 EDT 2015
C Fri Jul 17 17:13:26 EDT 2015
D Fri Jul 17 17:13:27 EDT 2015
```
With four different priorities, we see the effect of the different niceness values as each job finishes in priority order. Try experimenting with different nice values to demonstrate the different possibilities for yourself.

A final note on starting processes with nice; as with the nohup command, you cannot use a command list or a pipeline as the argument of nice.

# Changing priorities
- `renice`

If you happen to start a process and realize that it should run at a different priority, there is a way to change it after it has started, using the renice command. You specify an absolute priority (and not an adjustment) for the process or processes to be changed as shown in Listing 10.

- Listing 10. Using renice to change priorities

```bash
                    
:~$ sh count1.sh 100000000 A&
[1] 5724
:~$ A Fri Jul 17 17:30:20 EDT 2015
renice 1 5724;ps ‑l 5724
5724 (process ID) old priority 0, new priority 1
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY        TIME CMD
0 R  1000  5724  3850 99  81   1 ‑  1118 ‑      pts/5      0:35 sh count1.sh 10000
:~$ renice +3 5724;ps ‑l 5724
5724 (process ID) old priority 1, new priority 3
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY        TIME CMD
0 R  1000  5724  3850 99  83   3 ‑  1118 ‑      pts/5      0:50 sh count1.sh 10000
:~$ sudo renice ‑8 5724;ps ‑l 5724
5724 (process ID) old priority 3, new priority ‑8
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY        TIME CMD
0 R  1000  5724  3850 99  72  ‑8 ‑  1118 ‑      pts/5      1:01 sh count1.sh 10000
```

Remember that you have to be the superuser to give your processes higher scheduling priority and make them less nice.

You can find more information on nice and renice in the main pages.

This concludes your introduction to process execution priorities.




