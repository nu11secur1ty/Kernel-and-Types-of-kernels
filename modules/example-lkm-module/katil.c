/**
 * Author V.Varbanovski nu11secur1ty
 * katil - a module that does nothing more than
 * printing `katil` using `printk`.
 */

/**
 * `module.h` contains the most basic functionality needed for
 * us to create a loadable kernel module, including the `MODULE_*`
 * macros, `module_*` functions and including a bunch of other
 * relevant headers that provide useful functionality for us
 * (for instance, `printk`, which comes from `linux/printk.h`,
 * a header included by `linux/module.h`).
 */
#include <linux/module.h>

/**
 * Following, we make use of several macros to properly provide
 * information about the kernel module that we're creating.
 *
 * The informations supplied here are visible through tools like
 * `modinfo`.
 *
 * Note.: the license you choose here **DOES AFFECT** other things -
 * by using a properietary license your kernel will be "tainted".
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("V.Varbanovsi. nu11secur1ty");
MODULE_DESCRIPTION("Katil LKM");
MODULE_VERSION("0.1");

/** hello_init - initializes the module
 *
 * The `hello_init` method defines the procedures that performs the set up
 * of our module.
 */
static int
hello_init(void)
{
         // By making use of `printk` here (in the initialization),
         // we can look at `dmesg` and verify that what we log here
         // appears there at the moment that we load the module with
         // `insmod`.
	printk(KERN_INFO "Ko staaa katil?\n");
	return 0;
}

static void
hello_exit(void)
{
        // similar to `init`, but for the removal time.
	printk(KERN_INFO "Ae beee si igraesh!\n");
}

// registers the `hello_init` method as the method to run at module
// insertion time.
module_init(hello_init);

// similar, but for `removal`
module_exit(hello_exit);
