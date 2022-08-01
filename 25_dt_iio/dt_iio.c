#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

#define CMD_GET_STATE 0x11
#define CMD_SET_STATE 0x22
#define CMD_GET_ADC_VAL 0x55

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A driver for my simple AMTEGA I2C ADC");

struct my_adc {
	struct i2c_client *client;
};

static int my_adc_read_raw(struct iio_dev * indio_dev, struct iio_chan_spec const * chan, int *val, int *val2, long mask) {
	struct my_adc *adc = iio_priv(indio_dev);
	int ret;

	if(mask == IIO_CHAN_INFO_RAW) {
		ret = i2c_smbus_read_byte_data(adc->client, CMD_GET_ADC_VAL);
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
static int my_adc_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int my_adc_remove(struct i2c_client *client);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight,myadc",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct i2c_device_id my_adc[] = {
	{"my_adc", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, my_adc);

static struct i2c_driver my_driver = {
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
static int my_adc_probe(struct i2c_client *client, const struct i2c_device_id *id) {
	struct iio_dev *indio_dev;
	struct my_adc *adc;
	int ret;

	printk("dt_iio - Now I am in the Probe function!\n");

	if(client->addr != 0x10) {
		printk("dt_iio - Wrong I2C address!\n");
		return -1;
	}

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(struct iio_dev));
	if(!indio_dev) {
		printk("dt_iio - Error! Out of memory\n");
		return -ENOMEM;
	}

	adc = iio_priv(indio_dev);
	adc->client = client;

	indio_dev->name = id->name;
	indio_dev->info = &my_adc_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = my_adc_channels;
	indio_dev->num_channels = ARRAY_SIZE(my_adc_channels);

	/* Set the ADC to power up mode */
	ret = i2c_smbus_write_byte_data(adc->client, CMD_SET_STATE, 0x1);
	if(ret < 0) {
		printk("dt_iio - Error! Could not power ADC up\n");
		return -1;
	}

	/* Not shown in video, but useful! */
	/* Save indio_dev as i2c device data */
	i2c_set_clientdata(client, indio_dev);

	return devm_iio_device_register(&client->dev, indio_dev);
}

/**
 * @brief This function is called on unloading the driver 
 */
static int my_adc_remove(struct i2c_client *client) {
	struct iio_dev * indio_dev;
	struct my_adc *adc;

	printk("dt_iio - Now I am in the Remove function!\n");
	/* Not shown in video, but useful! */
	/* Set device to power down mode */
	indio_dev = i2c_get_clientdata(client);
	adc = iio_priv(indio_dev);
	i2c_smbus_write_byte_data(adc->client, CMD_SET_STATE, 0x0);

	return 0;
}

/* This will create the init and exit function automatically */
module_i2c_driver(my_driver);

