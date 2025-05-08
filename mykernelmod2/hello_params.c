#include <linux/module.h>
#include <linux/kernel.h>


/* Why THIS won't work... 
static int cnt = 3;
static int cnt_arr[cnt]  = {1, 2, 3};
C does NOT allow variable-length arrays at file scope...
 static means cnt_arry is at file scope...
 since it is of length cnt...
 	cnt is static, meaning it is at file scope 
 	BUT also means that its value can change throughout the program...
*/
// Do THIS instead...
#define MAX_CNT 100
static int cnt_arr[MAX_CNT]  = {1, 2, 3};
static int cnt = 0;

static char* dn = "";
static char* name = "Misa";
static char* alph_arr = "abcdefghijklmnopqrstuvwxyz";


static int hello_init(void){
	printk(KERN_EMERG "Hello %s!\n", name);
	printk(KERN_EMERG "So there's another %s!?\n", dn);
	int i = 0;
	printk(KERN_EMERG "I can count...\n");
	// use cnt, which is either 0, or length of cnt_arr provided by user,
	// 	in order to iterate through cnt_arr and arr
	while (i < cnt){
	printk(KERN_EMERG " %d ", cnt_arr[i]);
	printk(KERN_EMERG " %c ", alph_arr[i]);
		i = i + 1;
	}
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
// takes name of var, its type & perms
// module_param(name, type, perm);
//MODULE_PARM_DESC(_parm, desc);

// tell kernel ab nameparam
MODULE_PARM_DESC(name, "a string");
module_param(name, charp, 0);

// tell kernel ab alph_arr param
MODULE_PARM_DESC(alph_arr, "char array for abc");
module_param(alph_arr, charp, 0);

/*
 module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */

module_param_array(cnt_arr, int, &cnt, 0);
MODULE_PARM_DESC(cnt_arr, "int array for 123");

// tell kernel ab dn param
MODULE_PARM_DESC(dn, "string passed via 'dn=<value' when inserting kernel module... ie. insmod hello_params.ko dn='bundt cake'");
module_param(dn, charp, 0);

// tell the kernel src & build system what module license we're using...
// follow GPL -- General Public License -- ensure that code interacting w Kernel follows open-source protocols and standards... :)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Light Yagami");
MODULE_DESCRIPTION("Someone has to do it, so why not me?");
