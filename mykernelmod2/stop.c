#include <linux/module.h>
#include <linux/kernel.h>

static void stop(void){
	printk(KERN_EMERG "止める!\n");
}

module_exit(stop);
