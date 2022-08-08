#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

#define CMD_GET_STATE 0x11
#define CMD_SET_STATE 0x22
#define CMD_GET_ADC_VAL 0x55

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A driver for my simple AMTEGA SPI ADC");

struct my_adc {
	struct spi_device *client;
};

static int my_adc_read_raw(struct iio_dev * indio_dev, struct iio_chan_spec const * chan, int *val, int *val2, long mask) {
	struct my_adc *adc = iio_priv(indio_dev);
	int ret;

	if(mask == IIO_CHAN_INFO_RAW) {
		ret = spi_w8r8(adc->client, CMD_GET_ADC_VAL);
		if(ret < 0) {
			printk("dt_iio - Error reading ADC value!\n");
			return ret;
		}
		*val = ret;
	}
	else 
		return -EINVAL;
	return IIO_VAL_INT;
}

static const struct iio_chan_spec my_adc_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	}
};

static const struct iio_info my_adc_info = {
	.read_raw = my_adc_read_raw,
};

/* Declate the probe and remove functions */
static int my_adc_probe(struct spi_device *client);
static int my_adc_remove(struct spi_device *client);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight,myadc",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct spi_device_id my_adc[] = {
	{"my_adc", 0},
	{ },
};
MODULE_DEVICE_TABLE(spi, my_adc);

static struct spi_driver my_driver = {
	.probe = my_adc_probe,
	.remove = my_adc_remove,
	.id_table = my_adc,
	.driver = {
		.name = "my_adc",
		.of_match_table = my_driver_ids,
	},
};

/**
 * @brief This function is called on loading the driver 
 */
static int my_adc_probe(struct spi_device *client) {
	struct iio_dev *indio_dev;
	struct my_adc *adc;
	int ret;
	u8 buffer[2];

	printk("dt_iio - Now I am in the Probe function!\n");

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(struct iio_dev));
	if(!indio_dev) {
		printk("dt_iio - Error! Out of memory\n");
		return -ENOMEM;
	}

	adc = iio_priv(indio_dev);
	adc->client = client;

	indio_dev->name = "myadc";
	indio_dev->info = &my_adc_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = my_adc_channels;
	indio_dev->num_channels = ARRAY_SIZE(my_adc_channels);

	ret = spi_setup(client);
	if(ret < 0) {
		printk("dt_iio - Error! Failed to set up the SPI Bus\n");
		return ret;
	}

	/* Set the ADC to power up mode */
	buffer[0] = CMD_SET_STATE;
	buffer[1] = 0x1;

	ret = spi_write(adc->client, buffer, 2);
	if(ret < 0) {
		printk("dt_iio - Error! Could not power ADC up\n");
		return -1;
	}
	
	spi_set_drvdata(client, indio_dev);

	return devm_iio_device_register(&client->dev, indio_dev);
}

/**
 * @brief This function is called on unloading the driver 
 */
static int my_adc_remove(struct spi_device *client) {
	struct iio_dev * indio_dev;
	struct my_adc *adc;
	u8 buffer[2];

	printk("dt_iio - Now I am in the Remove function!\n");
	/* Set device to power down mode */
	buffer[0] = CMD_SET_STATE;
	buffer[1] = 0x0;
	indio_dev = spi_get_drvdata(client);
	adc = iio_priv(indio_dev);

	spi_write(adc->client, buffer, 2);

	return 0;
}

/* This will create the init and exit function automatically */
module_spi_driver(my_driver);

