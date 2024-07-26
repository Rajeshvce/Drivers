#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/fs.h>


static struct cdev *my_cdev;
static dev_t hello_char;

static int hello_open(struct inode *inode, struct file *filp)
{
    printk("Hello_open is opened\n");
    return 0;
}

ssize_t hello_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk("In hello_read\n");
    return 0;
}

ssize_t hello_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk("In hello_write\n");
    return 0;
}

static int hello_release(struct inode* inode, struct file *filp)
{
    printk("Releasing the hello_world driver");
    return 0;
}

static struct file_operations my_cdev_fops = {
    .owner   = THIS_MODULE,
    .open    = hello_open,
    .read    = hello_read,
    .write   = hello_write,
    .release = hello_release,
};

static int __init start_character(void)
{
	int ret;

	//registering the device with dynamic allocation of major and minor numbers
	ret = alloc_chrdev_region(&hello_char, 0, 1, "character_driver");
	if(ret != 0){
		printk(KERN_WARNING"alloc_chrdev_region is failed!\n");
		return -1;
	}
    
    //Kernel uses structure of type struct cdev to represent character devices internally.
    //allocate the cdev structure
    my_cdev = cdev_alloc();
    if(!my_cdev){
        printk(KERN_WARNING"cdev_alloc failed");
        return -1;
    }
    
    //initialize the cdev data structure
    my_cdev->owner = THIS_MODULE;
    my_cdev->ops   = &my_cdev_fops;

    //Add the character device to the kernel
    cdev_add(my_cdev, hello_char, 1);

    printk("Character driver initialization successfull!\n");
	return 0;	
}

static void __exit exit_character(void)
{
    //delete the created cdev structure
    cdev_del(my_cdev);

    //unregister the character driver
    unregister_chrdev_region(hello_char,1);

    printk("Character driver is exited successfully!\n");
}

MODULE_AUTHOR("Rajesh Mallempati");
MODULE_DESCRIPTION(" Demo character driver");
MODULE_LICENSE("GPL");

module_init(start_character);
module_exit(exit_character);
