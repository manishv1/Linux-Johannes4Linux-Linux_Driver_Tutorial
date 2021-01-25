# For Raspberry Pi 3

A Linux Driver Module is already using the Chip Select 0. To use it, the overlay *spidev_disabler.dts* is needed.

It can be compiled with:

```sh
dtc spidev_disabler.dts -O dtb >spidev_disabler.dtbo
```

and can be loaded with
```sh
sudo dtoverlay -d . spidev_disabler
```

Now the LKM can be loaded and the module should work!

For more information, see [this forum post](https://www.raspberrypi.org/forums/viewtopic.php?t=151423)

