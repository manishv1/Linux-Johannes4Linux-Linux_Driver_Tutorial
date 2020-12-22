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
7. PWM Module
8. Temperature Sensor (I2C)
9. Servo Motor Driver (High Resolution Timer)
