# Installing the patch, which you want.
- Run 
```bash
./nu11secur1ty
```
- .This patch is for stopping of load LKM Linux Modules when the user decides to play with Linux Kernel. Or to block attacker to execute some rootkit malicious software on your system! NOTE:  Tested only on Linux OS Opensuse Leap 15 
- Documentation:
-----------------------------------------------------------------------
A significant portion of kernel vulnerabilities do not affect core code,
but rather individual modules.  Unfortunately, there is no global kernel
setting to restrict unprivileged users from triggering the automatic
loading of kernel modules, for example by creating a socket using a
packet family that is compiled as a module and not already loaded.  On
most distributions, this creates a significant attack surface, and
requires maintenance of blacklists and other inelegant solutions to a
general problem.

The below patch replaces the existing "modules_disable" sysctl with a
finer-grained "modules_restrict" sysctl.  By default, this is set at 0,
which results in no deviation from normal module loading behavior.  When
set to 1, unprivileged users cannot trigger the automatic loading of
modules.  This behavior is based on grsecurity's GRKERNSEC_MODHARDEN
setting.  The current check is against current_uid(), since several
distributions explicitly remove CAP_SYS_MODULE from root processes, some
of which incidentally cause (and rely on) the automatic loading of
modules.  I expect this to be a point of discussion.

When set to 2, modules may not be loaded or unloaded by anyone, and the
sysctl may not be changed from that point forward.  This is designed to
provide protection against kernel module rootkits.

Signed-off-by: Dan Rosenberg <drosenberg@vsecurity.com>

```
---
 Documentation/sysctl/kernel.txt |   16 +++++++++-------
 include/linux/module.h          |    5 ++++-
 kernel/kmod.c                   |    4 ++++
 kernel/module.c                 |   21 ++++++++++++++++++---
 kernel/sysctl.c                 |   11 +++++------
 5 files changed, 40 insertions(+), 17 deletions(-)

diff --git a/Documentation/sysctl/kernel.txt b/Documentation/sysctl/kernel.txt
index 3894eaa..e000214 100644
--- a/Documentation/sysctl/kernel.txt
+++ b/Documentation/sysctl/kernel.txt
@@ -36,7 +36,7 @@ show up in /proc/sys/kernel:
 - kstack_depth_to_print       [ X86 only ]
 - l2cr                        [ PPC only ]
 - modprobe                    ==> Documentation/debugging-modules.txt
-- modules_disabled
+- modules_restrict
 - msgmax
 - msgmnb
 - msgmni
@@ -254,13 +254,15 @@ kernel stack.
 
 ==============================================================
 
-modules_disabled:
+modules_restrict:
 
-A toggle value indicating if modules are allowed to be loaded
-in an otherwise modular kernel.  This toggle defaults to off
-(0), but can be set true (1).  Once true, modules can be
-neither loaded nor unloaded, and the toggle cannot be set back
-to false.
+A value indicating if module loading is restricted in an 
+otherwise modular kernel.  This value defaults to off (0), 
+but can be set to (1) or (2).  If set to (1), modules cannot 
+be auto-loaded by non-root users, for example by creating a 
+socket using a packet family that is compiled as a module and 
+not already loaded.  If set to (2), modules can neither be 
+loaded nor unloaded, and the value can no longer be changed.
 
 ==============================================================
 
diff --git a/include/linux/module.h b/include/linux/module.h
index b29e745..869039a 100644
--- a/include/linux/module.h
+++ b/include/linux/module.h
@@ -175,7 +175,10 @@ struct notifier_block;
 
 #ifdef CONFIG_MODULES
 
-extern int modules_disabled; /* for sysctl */
+int module_proc_update_handler(struct ctl_table *table, int write,
+	void __user *buffer, size_t *length, loff_t *ppos);
+
+extern int modules_restrict; /* for sysctl */
 /* Get/put a kernel symbol (calls must be symmetric) */
 void *__symbol_get(const char *symbol);
 void *__symbol_get_gpl(const char *symbol);
diff --git a/kernel/kmod.c b/kernel/kmod.c
index 9cd0591..e2ab82b 100644
--- a/kernel/kmod.c
+++ b/kernel/kmod.c
@@ -90,6 +90,10 @@ int __request_module(bool wait, const char *fmt, ...)
 	if (ret)
 		return ret;
 
+	/* Can non-root users cause auto-loading of modules? */
+	if (current_uid() && modules_restrict)
+		return -EPERM;
+
 	/* If modprobe needs a service that is in a module, we get a recursive
 	 * loop.  Limit the number of running kmod threads to max_threads/2 or
 	 * MAX_KMOD_CONCURRENT, whichever is the smaller.  A cleaner method
diff --git a/kernel/module.c b/kernel/module.c
index 437a74a..cdc5c8c 100644
--- a/kernel/module.c
+++ b/kernel/module.c
@@ -56,6 +56,7 @@
 #include <linux/percpu.h>
 #include <linux/kmemleak.h>
 #include <linux/jump_label.h>
+#include <linux/sysctl.h>
 
 #define CREATE_TRACE_POINTS
 #include <trace/events/module.h>
@@ -88,7 +89,7 @@ struct list_head *kdb_modules = &modules; /* kdb needs the list of modules */
 
 
 /* Block module loading/unloading? */
-int modules_disabled = 0;
+int modules_restrict = 0;
 
 /* Waiting for a module to finish initializing? */
 static DECLARE_WAIT_QUEUE_HEAD(module_wq);
@@ -125,6 +126,20 @@ struct load_info {
 	} index;
 };
 
+/* Proc update handler for modules_restrict sysctl */
+int module_proc_update_handler(struct ctl_table *table, int write,
+	void __user *buffer, size_t *length, loff_t *ppos)
+{
+
+	/* If module loading is entirely disabled, do not allow
+	 * it to be re-enabled. */
+	if (modules_restrict == 2)
+		return -EPERM;
+
+	return proc_dointvec_minmax(table, write, buffer, length, ppos);
+
+}
+
 /* We require a truly strong try_module_get(): 0 means failure due to
    ongoing or failed initialization etc. */
 static inline int strong_try_module_get(struct module *mod)
@@ -734,7 +749,7 @@ SYSCALL_DEFINE2(delete_module, const char __user *, name_user,
 	char name[MODULE_NAME_LEN];
 	int ret, forced = 0;
 
-	if (!capable(CAP_SYS_MODULE) || modules_disabled)
+	if (!capable(CAP_SYS_MODULE) || (modules_restrict == 2))
 		return -EPERM;
 
 	if (strncpy_from_user(name, name_user, MODULE_NAME_LEN-1) < 0)
@@ -2699,7 +2714,7 @@ SYSCALL_DEFINE3(init_module, void __user *, umod,
 	int ret = 0;
 
 	/* Must have permission */
-	if (!capable(CAP_SYS_MODULE) || modules_disabled)
+	if (!capable(CAP_SYS_MODULE) || (modules_restrict == 2))
 		return -EPERM;
 
 	/* Do all the hard work */
diff --git a/kernel/sysctl.c b/kernel/sysctl.c
index c33a1ed..37b7964 100644
--- a/kernel/sysctl.c
+++ b/kernel/sysctl.c
@@ -552,14 +552,13 @@ static struct ctl_table kern_table[] = {
 		.proc_handler	= proc_dostring,
 	},
 	{
-		.procname	= "modules_disabled",
-		.data		= &modules_disabled,
+		.procname	= "modules_restrict",
+		.data		= &modules_restrict,
 		.maxlen		= sizeof(int),
 		.mode		= 0644,
-		/* only handle a transition from default "0" to "1" */
-		.proc_handler	= proc_dointvec_minmax,
-		.extra1		= &one,
-		.extra2		= &one,
+		.proc_handler	= module_proc_update_handler,
+		.extra1		= &zero,
+		.extra2		= &two,
 	},
 #endif
 #ifdef CONFIG_HOTPLUG
```
[More:](https://xorl.wordpress.com/2018/02/17/lkm-loading-kernel-restrictions/?fbclid=IwAR2Sa4CaS0l1jOVNzngk1G6pvLdC7LexrIuK8wNQNvDEFWmUInicVfuG1cs)

# BR nu11secur1ty
