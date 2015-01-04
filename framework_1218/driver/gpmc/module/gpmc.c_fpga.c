
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>

#include <plat/dma.h>
#include <plat/gpmc.h>
#include <plat/gpio.h>

#include "asm/uaccess.h"
#include "asm/io.h"
#include "asm/atomic.h"


#define USER_BUFF_SIZE 128

struct fpga_dev {
		dev_t devt;
		struct cdev cdev;
		struct semaphore sem;
		struct class *class;
		char *user_buff;
};

static struct fpga_dev fpga_dev;

unsigned long mem_base;
static void __iomem *fpga_base;

/* GPMC register offsets */
#define GPMC_REVISION 0x00
#define GPMC_SYSCONFIG 0x10
#define GPMC_SYSSTATUS 0x14
#define GPMC_IRQSTATUS 0x18
#define GPMC_IRQENABLE 0x1c
#define GPMC_TIMEOUT_CONTROL 0x40
#define GPMC_ERR_ADDRESS 0x44
#define GPMC_ERR_TYPE 0x48
#define GPMC_CONFIG 0x50
#define GPMC_STATUS 0x54
#define GPMC_PREFETCH_CONFIG1 0x1e0
#define GPMC_PREFETCH_CONFIG2 0x1e4
#define GPMC_PREFETCH_CONTROL 0x1ec
#define GPMC_PREFETCH_STATUS 0x1f0
#define GPMC_ECC_CONFIG 0x1f4
#define GPMC_ECC_CONTROL 0x1f8
#define GPMC_ECC_SIZE_CONFIG 0x1fc
#define GPMC_ECC1_RESULT 0x200
#define GPMC_ECC_BCH_RESULT_0 0x240

#define GPMC_CS 3

#define STNOR_GPMC_CONFIG1 0x28601001
#define STNOR_GPMC_CONFIG2 0x00011001
#define STNOR_GPMC_CONFIG3 0x00020201
#define STNOR_GPMC_CONFIG4 0x08031003
#define STNOR_GPMC_CONFIG5 0x000f1111
#define STNOR_GPMC_CONFIG6 0x0f030080

static const u32 gpmc_nor[7] = {
		STNOR_GPMC_CONFIG1,
		STNOR_GPMC_CONFIG2,
		STNOR_GPMC_CONFIG3,
		STNOR_GPMC_CONFIG4,
		STNOR_GPMC_CONFIG5,
		STNOR_GPMC_CONFIG6,
		0
};

static ssize_t fpga_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
		ssize_t status;
		size_t  len = USER_BUFF_SIZE - 1;
		int     i, tmp;

		if (count == 0)
				return 0;

		if (down_interruptible(&fpga_dev.sem))
				return -ERESTARTSYS;

		if (len > count)
				len = count;

		memset(fpga_dev.user_buff, 0, USER_BUFF_SIZE);

		if (copy_from_user(fpga_dev.user_buff, buff, len)) {
				status = -EFAULT;
				goto fpga_write_done;
		}

		/* do something with the user data */

		printk("fpga_write \n");
		for (i = 0; i < len; i = i + 2) {
				tmp = fpga_dev.user_buff[i] | (fpga_dev.user_buff[i+1]) << 8; 
				writew(tmp, fpga_base + i);
		}

fpga_write_done:

		up(&fpga_dev.sem);

		return status;
}

static ssize_t fpga_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
#if 0
		ssize_t status;
		size_t len;
		// int i,tmp;

		/* 
		   Generic user progs like cat will continue calling until we 
		   return zero. So if *offp != 0, we know this is at least the
		   second call.
		 */
		if (*offp > 0)
				return 0;

		if (down_interruptible(&fpga_dev.sem)) 
				return -ERESTARTSYS;

		strcpy(fpga_dev.user_buff, "fpga driver data goes here\n");

		len = strlen(fpga_dev.user_buff);

		if (len > count)
				len = count;

		if (copy_to_user(buff, fpga_dev.user_buff, len)) {
				status = -EFAULT;
				goto fpga_read_done;
		}

fpga_read_done:

		up(&fpga_dev.sem);

		return status;
#endif
		return 0;
}

static int fpga_open(struct inode *inode, struct file *filp)
{ 
		int status = 0;

		if (down_interruptible(&fpga_dev.sem)) 
				return -ERESTARTSYS;

		if (!fpga_dev.user_buff) {
				fpga_dev.user_buff = kmalloc(USER_BUFF_SIZE, GFP_KERNEL);

				if (!fpga_dev.user_buff) {
						printk(KERN_ALERT "fpga_open: user_buff alloc failed\n");
						status = -ENOMEM;
				}
		}



		printk("OPEN DEV OK!\n");


		up(&fpga_dev.sem);

		return status;
}

static const struct file_operations fpga_fops = {
		.owner = THIS_MODULE,
		.open  = fpga_open, 
		.read  = fpga_read,
		.write = fpga_write,
};

static int __init fpga_init_cdev(void)
{
		int error;
		u32 val;
		void __iomem *gpmc_base;

		fpga_dev.devt = MKDEV(0, 0);

		error = alloc_chrdev_region(&fpga_dev.devt, 0, 1, "fpga");
		if (error) {
				printk(KERN_ALERT "alloc_chrdev_region() failed: %d\n", error);
				return error;
		}

		cdev_init(&fpga_dev.cdev, &fpga_fops);
		fpga_dev.cdev.owner = THIS_MODULE;

		error = cdev_add(&fpga_dev.cdev, fpga_dev.devt, 1);
		if (error) {
				printk(KERN_ALERT "cdev_add() failed: %d\n", error);
				unregister_chrdev_region(fpga_dev.devt, 1);
				return error;
		}

		printk("Getting Chip Select\n");

		/* gpmc_base=ioremap(0x6E000000,SZ_4K);
		   val=__raw_readl(gpmc_base + 0x50);
		   printk("GPMC_CONFIG:%x \n",val);
		   __raw_writel(0x2,gpmc_base + 0x50); */

		val = 0xf64;
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG7, val);
		//val = gpmc_read_reg(GPMC_REVISION);
		//printk("GPMC revision %d.%d\n", (val >> 4) & 0x0f, val & 0x0f);
		//gpmc_write_reg(GPMC_IRQENABLE, 0);
		//gpmc_write_reg(GPMC_TIMEOUT_CONTROL, 0);

		/*gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG1, gpmc_nor[0]);
		  gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG2, gpmc_nor[1]);
		  gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG3, gpmc_nor[2]);
		  gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG4, gpmc_nor[3]);
		  gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG5, gpmc_nor[4]);
		  gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG6, gpmc_nor[5]);*/

		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG1, 0x1000);
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG2, 0x001e1e00);
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG3, 0x00080300);
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG4, 0x1c091c09);
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG5, 0x04181f1f);
		gpmc_cs_write_reg(GPMC_CS, GPMC_CS_CONFIG6, 0x00000FcF);

		val = gpmc_cs_read_reg(GPMC_CS, GPMC_CS_CONFIG7);
		printk("GPMC_CS3_CONFIG7 value 0x%x\n", val);

		/*if (gpmc_cs_request(GPMC_CS, SZ_4K, (unsigned long *)&mem_base) < 0) {
		  printk("Failed request for GPMC mem for usrp_e\n");
		  return -1;
		  }
		  printk("Got CS3, address = %lx\n", mem_base);
		 */

		mem_base=0x24000000;

		if (!request_mem_region(mem_base, SZ_4K, "mem_fpga")) {
				printk(KERN_ERR "Request_mem_region failed.\n");
				gpmc_cs_free(GPMC_CS);
				return -1;
		}

		fpga_base = ioremap(mem_base, SZ_4K); 


		printk("membase:%x fpgabase:%x \n",mem_base,fpga_base);

		//iounmap(gpmc_base);
		int j=0;
		short tmp=0;
		while(j++<10000)     
		{  
				// sleep(1000);        

				writew(0xA5,fpga_base+0x3D0);
				tmp=readw (fpga_base + 0x3D0);
				printk("BIST:%4x\n",tmp);
		}
		return 0;
}

static int __init fpga_init_class(void)
{
		struct device *device;

		fpga_dev.class = class_create(THIS_MODULE, "fpga");

		if (IS_ERR(fpga_dev.class)) {
				printk(KERN_ALERT "class_create(fpga) failed\n");
				return PTR_ERR(fpga_dev.class);
		}

		device = device_create(fpga_dev.class, NULL, fpga_dev.devt, NULL, "fpga");

		if (IS_ERR(device)) {
				class_destroy(fpga_dev.class);
				return PTR_ERR(device);
		}

		return 0;
}

static int __init fpga_init(void)
{
		printk(KERN_INFO "fpga_init()\n");

		memset(&fpga_dev, 0, sizeof(struct fpga_dev));

		sema_init(&fpga_dev.sem, 1);

		if (fpga_init_cdev())
				goto init_fail_1;

		if (fpga_init_class())
				goto init_fail_2;

		return 0;

init_fail_2:
		cdev_del(&fpga_dev.cdev);
		unregister_chrdev_region(fpga_dev.devt, 1);

init_fail_1:

		return -1;
}

static void __exit fpga_exit(void)
{
		printk(KERN_INFO "fpga_exit()\n");

		device_destroy(fpga_dev.class, fpga_dev.devt);
		class_destroy(fpga_dev.class);

		cdev_del(&fpga_dev.cdev);
		unregister_chrdev_region(fpga_dev.devt, 1);

		release_mem_region(mem_base, SZ_2K);
		//gpmc_cs_free(GPMC_CS);
		iounmap(fpga_base);

		if (fpga_dev.user_buff)
				kfree(fpga_dev.user_buff);
}

module_init(fpga_init);
module_exit(fpga_exit);

MODULE_AUTHOR("ysx");
MODULE_DESCRIPTION("gpmc fpga driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

