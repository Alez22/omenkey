#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcb8b6ec6, "kfree" },
	{ 0xfbe7861b, "memcpy" },
	{ 0x0e9cab28, "memset" },
	{ 0x9aa6980d, "mutex_lock" },
	{ 0x9aa6980d, "mutex_unlock" },
	{ 0xdd6830c7, "sysfs_emit" },
	{ 0x91f966bb, "kstrtou8" },
	{ 0x66648a76, "platform_device_register_full" },
	{ 0xe6112ad6, "wmi_has_guid" },
	{ 0xb880fdff, "sysfs_create_group" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x8e142c2e, "kstrtouint" },
	{ 0xd272d446, "__fentry__" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x2435d559, "strncmp" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x3962fd35, "sysfs_remove_group" },
	{ 0x97708cf7, "platform_device_unregister" },
	{ 0xe8213e80, "_printk" },
	{ 0xb1aae510, "wmi_evaluate_method" },
	{ 0xe63769e7, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xcb8b6ec6,
	0xfbe7861b,
	0x0e9cab28,
	0x9aa6980d,
	0x9aa6980d,
	0xdd6830c7,
	0x91f966bb,
	0x66648a76,
	0xe6112ad6,
	0xb880fdff,
	0x90a48d82,
	0x8e142c2e,
	0xd272d446,
	0xbd03ed67,
	0x2435d559,
	0xd09b06f5,
	0xd272d446,
	0xd272d446,
	0x3962fd35,
	0x97708cf7,
	0xe8213e80,
	0xb1aae510,
	0xe63769e7,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"kfree\0"
	"memcpy\0"
	"memset\0"
	"mutex_lock\0"
	"mutex_unlock\0"
	"sysfs_emit\0"
	"kstrtou8\0"
	"platform_device_register_full\0"
	"wmi_has_guid\0"
	"sysfs_create_group\0"
	"__ubsan_handle_out_of_bounds\0"
	"kstrtouint\0"
	"__fentry__\0"
	"__ref_stack_chk_guard\0"
	"strncmp\0"
	"kstrtoint\0"
	"__x86_return_thunk\0"
	"__stack_chk_fail\0"
	"sysfs_remove_group\0"
	"platform_device_unregister\0"
	"_printk\0"
	"wmi_evaluate_method\0"
	"module_layout\0"
;

MODULE_INFO(depends, "wmi");


MODULE_INFO(srcversion, "DC3F55D167676E111183BA1");
