#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kdev_t.h>
#include <linux/string.h>
#include <linux/videodev.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>
#include <asm/irq.h>
#include <asm/page.h>
#include <asm/io.h>
#include <asm/dma-mapping.h>
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
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/videodev.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kdev_t.h>
#include <linux/serial.h>
#include <asm/serial.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/serial_core.h>
#include <linux/serial.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/hardware.h>
#include <asm/serial.h>
#include <asm/arch/hardware.h>
#include <asm/arch/irqs.h>

#include <asm/arch/psc.h>
#include <linux/clk.h>
#include <asm/arch/clock.h>

#include <asm/io.h>
#include <asm/byteorder.h>
#include "incserial.h"


#define dprintk(x...)

static unsigned int simcard_major = 0;
#if (USE_UDEV==1)
static struct class *davinci_simcard_class;
#endif // USE_UDEV


unsigned char valuecount = 0;
static unsigned char value[256];
unsigned int pollflag = 0;


static int simcard_ioctl(struct inode *inode, struct file *filp,
	    unsigned int cmd, unsigned long args);
static int simcard_open(struct inode *inode, struct file *filp);
static int simcard_release(struct inode *inode, struct file *filp);
static unsigned int simcard_poll(struct file *filp, poll_table *wait);

typedef struct _davinci_uart_struct
{
    unsigned int TBR;           //0x00
    unsigned int IER;           //0x04
    unsigned int IIR;           //0x08
    unsigned int LCR;           //0x0C
    unsigned int MCR;           //0x10
    unsigned int LSR;           //0x14
    unsigned int reserve1;      //0x18
    unsigned int reserve2;      //0x1C
    unsigned int DLL;           //0x20
    unsigned int DLH;           //0x24
    unsigned int PID1;          //0x28
    unsigned int PID2;          //0x2C
    unsigned int PWREMU_MGMT;   //0x30
}davinci_uart_struct;

typedef struct _davinci_simcard_private
{
    wait_queue_head_t sendqueue;
    wait_queue_head_t rcvqueue;
}davinci_simcard_private;

davinci_simcard_private privatequeue;
#define SEND_TIMEOUT	100
#define RCV_TIMEOUT		1000

static	davinci_uart_struct * ptrglobaluartstruct = NULL;

static struct file_operations simcard_fxns = {
ioctl:   simcard_ioctl,
	 open:    simcard_open,
	 poll:	 simcard_poll,		 
	 release: simcard_release
};
static int simcard_open(struct inode *inode, struct file *filp)
{
    unsigned char value = 0x00;
    if((ptrglobaluartstruct->LSR & 0x01))
    {
	value =  ptrglobaluartstruct->TBR;
    }
    return 0;
}   

static unsigned int simcard_poll(struct file *filp, poll_table *wait)
{
    unsigned int mask = 0;

    poll_wait(filp, &privatequeue.rcvqueue, wait);

    if ( pollflag == 1 )
    {
	mask = POLLIN;
	pollflag = 0;
    }
    else
    {
	mask = 0x00;
    }
    return mask;
}
static int simcard_release(struct inode *inode, struct file *filp)
{
    return 0;
}

#define PIN_MUX_1_ADDR      (0x01C4000C)
#define PIN_MUX_4_ADDR      (0x01C40010)
SIMCARD_SEND sendbuf;
unsigned int sendindex = 0;

SIMCARD_SEND rcvbuf;
unsigned int rcvindex = 0;
unsigned int sendflag = 0;
static int simcard_ioctl(struct inode *inode, struct file *filp,
	    unsigned int cmd, unsigned long arg)
{

    printk("FILE: %s, LINE: %d........\r\n", __FILE__, __LINE__);
    switch( cmd )
    {
	case SIMCARD_CMD_SEND:
	    {		
		int ii = 0;	
		printk("FILE: %s, LINE: %d........\r\n", __FILE__, __LINE__);
		memset((void*)&sendbuf, 0, sizeof(SIMCARD_SEND));
		sendindex = 1;
		if (copy_from_user((void *)&sendbuf, (const void *)arg, sizeof(sendbuf))  )
		{
		    dprintk("KERNEL: copy_from_user failed, FILE: %s, LINE: %d.\n", __FILE__, __LINE__);
		    return -EINVAL;
		}
		for ( ii = 0; ii < sendbuf.sendlen; ii++ )
		{
		    ptrglobaluartstruct->TBR = sendbuf.sendbuf[ii];	
		}
		printk("FILE: %s, LINE: %d........sendbuf.sendlen = %d.value = 0x%x.\r\n", __FILE__, __LINE__, sendbuf.sendlen, sendbuf.sendbuf[ii-1]);
		return 0;
	    }
	    break;
	case SIMCARD_CMD_ENABLE:
	    *(unsigned int *)io_p2v(PIN_MUX_1_ADDR) |= 0x0004;				//ʹ��UART2����
	    break;
	case SIMCARD_CMD_DISABLE:
	    *(unsigned int *)io_p2v(PIN_MUX_1_ADDR) &= ~0x0004;				//�ر�UART2����
	    break;	
	case SIMCARD_CMD_GET:
	    {
		memset((void *)&rcvbuf, 0, sizeof(rcvbuf));

		if (copy_from_user((void *)&rcvbuf, (void *)arg, sizeof(rcvbuf))  )
		{
		    return -EINVAL;
		}
		memcpy(rcvbuf.sendbuf, value, valuecount);
		rcvbuf.sendlen = valuecount;
		if (copy_to_user((void *)arg, (void *)&rcvbuf, sizeof(rcvbuf))  )
		{                           
		    return -EINVAL;                                                     
		} 
		valuecount = 0;
		memset(value, 0, 256);	
		return 0;
	    }
	    break;	
	case SIMCARD_BAUD_SET:
	    {
		unsigned int clock_divisor = 0x00;
		switch( arg )
		{
		    case 9600:
			clock_divisor = (27000000 / 16 / 9600);
			break;
		    case 19200:
			clock_divisor = (27000000 / 16 / 19200);
			break;
		    case 38400:
			clock_divisor = (27000000 / 16 / 38400);
			break;
		    case 57600:
			clock_divisor = (27000000 / 16 / 57600);
			break;
		    case 115200:
			clock_divisor = (27000000 / 16 / 115200);
			break;
		    default:
			return -EINVAL;
			break;	
		}
		ptrglobaluartstruct->DLL = clock_divisor & 0xff;
		ptrglobaluartstruct->DLH = (clock_divisor >> 8) & 0xff;
	    }
	    break;
    }
    return 0;
}

unsigned int fixedrate = 24000000;
struct clk uart_clk= { 
    .name = "UART1",
    .rate = &fixedrate,
    .lpsc = DAVINCI_DM365_LPSC_UART1,
};

void clk_setenable(void)
{
    clk_disable(&uart_clk);
    clk_enable(&uart_clk);
}

void valuecheck(void)
{
    unsigned char valueread = 0x00;
    if ( ptrglobaluartstruct->LSR & 0x01 )
    {
	valueread = ptrglobaluartstruct->TBR;
	printk("in valuecheck valueread = 0x%x.\n", valueread);
    }

}

int simcard_serialinit(void)
{
    int clock_divisor = 0;
    unsigned char valueread = 0x00;
    ptrglobaluartstruct = ( davinci_uart_struct *)io_p2v(0x01D06000);
    ptrglobaluartstruct->IER = 0x0;
    ptrglobaluartstruct->LCR = (0x80) | (0x03);
    //clock_divisor = (24000000 / 16 / 9600);
    clock_divisor = (24000000 / 3.5 / 9600);
    //clock_divisor = (24000000 / 4 / 9600);
    //clock_divisor = (24000000 / 33600);
    ptrglobaluartstruct->DLL = clock_divisor & 0xff;
    ptrglobaluartstruct->DLH = (clock_divisor >> 8) & 0xff;
    ptrglobaluartstruct->LCR = (0x03);
    ptrglobaluartstruct->MCR = 0x01 | 0x02;
    ptrglobaluartstruct->IIR = 0x07 ;
    ptrglobaluartstruct->PWREMU_MGMT = 0xE003;
    ptrglobaluartstruct->IER = 0x0;		//enable interrupt
    ptrglobaluartstruct->IER = 0x1;		//enable interrupt

    if ( ptrglobaluartstruct->LSR & 0x01 )
    {
	valueread = ptrglobaluartstruct->TBR;
	printk("valueread = 0x%x.\n", valueread);
    }
    return 0;
}

void xorcheck(unsigned char *pbuf, unsigned int len)
{
    unsigned char value = 0x00;
    unsigned int i = 0;

    for ( i = 0; i < len - 1; i++ )
    {
	value = value ^ (*(pbuf + i));
    }
    if ( value != (*(pbuf + i)))
    {
	printk("check sum error.\n");
    }
    else
    {
	//	printk("ok. sum = 0x%x.\n", value);
    }
}

//static irqreturn_t simcard_isr(int irq, void *dev_id, struct pt_regs *regs)
static irqreturn_t simcard_isr(int irq, void *dev_id)
{
    unsigned int iirstatus = 0x00;
    int count = 0;

    printk("simcard_isr Enter\n");

    printk("FILE: %s LINE: %d....\r\n", __FILE__, __LINE__);
    iirstatus = ptrglobaluartstruct->IIR;
    //ptrglobaluartstruct->IIR |=  0x04;
    //printk("FILE: %s LINE: %d....iirstatus = 0x%x\r\n", __FILE__, __LINE__, iirstatus);
    valuecount = 0;
    if ( iirstatus & 0x04 )
    {
	value[valuecount] =  ptrglobaluartstruct->TBR;
	printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
	valuecount++;
	value[valuecount] =  ptrglobaluartstruct->TBR;
	printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
	valuecount++;
	value[valuecount] =  ptrglobaluartstruct->TBR;
	printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
	valuecount++;
	value[valuecount] =  ptrglobaluartstruct->TBR;
	printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
	valuecount++;
	value[valuecount] =  ptrglobaluartstruct->TBR;
	printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
	pollflag = 1;
	wake_up_interruptible(&privatequeue.rcvqueue);

#if 0
	valuecount++;
	while(1)
	{
	    udelay(10);

	    if ( ptrglobaluartstruct->LSR & 0x01 )
	    {
		value[valuecount] =  ptrglobaluartstruct->TBR;
		printk("FILE: %s LINE: %d....value = 0x%x\r\n", __FILE__, __LINE__, value[valuecount]);
		valuecount++;
	    }
	    count++;
	    if ( count > 10000 )
	    {
		ptrglobaluartstruct->IIR = 0x01 | 0x02 | 0x04;
		printk("wakeup...valuecount = %d.\n",valuecount);
		pollflag = 1;
		wake_up_interruptible(&privatequeue.rcvqueue);
		xorcheck(value, valuecount);
		break;
	    }
	}
#endif
    }
    printk("FILE: %s LINE: %d....\r\n", __FILE__, __LINE__);
    return IRQ_RETVAL(1);
}

int __init davinci_simcard_init(void)
{
    simcard_major = register_chrdev(0, "485", &simcard_fxns);
    if (simcard_major < 0)
    {
	printk("Failed to allocate major number.\n");
	return -ENODEV;
    }

#if (USE_UDEV==1)
    davinci_simcard_class = class_create(THIS_MODULE, "485");
    if (IS_ERR(davinci_simcard_class))
    {
	printk("Error creating davinci simcard device class.\n");
	return -EIO;
    }

    class_device_create(davinci_simcard_class, NULL, MKDEV(simcard_major, 0), NULL, "485");
#endif // USE_UDEV

    request_irq(IRQ_UARTINT1, simcard_isr, SA_INTERRUPT, "485_interrupt", NULL);
    clk_setenable();
    simcard_serialinit();

    init_waitqueue_head(&privatequeue.sendqueue);
    init_waitqueue_head(&privatequeue.rcvqueue);

    printk("Get PINMUX3 value : 0x%x.\r\n ", *(unsigned int *)io_p2v(PIN_MUX_1_ADDR) );

    {
	unsigned int tempvalue = *(unsigned int *)io_p2v(PIN_MUX_1_ADDR);
	tempvalue |= 3<<29;
	*(unsigned int *)io_p2v(PIN_MUX_1_ADDR)= tempvalue;
    }
    {
	unsigned int tempvalue = *(unsigned int *)io_p2v(PIN_MUX_4_ADDR);
	tempvalue |= 3<<14;
	*(unsigned int *)io_p2v(PIN_MUX_4_ADDR)= tempvalue;
    }

    printk("Get PINMUX4 value : 0x%x.\r\n ", *(unsigned int *)io_p2v(PIN_MUX_4_ADDR) );
    printk("PLLC1SYSCLK4 value : 0x%x.\r\n ", *(unsigned int *)io_p2v(0x01C40960) );

    //*(unsigned int *)io_p2v(PIN_MUX_1_ADDR)|= 0x0004;

    udelay(1000);
    udelay(1000);
    udelay(1000);
    udelay(1000);
    udelay(1000);
    udelay(1000);
    valuecheck();
    return 0;

}
void __exit davinci_simcard_exit(void)
{

#if (USE_UDEV==1)
    class_destroy(davinci_simcard_class);
#endif // USE_UDEV
    printk("Unregistering character device simcard\n");
    unregister_chrdev(simcard_major, "davinci_simcard");
    free_irq(IRQ_UARTINT1, NULL);
}

MODULE_LICENSE("GPL v2");
module_init(davinci_simcard_init);
module_exit(davinci_simcard_exit);

