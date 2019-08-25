hello-world-lkm - a "hello-world" loadable kernel module

Usage:

        1. install the necessary development headers

                sudo apt update -y
                sudo apt install -y libelf-dev linux-headers-`uname -r`

        2. build the kernel module

                make kbuild

        3. load the module

                sudo insmod ./hello.ko

        4. check that the module has been inserted

                lsmod | grep hello

        5. check that the module has been really loaded

                dmesg

        6. remove the module

                sudo rmmod hello.ko

https://ops.tips
