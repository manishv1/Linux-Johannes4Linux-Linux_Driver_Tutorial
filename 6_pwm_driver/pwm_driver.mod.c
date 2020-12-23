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
	{ 0xa4ab8272, "cdev_del" },
	{ 0xc32a1c5d, "pwm_free" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x254652b1, "class_destroy" },
	{ 0xda800db5, "device_destroy" },
	{ 0x6b250fcf, "pwm_request" },
	{ 0x570c6c9a, "cdev_add" },
	{ 0xad534415, "cdev_init" },
	{ 0x13e86e9b, "device_create" },
	{ 0xe6d85203, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x5f754e5a, "memset" },
	{ 0x7941ad4, "pwm_apply_state" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D90BC1E26C96FAD4027433C");
