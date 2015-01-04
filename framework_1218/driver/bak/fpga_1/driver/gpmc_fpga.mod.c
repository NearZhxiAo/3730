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
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x796a78cc, "module_layout" },
	{ 0xdaee8348, "__class_create" },
	{ 0x3b080c0d, "device_create" },
	{ 0x4f89c9de, "gpmc_cs_free" },
	{ 0xadf42bd5, "__request_region" },
	{ 0xe9ce8b95, "omap_ioremap" },
	{ 0x20222be4, "cdev_add" },
	{ 0x22b61c98, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x5f754e5a, "memset" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x1a9df6cc, "malloc_sizes" },
	{ 0x8cf51d15, "up" },
	{ 0xbb4f6c53, "kmem_cache_alloc" },
	{ 0xb0bb9c02, "down_interruptible" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x37a0cba, "kfree" },
	{ 0x15331242, "omap_iounmap" },
	{ 0x9bce482f, "__release_region" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x734d612a, "cdev_del" },
	{ 0x36916f12, "class_destroy" },
	{ 0xc3fd4289, "device_destroy" },
	{ 0xea147363, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EE50510215AAB35634D402F");
