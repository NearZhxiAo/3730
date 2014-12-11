/*
 * TL2796 LCD Panel Driver for the Samsung Universal board
 *
 * Copyright (c) 2008 Samsung Electronics
 * Author: InKi Dae  <inki.dae@samsung.com>
 *
 * Derived from drivers/video/omap/lcd-apollon.c
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <linux/wait.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <asm/arch/mux.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/cdev.h>

#include "davincifb_r61505w.h"


#define SLEEPMSEC		0x1000
#define ENDDEF			0x2000
#define	DEFMASK			0xFF00
#define COMMAND_ONLY		0x0000
#define DATA_ONLY		0xFF

#define SPI_GPIO_SCL		36
#define SPI_GPIO_SDI		37
//#define SPI_GPIO_SDO		38
#define SPI_GPIO_CS        		35

/*backlight gpio*/
#define KTD104_BL_GPIO_CTRL		23

static struct spi_device *g_spi;
#define PULSE_NUM(set,cur)	(set + 32 - cur)

typedef enum{
	BL_LEVEL0,
	BL_LEVEL1,
	BL_LEVEL2,
	BL_LEVEL3,
	BL_LEVEL4,		
	BL_LEVEL5,
	BL_LEVEL_NUM		
}bl_level;

typedef struct {
	bl_level level;
	unsigned char data;
}ktd104_bl_data;

typedef struct {
	unsigned char data;	
	float bl_current;
}ktd104_bl_current;

ktd104_bl_data bl_data[] = {
		{BL_LEVEL0,0},
		{BL_LEVEL1,16},
		{BL_LEVEL2,20},
		{BL_LEVEL3,24},
		{BL_LEVEL4,28},
		{BL_LEVEL5,32}
};

ktd104_bl_current bl_current[] = {
		{1,26.75},{2,25.50},{3,24.25},{4,23.00},
		{5,21.75},{6,20.50},{7,19.25},{8,18.00},
		{9,16.75},{10,15.50},{11,14.25},{12,13.00},
		{13,211.75},{14,10.05},{15,9.25},{16,8.00},		
		{17,7.50},{18,7.00},{19,6.50},{20,6.00},	
		{21,5.50},{22,5.00},{23,4.50},{24,4.00},
		{25,3.50},{26,3.00},{27,2.50},{28,2.00},
		{29,1.50},{30,1.00},{31,0.50},{32,0.25}
};
static ktd104_bl_data cur_bl_data = {0};

//const unsigned short SEQ_SETTING[] = {
const unsigned short SEQ_SETTING[] = {
	SLEEPMSEC, 10,
	COMMAND_ONLY, COMMAND_ONLY, /* panel setting */
	COMMAND_ONLY, COMMAND_ONLY,
	0x00A4, 0x0001,
	SLEEPMSEC, 10,
	0x0060, 0x2700,
	0x0008, 0x0808,
/*Y Control, R30~39h: γ Control*/
	0x0030, 0x0500,
	0x0031, 0x7715,
	0x0032, 0x0803, 

	0x0033, 0x080a, 
	0x0034, 0x0000,
	0x0035, 0x0a08,
	0x0036, 0x7308,
	0x0037, 0x1507, 
	0x0038, 0x0009,
	0x0039, 0x3333,
	
	0x0090, 0x0017,
	0x000a, 0x0008,
	SLEEPMSEC, 20,
	0X0010, 0x0430,
	SLEEPMSEC, 100,
	0x0011, 0x0247,
	SLEEPMSEC, 100,
	0x000E, 0x0030,
	SLEEPMSEC, 20,
	0x0013, 0x1600,
	SLEEPMSEC, 20,
	0x002A, 0x006b,
	0x0012, 0x019a,
	SLEEPMSEC, 50,
	0x0012, 0x01bf,
	SLEEPMSEC, 200,
/*Window address control instruction	*/	
	0x0050, 0x0000,
	0x0051, 0x00EF,
	0x0052, 0x0000,
	0x0053, 0x013F,
/*Base image display control instruction*/
	0x0061, 0x0001,
	0x006A, 0x0000,
/*Partial display control instruction*/		
	0x0080, 0x0000,
	0x0081, 0x0000,
	0x0082, 0x0000,
/*Panel interface control instruction*/	
	0x0092, 0x0300,
	0x0093, 0x0503,

	0x0094, 0x0005, 
	0x0095, 0x0003,
	0x0001, 0x0100,
	SLEEPMSEC, 20,
	0x0002, 0x0200,
	SLEEPMSEC, 20,
	0x0003, 0x1030,
	SLEEPMSEC, 20,
	0x000C, 0x0110,
/*客户注意这个参数设置，需要和客户端一致，详见61505w此寄存器描述*/
	0x000F, 0x0000,
/*RAM access instruction*/
	0x0020, 0x0000,
	0x0021, 0x0000,
	0x0007, 0x0100,
	SLEEPMSEC, 40,
	0x0022, 0x0000,
	SLEEPMSEC, 100,
	ENDDEF, 0x0000
};

/*
*Get pulse number
*
*/
const unsigned short RECEIVE_BUF[100] ;

static  char  get_pulse_num(bl_level level) { 
	char loop_num = BL_LEVEL_NUM;
	while(loop_num --){
		if(level == bl_data[loop_num].level)
			return PULSE_NUM(bl_data[loop_num].data,cur_bl_data.data);
		else
			continue;
	}
	return -1;
}
static void r61505w_updata_backlight(bl_level level)
{
	unsigned char pulse_num = get_pulse_num(level);
	if(pulse_num < 0){
		printk(KERN_ERR,"Get pulse number fail!\n");
		return;
	}
		
	if(level == BL_LEVEL0){
			gpio_set_value(KTD104_BL_GPIO_CTRL, 0);
			udelay(600);
	}
	/*save current backlight state */
	cur_bl_data.level = level;
	cur_bl_data.data = pulse_num;
	/*set backlight */
	while(pulse_num --){
		gpio_set_value(KTD104_BL_GPIO_CTRL, 0);
		udelay(100);		
		gpio_set_value(KTD104_BL_GPIO_CTRL, 1);
		udelay(150);
	}
	
}

static int r61505w_spi_write_driver(unsigned short addr, unsigned short data)
{
	u32 buf[1];
	struct spi_message msg;

	struct spi_transfer xfer = {
		.len	= 4,
		.tx_buf	= buf,
	};

	buf[0] = (addr << 16) | data;

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);
	return spi_sync(g_spi, &msg);
}
static void r61505w_spi_read(unsigned short address, unsigned short *rxbuf)
{
	u32 buf[1];
	int			status;
	struct spi_message msg;

	struct spi_transfer xfer = {
		.len	= 2,
		.rx_buf	= buf,
	};
	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);

	r61505w_spi_write_driver(0x70, address);
	status = spi_sync(g_spi, &msg);	
	if (status == 0) {
		memcpy(rxbuf, buf, sizeof(buf));
		status = msg.status;
	}
	
}

static void r61505w_spi_write(unsigned short address, unsigned short command)
{
	if (address == COMMAND_ONLY)
	{
		r61505w_spi_write_driver(0x70, address);
		r61505w_spi_write_driver(0x70, command);
	}
	else if(address == 0x0022)
	{
		r61505w_spi_write_driver(0x70, address);
	}
	else
	{
		r61505w_spi_write_driver(0x70, address);
		r61505w_spi_write_driver(0x72, command);
	}
}

static void r61505w_panel_send_sequence(const unsigned short *wbuf)
{
	int i = 0;

	while ((wbuf[i] & DEFMASK) != ENDDEF) {
		if ((wbuf[i] & DEFMASK) != SLEEPMSEC)
			r61505w_spi_write(wbuf[i], wbuf[i+1]);
		else
			msleep(wbuf[i+1]);
			//mdelay(wbuf[i+1]);
		i += 2;
	}
}
static void r61505w_panel_receive_sequence(const unsigned short *wbuf)
{
	int i = 0;
	unsigned short *recv_p;
	recv_p = RECEIVE_BUF;
	while ((wbuf[i] & DEFMASK) != ENDDEF) {
		if ((wbuf[i] & DEFMASK) != SLEEPMSEC)
			r61505w_spi_read(wbuf[i], recv_p);
		i += 2;
		recv_p++ ;
	}
}

void r61505w_ldi_init(void)
{
	int i;
	r61505w_panel_send_sequence(SEQ_SETTING);
//	r61505w_panel_send_sequence(SEQ_STANDBY_OFF);

	r61505w_panel_receive_sequence(SEQ_SETTING);
//	for(i=0; i < 50; i++)
//		printk("zhoujian@@@receive_sequence:RECEIVE_BUF[%d] = %d \n\r",i,RECEIVE_BUF[i]);
}

static int __init r61505w_probe(struct spi_device *spi)
{
	int ret;

	spi->bits_per_word = 24;
	ret = spi_setup(spi);
	g_spi = spi;
	printk("zhoujian@@r61505w_probe\n\r");
	davinci_cfg_reg(DM365_GPIO35, PINMUX_RESV);
	davinci_cfg_reg(DM365_GPIO36, PINMUX_RESV);
	davinci_cfg_reg(DM365_GPIO37, PINMUX_RESV);
	gpio_direction_output(SPI_GPIO_SCL, 1);
	gpio_direction_output(SPI_GPIO_SDI, 1);
//	gpio_direction_output(SPI_GPIO_SDO, 1);
	gpio_direction_output(SPI_GPIO_CS, 1);
	
	r61505w_ldi_init();
//	r61505w_ldi_enable();

	r61505w_updata_backlight(BL_LEVEL3);

	davinci_cfg_reg(DM365_GPIO35, PINMUX_FREE);
	davinci_cfg_reg(DM365_GPIO36, PINMUX_FREE);
	davinci_cfg_reg(DM365_GPIO37, PINMUX_FREE);

	davinci_cfg_reg(DM365_SPI4_SCLK, PINMUX_RESV);
	davinci_cfg_reg(DM365_SPI4_SDO, PINMUX_RESV);
	davinci_cfg_reg(DM365_SPI4_SDENA0, PINMUX_RESV);

	if (ret < 0)
		return 0;

	return ret;
}
static int r61505w_remove(struct spi_device *spi)
{
	printk("zhoujian@@r61505w_remove\n\r");
	return 0;
} 
static int r61505w_lcd_sleep()
{
}
static int r61505w_lcd_wakeup()
{
}

static int spi_gpio_open(struct inode *inode, struct file *filp)
{
	davinci_cfg_reg(DM365_GPIO35, PINMUX_RESV);
	davinci_cfg_reg(DM365_GPIO36, PINMUX_RESV);
	davinci_cfg_reg(DM365_GPIO37, PINMUX_RESV);
	printk("zhoujian@@@spi_gpio_open \n");
	return 0;
}
static int spi_gpio_release(struct inode *inode, struct file *filp)
{

	davinci_cfg_reg(DM365_GPIO35, PINMUX_FREE);
	davinci_cfg_reg(DM365_GPIO36, PINMUX_FREE);
	davinci_cfg_reg(DM365_GPIO37, PINMUX_FREE);	
	printk("zhoujian@@@spi_gpio_release \n");
	return 0;
}
static int spi_gpio_ioctl(struct inode *inode, struct file *filp,
	    unsigned int cmd, unsigned long args)
{
	int mode;
    	switch (cmd) {
	case SPI_GPIO_IOC_INIT:
		gpio_direction_output(SPI_GPIO_SCL, 1);
		gpio_direction_output(SPI_GPIO_SDI, 1);
		gpio_direction_output(SPI_GPIO_CS, 1);
		r61505w_ldi_init();
		r61505w_updata_backlight(BL_LEVEL3);
		printk("zhoujian@@@spi_gpio_ioctl_init \n");
		break;
	case SPI_GPIO_IOC_SET:
		mode = args;
		if(mode == 0)
		{
			r61505w_lcd_sleep();
			printk("zhoujian@@@spi_gpio_ioctl_sleep mode = %d \n",mode);
		}
		else
		{
			r61505w_lcd_wakeup();
			printk("zhoujian@@@spi_gpio_ioctl_unsleep mode = %d \n",mode);
		}
		break;
		
    	}
	return 0;
}

static struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
	.open = spi_gpio_open,
	.release = spi_gpio_release,
	.ioctl = spi_gpio_ioctl,
};
static struct spi_driver r61505w_driver = {
	.driver = {
		.name	= "R61505W",
		.owner	= THIS_MODULE,
	},
	.probe		= r61505w_probe,
	.remove		= r61505w_remove,
	.suspend	= NULL,
	.resume		= NULL,
}; 
/* global variable which keeps major and minor number of the driver in it */
static dev_t dev;
/* global variable of type cdev to register driver to the kernel */
static struct cdev cdev;

static struct class *spi_gpio_class;

#define DRIVERNAME  "Spi_Gpio_R61505w"

static int __init r61505w_init(void)
{
	int result;
	/* Register the driver in the kernel */
	/* dynamically get the major number for the driver using
	   alloc_chrdev_region function */	

	result = alloc_chrdev_region(&dev, 0, 1, DRIVERNAME);
	/* if it fails return error */
	if (result < 0) {
		printk(KERN_ERR "DaVinciPreviewer: Module intialization \
		failed. could not register character device\n");
		return -ENODEV;
	} else{
	}
	/* initialize cdev with file operations */
	cdev_init(&cdev, &gpio_fops);
	
	cdev.owner = THIS_MODULE;
	cdev.ops = &gpio_fops;

	/* add cdev to the kernel */
	result = cdev_add(&cdev, dev, 1);
	if (result) {
		unregister_chrdev_region(dev, 1);
		printk(KERN_ERR
		       "r61505w_init: Error adding "
			"r61505w_init .. error no:%d\n",
		       result);
		return -EINVAL;
	}
	
	/* register character driver to the kernel */
	register_chrdev(MAJOR(dev), DRIVERNAME, &gpio_fops);
	
	if( spi_register_driver(&r61505w_driver) != 0){
		unregister_chrdev(MAJOR(dev), DRIVERNAME);
		unregister_chrdev_region(dev, 1);
		cdev_del(&cdev);
		return -EINVAL;
	}

	spi_gpio_class = class_create(THIS_MODULE, "R61505W");
	if (!spi_gpio_class) {
		driver_unregister(&r61505w_driver);
		unregister_chrdev_region(dev, 1);
		unregister_chrdev(MAJOR(dev), DRIVERNAME);
		cdev_del(&cdev);
		return -EIO;
	}
	class_device_create(spi_gpio_class, NULL, dev, NULL, "R61505W");
	return 0;
}

static void __exit r61505w_exit(void)
{
	unregister_chrdev_region(dev, 1);
	class_device_destroy(spi_gpio_class, dev);
	class_destroy(spi_gpio_class);
	spi_unregister_driver(&r61505w_driver);
//	platform_device_unregister(&previewer_device);
	cdev_del(&cdev);
	unregister_chrdev(MAJOR(dev), DRIVERNAME);
	return 0;
}

module_init(r61505w_init);
module_exit(r61505w_exit);
MODULE_LICENSE("GPL");

