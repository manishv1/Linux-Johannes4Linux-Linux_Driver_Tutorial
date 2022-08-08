# Using the Atmega I2C ADC

I recommend to decrease the I2C frequency so that the I2C bus is not to fast for my slow Atmega I2C ADC (I use an 8MHz oscillator).

To decrease the frequency, add the following at the end of /boot/config.txt

~~~
dtparam i2c_baudrate=10000
~~~


And also don't forget to enable I2C over raspi-config!
