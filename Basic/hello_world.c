#include <linux/init.h>
#include <linux/module.h>

static int __init startmodule(void){
    printk("Hello world! \n");
    return 0;
}

static void __exit killmodule(void){
    printk("Leaving the world! \n");
}

static char *whom = "world";
static int howmany = 1;

module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

MODULE_AUTHOR("Rajesh");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world driver");

module_init(startmodule);
module_exit(killmodule);
