#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xe509ba75, "struct_module" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x9d669763, "memcpy" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x381f6ad5, "init_waitqueue_head" },
	{ 0xb794c614, "request_irq" },
	{ 0x9ad8806a, "class_device_create" },
	{ 0x66a4982f, "class_create" },
	{ 0x625e6c7c, "register_chrdev" },
	{ 0xbfbebf49, "clk_enable" },
	{ 0x9f949ed0, "clk_disable" },
	{ 0xafd441b4, "__wake_up" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0x843af3ad, "class_destroy" },
	{ 0xdd132261, "printk" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

