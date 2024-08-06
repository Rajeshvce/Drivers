#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/ioctl.h>

#define mem_size 1024
#define READ_CHARACTER_DRIVER _IOR('c','1', int*)
#define WRITE_CHARACTER_DRIVER _IOW('c','2',int*)

static struct cdev *my_cdev;
static struct class *my_cdev_class;
static dev_t hello_char;
static struct device *my_cdev_device;
char *kernel_buffer;
int val = 0;

static int hello_open(struct inode *inode, struct file *filp)
{
    printk("Hello_open is opened\n");
    return 0;
}

ssize_t hello_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    if( copy_to_user(buf, kernel_buffer, mem_size) )
    {
        printk("Unable to read\n");
    }
    printk("Able to read\n");
    return mem_size;
}

ssize_t hello_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    if( copy_from_user(kernel_buffer, buf, len) )
    {
        printk("Unable to write\n");
    }
    printk("able to write\n");
    return len;
}

static int hello_release(struct inode* inode, struct file *filp)
{
    printk("Releasing the hello_world driver");
    return 0;
}

static long hello_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    switch(cmd){
        case READ_CHARACTER_DRIVER:
            if(copy_to_user((int*)arg, &val, sizeof(val)))
            {
                printk("Unable to read the character driver");
            }
            printk("val = %d\n", val);
            break;
        case WRITE_CHARACTER_DRIVER:
            if(copy_from_user(&val,(int*) arg, sizeof(val)))
            {
                printk("Unable to write the character driver");
            }
            printk("value = %d\n", val);
            break;
        default:
            printk("enter a valid argument");
            break;
    }
    return 0;
}

static struct file_operations my_cdev_fops = {
    .owner          = THIS_MODULE,
    .open           = hello_open,
    .read           = hello_read,
    .write          = hello_write,
    .release        = hello_release,
    .unlocked_ioctl = hello_ioctl,
};

static int __init start_character(void)
{
     int ret;

     //registering the device with dynamic allocation of major and minor numbers
     //require linux/fs.h header for this functions to work.
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

    //initialie the cdev data structure with the fileoperations
    cdev_init(my_cdev, &my_cdev_fops);

    //initialize the owner field of the owner module
    my_cdev->owner = THIS_MODULE;

    //Add the character device to the kernel
    cdev_add(my_cdev, hello_char, 1);

    //Create struct class
    my_cdev_class =  class_create(THIS_MODULE, "my_cdev_class");
    if(!my_cdev_class){
        printk(KERN_WARNING,"class create failed");
        return -1;
    }

    //Create a device node
    my_cdev_device = device_create(my_cdev_class, NULL, hello_char, NULL, "my_cdev");
    if(IS_ERR(my_cdev_device)){
        printk(KERN_WARNING"device create failed\n");
        return -1;
    }

    //Allocating memory for kernel_buffer from kernel ram (linux/slab.h)
    kernel_buffer = kmalloc(mem_size, GFP_KERNEL);
    if(!kernel_buffer){
        printk(KERN_WARNING"cannot allocate memory for kernel variable");
        return -1;
    }

    strcpy(kernel_buffer, "Hello_world");

    printk("Character driver initialization successfull!\n");
        return 0;
}

static void __exit exit_character(void)
{
    //Destroy the device
    device_destroy(my_cdev_class,my_cdev_device);

    //Destroy the class
    class_destroy(my_cdev_class);

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
