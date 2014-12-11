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
	{ 0xf20dabd8, "free_irq" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xb794c614, "request_irq" },
	{ 0x77cc196e, "register_chrdev" },
	{ 0x8a7baba5, "__wake_up" },
	{ 0x6b2941b2, "__arch_copy_to_user" },
	{ 0xaf507de1, "__arch_copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xdd132261, "printk" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

