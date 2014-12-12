#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

#undef unix
struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = __stringify(KBUILD_MODNAME),
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x4c7d7355, "struct_module" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0x83b68fca, "remove_proc_entry" },
	{ 0x77cc196e, "register_chrdev" },
	{ 0xdd132261, "printk" },
	{ 0xf716e4e5, "remap_pfn_range" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

