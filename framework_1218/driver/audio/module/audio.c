#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/seq_file.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <asm/cacheflush.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/io.h>



#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#warning *** not a warning *** Note: LINUX_VERSION_CODE >= 2.6.26

#define USE_CACHE_VOID_ARG
#undef USE_CLASS_DEVICE
#undef USE_CLASS_SIMPLE

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)

#warning *** not a warning *** Note: 2.6.26 > LINUX_VERSION_CODE >= 2.6.18

#define USE_CACHE_VOID_ARG
#define USE_CLASS_DEVICE
#undef USE_CLASS_SIMPLE

#else  /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) */

#warning *** not a warning *** Note: LINUX_VERSION_CODE < 2.6.18

#define USE_CLASS_SIMPLE
#undef USE_CLASS_DEVICE
#undef USE_CACHE_VOID_ARG

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) */

/*
 * Poor man's config params
 */

/*
 * NOCACHE means physical memory block is ioremap()'ed as noncached
 */
#define NOCACHE

/*
 * CHECK_FOR_ALLOCATED_BUFFER means ensure that the passed addr/size block
 * is actually an allocated, CMEM-defined buffer.
 */
//#define CHECK_FOR_ALLOCATED_BUFFER

#define HEAP_ALIGN PAGE_SIZE


#ifdef __DEBUG
#define __D(fmt, args...) printk(KERN_DEBUG "CMEMK Debug: " fmt, ## args)
#else
#define __D(fmt, args...)
#endif

#define __E(fmt, args...) printk(KERN_ERR "CMEMK Error: " fmt, ## args)

#define MAXTYPE(T) ((T) (((T)1 << ((sizeof(T) * 8) - 1) ^ ((T) -1))))

/*
 * Change here for supporting more than 2 blocks.  Also change all
 * NBLOCKS-based arrays to have NBLOCKS-worth of initialization values.
 */
#define NBLOCKS 2

static int gbshm_major;
static struct proc_dir_entry *gbshm_proc_entry;
static atomic_t reference_count = ATOMIC_INIT(0);

static unsigned int USE_UDEV = 1;

#if (USE_UDEV==1)
#ifdef USE_CLASS_SIMPLE
static struct class_simple *gbshm_class;
#else
static struct class *gbshm_class;
#endif
#endif // USE_UDEV

static DECLARE_MUTEX(cmem_mutex);

/* Forward declaration of system calls */
static int ioctl(struct inode *inode, struct file *filp,
	    unsigned int cmd, unsigned long args);
static int mmap(struct file *filp, struct vm_area_struct *vma);
static int open(struct inode *inode, struct file *filp);
static int release(struct inode *inode, struct file *filp);

static struct file_operations gbshm_fxns = {
ioctl:   ioctl,
	 mmap:    mmap,
	 open:    open,
	 release: release
};
static int cmem_proc_open(struct inode *inode, struct file *file);

static struct file_operations gbshm_proc_ops = {
    .owner = THIS_MODULE,
    .open = cmem_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};

static int cmem_proc_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int ioctl(struct inode *inode, struct file *filp,
	    unsigned int cmd, unsigned long args)
{
    return 0;
}

static int mmap(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long physp;

    __D("mmap: vma->vm_start     = %#lx\n", vma->vm_start);
    __D("mmap: vma->vm_pgoff     = %#lx\n", vma->vm_pgoff);
    __D("mmap: vma->vm_end       = %#lx\n", vma->vm_end);
    __D("mmap: size              = %#lx\n", vma->vm_end - vma->vm_start);

    physp = vma->vm_pgoff << PAGE_SHIFT;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    vma->vm_flags |= VM_RESERVED | VM_IO;
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
                        vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
        __E("set_noncached: failed remap_pfn_range\n");
        return -EAGAIN;
    }

    return 0;	
}

static int open(struct inode *inode, struct file *filp)
{
    __D("open: called.\n");

    atomic_inc(&reference_count);

    return 0;
}

static int release(struct inode *inode, struct file *filp)
{
    int last_close = 0;
    __D("close: called.\n");

    /* Force free all buffers owned by the 'current' process */

    if (atomic_dec_and_test(&reference_count)) {
	__D("close: all references closed, force freeing all busy buffers.\n");

	last_close = 1;
    }

    __D("close: returning\n");

    return 0;
}

int __init gbshm_init(void)
{
    int err = 0;

    /* cut-and-paste above as part of adding support for more than 2 blocks */

    gbshm_major = register_chrdev(0, "gbshm", &gbshm_fxns);

    if (gbshm_major < 0) {
	__E("Failed to allocate major number.\n");
	return -ENODEV;
    }

    __D("Allocated major number: %d\n", gbshm_major);

#if (USE_UDEV==1)
#ifdef USE_CLASS_SIMPLE
    gbshm_class = class_simple_create(THIS_MODULE, "gbshm");
#else
    gbshm_class = class_create(THIS_MODULE, "gbshm");
#endif
    if (IS_ERR(gbshm_class)) {
	__E("Error creating cmem device class.\n");
	err = -EIO;
	return err;
    }

#ifdef USE_CLASS_SIMPLE
    class_simple_device_add(gbshm_class, MKDEV(gbshm_major, 0), NULL, "gbshm");
#else
#ifdef USE_CLASS_DEVICE
    class_device_create(gbshm_class, NULL, MKDEV(gbshm_major, 0), NULL, "gbshm");
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
    device_create(gbshm_class, NULL, MKDEV(gbshm_major, 0), NULL, "gbshm");
#else
    device_create(gbshm_class, NULL, MKDEV(gbshm_major, 0), "gbshm");
#endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#endif // USE_CLASS_DEVICE
#endif // USE_CLASS_SIMPLE
#endif // USE_UDEV

    /* Create the /proc entry */

    printk(KERN_INFO "gbshm initialized\n");

    return 0;

}

void __exit gbshm_exit(void)
{
    __D("In gbshm_exit()\n");

    /* Remove the /proc entry */
    remove_proc_entry("gbshm", NULL);

#if (USE_UDEV==1)

#ifdef USE_CLASS_SIMPLE
    class_simple_device_remove(MKDEV(gbshm_major, 0));
    class_simple_destroy(gbshm_class);
#else
#ifdef USE_CLASS_DEVICE
    class_device_destroy(gbshm_class, MKDEV(gbshm_major, 0));
#else
    device_destroy(gbshm_class, MKDEV(gbshm_major, 0));
#endif // USE_CLASS_DEVICE
    class_destroy(gbshm_class);
#endif // USE_CLASS_SIMPLE
#endif // USE_UDEV

    __D("Unregistering character device mem\n");
    unregister_chrdev(gbshm_major, "gbshm");
    printk(KERN_INFO "gbshm unregistered\n");
}

MODULE_LICENSE("GPL");
module_init(gbshm_init);
module_exit(gbshm_exit);
