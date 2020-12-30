#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xe9942317, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xa4ab8272, "cdev_del" },
	{ 0x88161187, "i2c_del_driver" },
	{ 0xe52760d4, "i2c_unregister_device" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x254652b1, "class_destroy" },
	{ 0x2d2f1e0c, "i2c_smbus_write_byte_data" },
	{ 0xc823420c, "i2c_smbus_read_word_data" },
	{ 0x32f973e3, "i2c_put_adapter" },
	{ 0xbdae2e40, "i2c_register_driver" },
	{ 0x7e3aa9b7, "i2c_new_device" },
	{ 0x2c5dad49, "i2c_get_adapter" },
	{ 0xda800db5, "device_destroy" },
	{ 0x570c6c9a, "cdev_add" },
	{ 0xad534415, "cdev_init" },
	{ 0x13e86e9b, "device_create" },
	{ 0xe6d85203, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x824e6bcf, "i2c_smbus_read_byte_data" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "2B212FF02796F4E09B49204");
