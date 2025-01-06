#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>

#define mem_size 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajesh Mallempati");
MODULE_DESCRIPTION("First loadable kernel module for learning");

static struct proc_dir_entry *my_proc_dir;
static char* kernel_buffer;
static size_t data_size = 0;
//ssize_t  (*proc_read)(struct file *, char __user *, size_t, loff_t *);

static ssize_t read_proc(struct file* file_pointer, char* user_space_buffer, size_t count, loff_t* offset)
{
	printk("Reading from the proc entry\n");
	int res;
	
	/*to handle the memory limit of the user_space_buffer
	size_t bytes_to_read = data_size - *offset;
	if (count < bytes_to_read) {
    		bytes_to_read = count;
	}*/

	if(*offset >= data_size) return 0;

	res = copy_to_user(user_space_buffer,kernel_buffer, data_size);
	if(res){
		printk("Unable to read\n");
		return EFAULT;
	}
	
	*offset += data_size;
	return data_size;
}

//ssize_t	(*proc_write)(struct file *, const char __user *, size_t, loff_t *);
static ssize_t write_proc(struct file* file_pointer,const char* user_space_buffer,size_t count, loff_t* offset)
{
	printk("Writing to the proc entry\n");
	
	if(count > mem_size){
		count = mem_size;
	}

	int res;
	res = copy_from_user(kernel_buffer,user_space_buffer,count);
	if(res){
		printk("Unable to write\n");
		return -EFAULT;
	}		
	kernel_buffer[count] = '\0'; 
	data_size = count;

	printk("%s",kernel_buffer);
	return count;
}

struct proc_ops driver_proc_ops = {
	.proc_read  = read_proc,
	.proc_write = write_proc,
};

static int my_module_init(void){
	printk("Hello world module from rajesh\n");
	my_proc_dir = proc_create("my_proc_entry", 0666,
		       	NULL, &driver_proc_ops);
	
	kernel_buffer = kmalloc(mem_size, GFP_KERNEL);
	if(!kernel_buffer){
                printk(KERN_WARNING "cannot allocate memory for kernel variable");
                return -1;
        }

	if(my_proc_dir == NULL) return -1;
	return 0;
}

static void my_module_exit(void){
	printk("Good bye from rajesh\n");
	proc_remove(my_proc_dir);
	kfree(kernel_buffer);
}

module_init(my_module_init);
module_exit(my_module_exit);
