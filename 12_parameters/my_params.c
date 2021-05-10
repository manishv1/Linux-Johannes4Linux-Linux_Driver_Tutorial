#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM to demonstrate the use of parameters");

/* Kernel Module's Paramerters*/
static unsigned int gpio_nr = 12;
static char *device_name = "testdevice";

module_param(gpio_nr, uint, S_IRUGO);
module_param(device_name, charp, S_IRUGO);

MODULE_PARM_DESC(gpio_nr, "Nr. of GPIO to use in this LKM");
MODULE_PARM_DESC(device_name, "Device name to use in this LKM");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("gpio_nr = %u\n", gpio_nr);
	printk("device_name = %s\n", device_name);
	printk("Hello, Kernel!\n");
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


