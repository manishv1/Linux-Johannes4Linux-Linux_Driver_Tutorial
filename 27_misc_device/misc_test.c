#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple demonstration for a misc device");

#define MAX_SIZE 256
static char data[MAX_SIZE];
static size_t data_len;

static int my_open(struct inode *inode, struct file *file) {
	printk("misc_test - Open called\n");
	printk("misc_test - Device Numbers: %d %d\n", imajor(inode), iminor(inode));
	if(file->f_mode & FMODE_READ) 
		printk("misc_test - Open called with read permissions\n");
	if(file->f_mode & FMODE_WRITE) 
		printk("misc_test - Open called with write permissions\n");
	return 0;
}

static int my_close(struct inode *inode, struct file *file) {
	printk("misc_test - Close called\n");
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos) {
	int status;
	if(user_len > data_len)
		data_len = MAX_SIZE;
	else
		data_len = user_len;
	printk("misc_test - Write called\n");
	status = copy_from_user(data, user_buffer, data_len);
	if(status) {
		printk("misc_test - Error during copy_from_user\n");
		return -status;
	}
	return data_len;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos) {
	int status;
	size_t len;
	if(user_len < data_len)
		len = user_len;
	else
		len = data_len;
	printk("misc_test - Read called\n");
	status = copy_to_user(user_buffer, data, len);
	if(status) {
		printk("misc_test - Error during copy_to_user\n");
		return -status;
	}
	return len;
}


static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_close,
};

static struct miscdevice my_device = {
	.name = "testdev",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	int status;
	printk("misc_test - Register misc device\n");
	status = misc_register(&my_device);
	if(status) {
		printk("misc_test - Error during Register misc device\n");
		return -status;
	}
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("misc_test - Deregister misc device\n");
	misc_deregister(&my_device);
}

module_init(my_init);
module_exit(my_exit);


