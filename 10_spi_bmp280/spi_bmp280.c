#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM to read and write some registers of a BMP280 sensor");


#define MY_BUS_NUM 0
static struct spi_device *bmp280_dev;

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	struct spi_master *master;
	u8 id;
	u8 val[] = {0x75, 0x40};

	/* Parameters for SPI device */
	struct spi_board_info spi_device_info = {
		.modalias = "bmp280",
		.max_speed_hz = 1000000,
		.bus_num = MY_BUS_NUM,
		.chip_select = 0,
		.mode = 3,
	};

	/* Get access to spi bus */
	master = spi_busnum_to_master(MY_BUS_NUM);
	/* Check if we could get the master */
	if(!master) {
		printk("There is no spi bus with Nr. %d\n", MY_BUS_NUM);
		return -1;
	}

	/* Create new SPI device */
	bmp280_dev = spi_new_device(master, &spi_device_info);
	if(!bmp280_dev) {
		printk("Could not create device!\n");
		return -1;
	}

	bmp280_dev -> bits_per_word = 8;

	/* Setup the bus for device's parameters */
	if(spi_setup(bmp280_dev) != 0){
		printk("Could not change bus setup!\n");
		spi_unregister_device(bmp280_dev);
		return -1;
	}

	/* Read Chip ID */
	id = spi_w8r8(bmp280_dev, 0xD0);
	printk("Chip ID: 0x%x\n", id);

	/* Write to config reg */
	spi_write(bmp280_dev, val, sizeof(8));
	id = spi_w8r8(bmp280_dev, 0xF5);
	printk("Config Reg. value: 0x%x\n", id);

	printk("Hello, Kernel!\n");

	
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	if(bmp280_dev)
		spi_unregister_device(bmp280_dev);
		
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


