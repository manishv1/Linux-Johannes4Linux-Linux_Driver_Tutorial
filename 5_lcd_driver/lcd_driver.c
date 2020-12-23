#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A driver to write to a LCD text display");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "lcd"
#define DRIVER_CLASS "MyModuleClass"

/* LCD char buffer */
static char lcd_buffer[17];

/* Pinout for LCD Display */
unsigned int gpios[] = {
	3, /* Enable Pin */
	2, /* Register Select Pin */
	4, /* Data Pin 0*/
	17, /* Data Pin 1*/
	27, /* Data Pin 2*/
	22, /* Data Pin 3*/
	10, /* Data Pin 4*/
	9, /* Data Pin 5*/
	11, /* Data Pin 6*/
	5, /* Data Pin 7*/
};

#define ENABLE_PIN gpios[0]
#define REGISTER_SELECT gpios[1]

/**
 * @brief Function will generate a pulse on the enable signal
 */
void lcd_enable(void) {
	gpio_set_value(ENABLE_PIN, 1);
	msleep(5);
	gpio_set_value(ENABLE_PIN, 0);
}

/**
 * @brief send byte to the LCD
 *
 * @param data data to send
 */
void lcd_send_byte(char data) {
	int i;
	for(i=0; i<8; i++)
		gpio_set_value(gpios[i+2], ((data) & (1<<i)) >> i);
	lcd_enable();
	msleep(5);
}

/**
 * @brief sends a command to the lcd display
 *
 * @param cmd: cmd to send
 */
void lcd_cmd(uint8_t cmd) {
	gpio_set_value(REGISTER_SELECT, 0); /* RS to CMD */
	lcd_send_byte(cmd);
}

/**
 * @brief sends a byte to the lcd display
 *
 * @param data: char to send
 */
void lcd_data(uint8_t data) {
	gpio_set_value(REGISTER_SELECT, 1); /* RS to data */
	lcd_send_byte(data);
}

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta, i;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(lcd_buffer));

	/* Copy data to user */
	not_copied = copy_from_user(lcd_buffer, user_buffer, to_copy);

	/* Clear the display */
	lcd_cmd(0x1);

	/* Send Data to display */
	for(i=0; i<to_copy-1; i++)
		lcd_data(lcd_buffer[i]);

	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	int i;
	char *names[] = {"ENABLE_PIN", "REGISTER_SELECT", "DATA_PIN0", "DATA_PIN1", "DATA_PIN2", "DATA_PIN3", "DATA_PIN4", "DATA_PIN5", "DATA_PIN6", "DATA_PIN7"};
	printk("Hello, Kernel!\n");
	char text[] ="Hello lcd!";

	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not e created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("lcd-driver - Registering of device to kernel failed!\n");
		goto AddError;
	}

	/* Initialize GPIOs */
	printk("lcd-driver - GPIO Init\n");
	for(i=0; i<10; i++) {
		if(gpio_request(gpios[i], names[i])) {
			printk("lcd-driver - Error Init GPIO %d\n", gpios[i]);
			goto GpioInitError;
		}
	}

	printk("lcd-driver - Set GPIOs to output\n");
	for(i=0; i<10; i++) {
		if(gpio_direction_output(gpios[i], 0)) {
			printk("lcd-driver - Error setting GPIO %d to output\n", i);
			goto GpioDirectionError;
		}
	}

	printk("lcd-driver - Init LCD display\n");
	lcd_cmd(0x30);	/* Turn display on, 8 bit interface */

	lcd_cmd(0xf);	/* Display on, blinking cursor, only use 1 line */

	lcd_cmd(0x1);	/* Clear display */

	for(i=0; i<sizeof(text); i++)
		lcd_data(text[i]);
	

	return 0;
GpioDirectionError:
	i=9;
GpioInitError:
	for(;i>=0; i--)
		gpio_free(gpios[i]);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	int i;
	lcd_cmd(0x1);	/* Clear display */
	for(i=0; i<10; i++){
		gpio_set_value(gpios[i], 0);
		gpio_free(gpios[i]);
	}
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


