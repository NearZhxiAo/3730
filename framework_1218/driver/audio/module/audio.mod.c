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
	{ 0xb006335a, "remap_pfn_range" },
	{ 0x625e6c7c, "register_chrdev" },
	{ 0xdd132261, "printk" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0x1765022d, "remove_proc_entry" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

