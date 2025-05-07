#include <linux/module.h>
#include <linux/kernel.h>

static int hello_init(void){
	printk(KERN_EMERG "Hello world!\n");
	return 0;
}

/*
If printk loglevel is higher than that of console, then message will print to console...
--> check console loglevel:
	lighty@g4m1:~/mykernelmod$ sudo cat /proc/sys/kernel/printk
	4	4	1	7
--> want our printk loglevel to be 3 or higher...
  */
 
static void hello_exit(void){
	printk(KERN_EMERG "Goodbye world\n");
}

// when kernel invokes its module_init function, tell it to point to hello_init
// Register hello_init as this kernel module's initialization function...
//   module_init tells the kernel what to load (by pointing to hello_init fxn) when this src code is compiled
//   during compilation, it places a ptr/addr in a special ELF section (.init.text or .initcall6.init) that the kernel will later run when initializing this module
module_init(hello_init);
module_exit(hello_exit);

// tell the kernel src & build system what module license we're using...
// follow GPL -- General Public License -- ensure that code interacting w Kernel follows open-source protocols and standards... :)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Light Yagami");
MODULE_DESCRIPTION("Someone has to do it, so why not me?");
