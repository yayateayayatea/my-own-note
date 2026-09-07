#include <linux/module.h> 
#include <linux/init.h> 

static int file01_init(void)
{
	printk(KERN_INFO "i am init\n");
	return 0;
}

static void file01_exit(void)
{
	printk(KERN_INFO "i am exit\n");
}

module_init(file01_init);
module_exit(file01_exit);

/* license */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("my first kernel module");