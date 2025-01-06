#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajesh Mallempati");
MODULE_DESCRIPTION("First loadable kernel module for learning");

static struct proc_dir_entry *my_proc_dir;

//ssize_t  (*proc_read)(struct file *, char __user *, size_t, loff_t *);

static ssize_t read_proc(struct file* file_pointer, char* user_space_buffer, size_t count, loff_t* offset)
{
	printk("Reading from the proc entry\n");
	char msg[] = "Hii Rajesh\n";
	size_t len = strlen(msg);

	if(*offset >= len) return 0;
	int res;

	res = copy_to_user(user_space_buffer, msg,len);

	*offset += len;
	return len;
}

//ssize_t	(*proc_write)(struct file *, const char __user *, size_t, loff_t *);
static ssize_t write_proc(struct file* file_pointer,const char* user_space_buffer,size_t count, loff_t* offset)
{
	printk("Writing to the proc entry\n");
	char* kernel_msg;	
	kernel_msg = kmalloc(count+1, GFP_KERNEL);
    	if(!kernel_msg){
        	printk(KERN_WARNING "cannot allocate memory for kernel variable");
        	return -1;
    	}

	int res;
	res = copy_from_user(kernel_msg,user_space_buffer,count);
	
	kernel_msg[count] = '\0'; 
	printk("%s",kernel_msg);
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

	if(my_proc_dir == NULL) return -1;
	return 0;
}

static void my_module_exit(void){
	printk("Good bye from rajesh\n");
	proc_remove(my_proc_dir);
}

module_init(my_module_init);
module_exit(my_module_exit);
