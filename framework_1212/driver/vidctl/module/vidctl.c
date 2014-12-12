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

typedef enum _enablemode
{
	ENABLE_PAL,
	ENABLE_NTSC
}enablemode;

typedef enum _enableresol
{
	ENABLE_RESOL_D1,
	ENABLE_RESOL_CIF,
	DISABLE_RSZ
}enableresol;


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
	volatile unsigned int *pReg = NULL;
	switch(cmd)
	{
//		case 1:		//enable and set register  D1
		case ENABLE_RESOL_D1:		//enable and set register  D1
			set_reg();
			if ( args == ENABLE_PAL )		//ENABLE_PAL   720*576
			{
				pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x28); //isif 0x28---287
				*pReg = 0x11f;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x14); //ipipe 0x14---287
				*pReg = 0x11f;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x14); //rsz 0x14---287
				*pReg = 0x11f;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x6c); //rsz 0x6c---575
				*pReg = 0x23f;
			}
			else if (args == ENABLE_NTSC )	//ENABLE_NTSC   720*480
			{
				pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x28); //isif 0x28---239
				*pReg = 0xef;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x14); //ipipe 0x14---239
				*pReg = 0xef;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x14); //rsz 0x14---239
				*pReg = 0xef;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x6c); //rsz 0x6c---479
				*pReg = 0x1df;
			}
			break;
		case ENABLE_RESOL_CIF:
			set_reg();
			if ( args == ENABLE_PAL )		//ENABLE_PAL   360*288
			{
				pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x28); //isif 0x28---287
				*pReg = 0x11f;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x14); //ipipe 0x14---287
				*pReg = 0x11f;		

				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x14); //isif 0x28---287
				*pReg = 0x11f;	//288
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x1C); //isif 0x28---287
				*pReg = 0x2CF;	//720
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x6C); //isif 0x28---287
				*pReg = 0x11f;	//288
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x70); //isif 0x28---287
				*pReg = 0x17f;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x7C); //isif 0x28---287
				*pReg = 0x100;	//1
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x90); //isif 0x28---287
				//*pReg = 0x20b;
				*pReg = 0x1E3;	//0.53
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC0); //isif 0x28---287
				*pReg = 0x180;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC8); //isif 0x28---287
				*pReg = 0x120;	//288
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xdC); //isif 0x28---287
				*pReg = 0x180;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xE4); //isif 0x28---287
				*pReg = 0x90;	//144
			}
			else if (args == ENABLE_NTSC )	//ENABLE_NTSC   360*240
			{
				pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x28); //isif 0x28---239
				*pReg = 0xef;
				pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x14); //ipipe 0x14---239
				*pReg = 0xef;

				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x14); //isif 0x28---287
				*pReg = 0xEf;	//287
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x1C); //isif 0x28---287
				*pReg = 0x2CF;	//720
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x6C); //isif 0x28---287
				*pReg = 0xEf;	//287
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x70); //isif 0x28---287
				*pReg = 0x17f;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x7C); //isif 0x28---287
				*pReg = 0x100;	//1
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x90); //isif 0x28---287
				//*pReg = 0x20b;
				*pReg = 0x1E3;	//0.53 384/720
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC0); //isif 0x28---287
				*pReg = 0x180;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC8); //isif 0x28---287
				*pReg = 0xF0;	//288
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xdC); //isif 0x28---287
				*pReg = 0x180;	//384
				pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xE4); //isif 0x28---287
				*pReg = 0x78;	//120			
			}

			pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x7C); // EDMACC.QUEPRI
			//*(pReg ) = 0x200;	
			printk("in set RSZ parameter 0x7C= 0x%x.\r\n", *(pReg));
			pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x90); // EDMACC.QUEPRI
			//*(pReg ) = 0x200;	
			break;
		case DISABLE_RSZ:		//disable rsz
//		case 3:		//disable rsz
			pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x00); //rsz 0x00---disable
			*pReg = 0x0;
			pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x58); //rsz 0x58---disbale
			*pReg = 0x0;
			break;
		default:
			break;
	}
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

void set_reg_isif(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x00); // EDMACC.QUEPRI
	*pReg = 0x01;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x004); // EDMACC.QUEPRI
	*pReg = 0x2084;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x01c); // EDMACC.QUEPRI
	*pReg = 0x59f;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x020); // EDMACC.QUEPRI
	*pReg = 0x1;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x024); // EDMACC.QUEPRI
	*pReg = 0x1;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x028); // EDMACC.QUEPRI
	*pReg = 0x11f;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x034); // EDMACC.QUEPRI
	*pReg = 0x2d;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x038); // EDMACC.QUEPRI
	*pReg = 0x249;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x078); // EDMACC.QUEPRI
	*pReg = 0x11f;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x094); // EDMACC.QUEPRI
	*pReg = 0x3;

	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x098); // EDMACC.QUEPRI
	*pReg = 0x812;


}


void printk_reg_isif_mod(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = (unsigned int *)IO_ADDRESS(0x1c71000); // EDMACC.QUEPRI

	for (i = 0; i < 40;i++)
	{
		printk("isif: 0x%x value 0x%x.\r\n", i*4 , *(pReg+i));
	}
}



//no used
void printk_reg_isif(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 114;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c71000+i*4); // EDMACC.QUEPRI
		//printk("isif: 0x%x value 0x%x.\r\n", i*4 , *(pReg+i));
		printk("isif: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}
void set_reg_isif_new(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = ( unsigned int *)IO_ADDRESS(0x1c71000+0x04); // EDMACC.QUEPRI
	*(pReg ) = 0xa084;
	printk("in set_reg_isif_new............. set value 0x04 = 0x%x.\r\n", *(pReg)); 
	pReg = (unsigned int *)IO_ADDRESS(0x1c71000+0x1C); // EDMACC.QUEPRI
	*(pReg ) = 0x59f;
	printk("in set_reg_isif_new............. set value 0x1C = 0x%x.\r\n", *(pReg)); 
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x20); // EDMACC.QUEPRI
	*(pReg ) = 0x01;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x24); // EDMACC.QUEPRI
	*(pReg ) = 0x01;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x28); // EDMACC.QUEPRI
	*(pReg ) = 0x011f;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x34); // EDMACC.QUEPRI
	*(pReg ) = 0x2D;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x38); // EDMACC.QUEPRI
	*(pReg ) = 0x249;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x70); // EDMACC.QUEPRI
	*(pReg ) = 0x011f;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x84); // EDMACC.QUEPRI
	*(pReg ) = 0x03;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x88); // EDMACC.QUEPRI
	*(pReg ) = 0x812;
	pReg = (volatile unsigned int *)IO_ADDRESS(0x1c71000+0x00); // EDMACC.QUEPRI
	*(pReg ) = 0x1;

	printk("in set_reg_isif_new............. set value 0x04 = 0x%x.\r\n", *(pReg+0x0)); 

}

//in rsz_run set
void printk_reg_ipipeif(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 23;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c71200+i*4); // EDMACC.QUEPRI
		printk("ipipeif: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}
void set_reg_ipipeif(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = (unsigned int *)IO_ADDRESS(0x1c71200); // EDMACC.QUEPRI
	*(pReg + 0) = 0x00;
	pReg = (unsigned int *)IO_ADDRESS(0x1c71200+0x04); // EDMACC.QUEPRI
	*(pReg + 0) = 0x2000;
	pReg = (unsigned int *)IO_ADDRESS(0x1c71200+0x30); // EDMACC.QUEPRI
	*(pReg + 0) = 0x48;

}

//in drv_init
void printk_reg_ipipe(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 256;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c70800+i*4); // EDMACC.QUEPRI
		printk("ipipe: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}
void set_reg_ipipe(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x08); // EDMACC.QUEPRI
	*(pReg  ) = 0x03;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x14); // EDMACC.QUEPRI
	*(pReg ) = 0x11f;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x1C); // EDMACC.QUEPRI
	*(pReg ) = 0x2Cf;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x2C); // EDMACC.QUEPRI
	*(pReg ) = 0x0e;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70800+0x00); // EDMACC.QUEPRI
	*(pReg ) = 0x01;
}

//in drv_init and rsz_run
void printk_reg_rsz(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 94;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c70400+i*4); // EDMACC.QUEPRI
		printk("rsz: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}
void set_reg_rsz(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x08); // EDMACC.QUEPRI
	*(pReg ) = 0x0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x14); // EDMACC.QUEPRI
	*(pReg ) = 0x11f;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x1C); // EDMACC.QUEPRI
	*(pReg ) = 0x2Cf;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x20); // EDMACC.QUEPRI
	*(pReg ) = 0x0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x64); // EDMACC.QUEPRI
	*(pReg ) = 0x0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x6C); // EDMACC.QUEPRI
	*(pReg ) = 0x23f;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x70); // EDMACC.QUEPRI
	*(pReg ) = 0x2CF;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x74); // EDMACC.QUEPRI
	*(pReg ) = 0x0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x7C); // EDMACC.QUEPRI
	*(pReg ) = 0x80;	

	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC0); // EDMACC.QUEPRI
	*(pReg ) = 0x2e0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xC8); // EDMACC.QUEPRI
	*(pReg ) = 0x240;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xDC); // EDMACC.QUEPRI
	*(pReg ) = 0x2E0;	
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0xE4); // EDMACC.QUEPRI
	*(pReg ) = 0x120;	

	//2011-05-24
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x0); // EDMACC.QUEPRI
	printk("in set RSZ parameter 0x0= 0x%x.\r\n", *(pReg));

	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x58); // EDMACC.QUEPRI
	printk("in set RSZ parameter 0x58= 0x%x.\r\n", *(pReg));

	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x7C); // EDMACC.QUEPRI
//	*(pReg ) = 0x200;	
	printk("in set RSZ parameter 0x7C= 0x%x.\r\n", *(pReg));
	pReg = (unsigned int *)IO_ADDRESS(0x1c70400+0x90); // EDMACC.QUEPRI
//	*(pReg ) = 0x200;	
	printk("in set RSZ parameter 0x90= 0x%x.\r\n", *(pReg));

//	printk_reg_rsz();

}

//no used
void printk_reg_h3a(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 31;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c71400+i*4); // EDMACC.QUEPRI
		printk("h3a: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}

//in drv_init
void printk_reg_isp(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 12 ;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c70000+i*4); // EDMACC.QUEPRI
		printk("isp: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}

//no used
void printk_reg_vpss(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	for (i = 0; i < 1 ;i++)
	{
		pReg = (unsigned int *)IO_ADDRESS(0x1c70200+i*4); // EDMACC.QUEPRI
		printk("vpss: 0x%x value 0x%x.\r\n", i*4 , *(pReg));
	}
}
void set_reg_vpss(void)
{
	volatile unsigned int *pReg = NULL;
	unsigned int i = 0;

	pReg = (unsigned int *)IO_ADDRESS(0x1c70200); // EDMACC.QUEPRI
	*pReg = 0x01;
}

void printk_reg(void)
{
	printk_reg_isif();
	printk_reg_ipipeif();
	printk_reg_ipipe();
	printk_reg_rsz();
	printk_reg_h3a();
	printk_reg_isp();
	printk_reg_vpss();
	//set_reg_isif();
	//printk_reg_isif_mod();
}
void set_reg(void)
{
	set_reg_isif_new();
	set_reg_ipipeif();
	set_reg_ipipe();
	set_reg_rsz();
	set_reg_vpss();
	{
		volatile unsigned int *pReg = NULL;
		pReg = (unsigned int *)IO_ADDRESS(0x1c71e00+0x4); // EDMACC.QUEPRI
		*pReg = 0x2000;

		pReg = (unsigned int *)IO_ADDRESS(0x1c71e00+0x64); // EDMACC.QUEPRI
		*pReg = 0x0800;

		pReg = (unsigned int *)IO_ADDRESS(0x1c71e00+0x68); // EDMACC.QUEPRI
		*pReg = 0x0001;

		pReg = (unsigned int *)IO_ADDRESS(0x1c40004); // EDMACC.QUEPRI
		*pReg &= ~0x00400000;
	}
}

int __init gbshm_init(void)
{
	int err = 0;

	/* cut-and-paste above as part of adding support for more than 2 blocks */

	gbshm_major = register_chrdev(0, "vidctl", &gbshm_fxns);

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

	printk(KERN_INFO "vidctl initialized\n");


	//set_reg();
	//printk_reg();
	//tvp5150_set_input_mux(0);

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
	printk(KERN_INFO "vidctl unregistered\n");
}

MODULE_LICENSE("GPL");
module_init(gbshm_init);
module_exit(gbshm_exit);
