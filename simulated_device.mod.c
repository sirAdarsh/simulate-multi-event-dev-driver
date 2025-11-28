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
	{ 0x003b23f9, "single_open" },
	{ 0x4ff89cd9, "pcpu_hot" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xf296206e, "nr_cpu_ids" },
	{ 0xb5c51982, "__cpu_online_mask" },
	{ 0x86632fd6, "_find_next_bit" },
	{ 0xf2c4f3f1, "seq_printf" },
	{ 0xb5c51982, "__cpu_possible_mask" },
	{ 0xdf4bee3d, "alloc_workqueue" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xefd5d5d8, "cdev_init" },
	{ 0x4c1dbbd9, "cdev_add" },
	{ 0xea5ac1d9, "class_create" },
	{ 0xf98f93a7, "device_create" },
	{ 0x82c6f73b, "proc_create" },
	{ 0x535f4f5f, "hrtimer_init" },
	{ 0x5fa07cc0, "hrtimer_start_range_ns" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0xbeb1d261, "destroy_workqueue" },
	{ 0x0c72f9ad, "cdev_del" },
	{ 0x14fcde53, "class_destroy" },
	{ 0x36a36ab1, "hrtimer_cancel" },
	{ 0xbeb1d261, "__flush_workqueue" },
	{ 0x33c78c8a, "remove_proc_entry" },
	{ 0x6fdeeff0, "device_destroy" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x494c552b, "_find_first_bit" },
	{ 0x2182515b, "__num_online_cpus" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0xfed1e3bc, "kmalloc_caches" },
	{ 0x70db3fe4, "__kmalloc_cache_noprof" },
	{ 0x49733ad6, "queue_work_on" },
	{ 0x5a844b26, "__x86_indirect_thunk_rax" },
	{ 0x49fc4616, "hrtimer_forward" },
	{ 0xe4de56b4, "__ubsan_handle_load_invalid_value" },
	{ 0xbd4e501f, "seq_read" },
	{ 0xfc8fa4ce, "seq_lseek" },
	{ 0xcb077514, "single_release" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x5ae9ee26, "__per_cpu_offset" },
	{ 0xe1e1f979, "_raw_spin_lock_irqsave" },
	{ 0x97acb853, "ktime_get" },
	{ 0x40a621c5, "scnprintf" },
	{ 0x81a1a811, "_raw_spin_unlock_irqrestore" },
	{ 0xe8213e80, "_printk" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xba157484, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x003b23f9,
	0x4ff89cd9,
	0x092a35a2,
	0xd272d446,
	0xf296206e,
	0xb5c51982,
	0x86632fd6,
	0xf2c4f3f1,
	0xb5c51982,
	0xdf4bee3d,
	0x9f222e1e,
	0xefd5d5d8,
	0x4c1dbbd9,
	0xea5ac1d9,
	0xf98f93a7,
	0x82c6f73b,
	0x535f4f5f,
	0x5fa07cc0,
	0x0bc5fb0d,
	0xbeb1d261,
	0x0c72f9ad,
	0x14fcde53,
	0x36a36ab1,
	0xbeb1d261,
	0x33c78c8a,
	0x6fdeeff0,
	0x092a35a2,
	0x494c552b,
	0x2182515b,
	0xbd03ed67,
	0xfed1e3bc,
	0x70db3fe4,
	0x49733ad6,
	0x5a844b26,
	0x49fc4616,
	0xe4de56b4,
	0xbd4e501f,
	0xfc8fa4ce,
	0xcb077514,
	0xd272d446,
	0xd272d446,
	0x5ae9ee26,
	0xe1e1f979,
	0x97acb853,
	0x40a621c5,
	0x81a1a811,
	0xe8213e80,
	0xcb8b6ec6,
	0x90a48d82,
	0xba157484,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"single_open\0"
	"pcpu_hot\0"
	"_copy_to_user\0"
	"__stack_chk_fail\0"
	"nr_cpu_ids\0"
	"__cpu_online_mask\0"
	"_find_next_bit\0"
	"seq_printf\0"
	"__cpu_possible_mask\0"
	"alloc_workqueue\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"class_create\0"
	"device_create\0"
	"proc_create\0"
	"hrtimer_init\0"
	"hrtimer_start_range_ns\0"
	"unregister_chrdev_region\0"
	"destroy_workqueue\0"
	"cdev_del\0"
	"class_destroy\0"
	"hrtimer_cancel\0"
	"__flush_workqueue\0"
	"remove_proc_entry\0"
	"device_destroy\0"
	"_copy_from_user\0"
	"_find_first_bit\0"
	"__num_online_cpus\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"queue_work_on\0"
	"__x86_indirect_thunk_rax\0"
	"hrtimer_forward\0"
	"__ubsan_handle_load_invalid_value\0"
	"seq_read\0"
	"seq_lseek\0"
	"single_release\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"__per_cpu_offset\0"
	"_raw_spin_lock_irqsave\0"
	"ktime_get\0"
	"scnprintf\0"
	"_raw_spin_unlock_irqrestore\0"
	"_printk\0"
	"kfree\0"
	"__ubsan_handle_out_of_bounds\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CF5451DE16C8D62135C5CCF");
