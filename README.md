# Linux Driver Tutorial

Here you can find examples for simple Linux Kernel Modules and Linux Drivers.

## Preparation

I used a Raspberry Pi 3 to develop and test my modules and drivers. To compile them, you need to install the Kernel headers on your Pi. On Raspbian you can do this with the following command:

```bash
sudo apt install raspberrypi-kernel-headers
```

You also need the build utils (make, gcc, ...) but they come preinstalled on Raspbian.

## Content

In this repo you can find examples for:
1. Simple Kernel Module
2. Device Numbers and Device Files
3. Create device file in driver and callbacks
4. GPIO Driver
5. Text LCD Driver
6. PWM Module
7. Temperature Sensor (I2C)
8. Timer in Linux Kernel Modules
9. High Resolution Timer in Linux Kernel Modules
10. Accessing SPI with a Linux Kernel Module (BMP280 sensor again)
11. Using a GPIO Interrupt in a Linux Kernel Module

## More Information

For more information about my Linux Driver examples check out my [videos and my playlist](https://www.youtube.com/watch?v=x1Y203vH-Dc&list=PLCGpd0Do5-I3b5TtyqeF1UdyD4C-S-dMa)
