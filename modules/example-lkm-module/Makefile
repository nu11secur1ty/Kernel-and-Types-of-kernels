KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

# Formats any C-related file using the clang-format
# definition at the root of the project.
#
# Make sure you have clang-format installed before
# executing.
fmt:
	find . -name "*.c" -o -name "*.h" | \
		xargs clang-format -style=file -i


# Removes any binary generated.
clean:
	find . -name "*.out" -type f -delete
	make -c $(KDIR) M=`pwd`
