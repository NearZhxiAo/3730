#ifndef	_LIB_GPIO_H 
#define	_LIB_GPIO_H

#ifdef __cplusplus
extern "C"{
#endif

#define CMDINFO_LEN 128

/* all the regs*/
typedef struct _gpio_regs_struct
{
	volatile unsigned int gpio_pid; 		//00h

	volatile unsigned int gpio_reserved0;	//04h
	volatile unsigned int gpio_binten;   	//08h

	volatile unsigned int gpio_reserved1;	//0Ch
	volatile unsigned int gpio_dir01;		//10h
	volatile unsigned int gpio_out_data01;	//14h
	volatile unsigned int gpio_set_data01;	//18h
	volatile unsigned int gpio_clr_data01;	//1Ch
	volatile unsigned int gpio_in_data01;	//20h
	volatile unsigned int gpio_set_ris_trig01;//24h
	volatile unsigned int gpio_clr_ris_trig01;//28h
	volatile unsigned int gpio_set_fal_trig01;//2Ch
	volatile unsigned int gpio_clr_fal_trig01;//30h
	volatile unsigned int gpio_intstat01;		//34h
	volatile unsigned int gpio_dir23;		//38h
	volatile unsigned int gpio_our_data23;	//3Ch
	volatile unsigned int gpio_set_data23;	//40h
	volatile unsigned int gpio_clr_data23;	//44h
	volatile unsigned int gpio_in_data23;	//48h

	volatile unsigned int gpio_set_ris_trig23;	//4Ch
	volatile unsigned int gpio_clr_ris_trig23;	//50h
	volatile unsigned int gpio_set_fal_trig23;	//54h
	volatile unsigned int gpio_clr_fal_trig23;	//58h
	volatile unsigned int gpio_intstat23;	//5Ch

	volatile unsigned int gpio_dir4;		//60h
	volatile unsigned int gpio_out_data4;	//64h
	volatile unsigned int gpio_set_data4;	//68h
	volatile unsigned int gpio_clr_data4;	//6Ch
	volatile unsigned int gpio_in_data4;	//70h

	volatile unsigned int gpio_set_ris_trig4;	//74h
	volatile unsigned int gpio_clr_ris_trig4;	//78h
	volatile unsigned int gpio_set_fal_trig4;	//7Ch
	volatile unsigned int gpio_clr_fal_trig4;	//80h
	volatile unsigned int gpio_intstat4;	//84h
/*
	volatile unsigned int gpio_dir6;		//88h
	volatile unsigned int gpio_out_data6;	//8Ch
	volatile unsigned int gpio_set_data6;	//90h
	volatile unsigned int gpio_clr_data6;	//94h
	volatile unsigned int gpio_in_data6;	//98h
*/
}GPIOREG_STRUCT;


/* all the pinmuxs with gpio*/
typedef struct _pinmux_struct
{
	volatile unsigned int pinmux_0;
	volatile unsigned int pinmux_1;
//	volatile unsigned int pinmux_2; 
//	volatile unsigned int pinmux_3;
//	volatile unsigned int pinmux_4;
}PINMUX_STRUCT;

#define PINMUX_PHYADDR_BASE			0x01C40000
#define PINMUX_PHYADDR_3			PINMUX_PHYADDR_BASE + 0x0C		
#define PINMUX_PHYADDR_4			PINMUX_PHYADDR_BASE + 0x10		
#define PINMUX_STRUCT_LEN			sizeof(PINMUX_STRUCT)

#define GPIOREG_PHYADDR_BASE 		0x01C67000
#define GPIOREG_STRUCT_LEN			sizeof(GPIOREG_STRUCT)

#define I2C_PHYADDR_BASE			0x01C21000

/* i2c struct */
typedef struct _i2c_struct
{
	volatile unsigned int oar;
	volatile unsigned int imr;
	volatile unsigned int str; 
	volatile unsigned int clkl;
	volatile unsigned int clkh;
	volatile unsigned int cnt;
	volatile unsigned int drr;
	volatile unsigned int sar;
	volatile unsigned int dxr;
	volatile unsigned int mdr;
	volatile unsigned int ivr;
	volatile unsigned int emdr;
	volatile unsigned int psc;
	volatile unsigned int pid1;
	volatile unsigned int pid2;
}I2C_STRUCT;

#define I2C_STRUCT_LEN			sizeof(I2C_STRUCT)

typedef enum _GPIO_OUTPUT_DEFINE
{
	GPIO_OUTPUT_START,
	GPIO_OUTPUT_PWR_LED_RED,
	GPIO_OUTPUT_PWR_LED_GREEN,
	GPIO_OUTPUT_STATE_LED_RED,
	GPIO_OUTPUT_STATE_LED_GREEN,
	GPIO_OUTPUT_DSP_PWR_EN,
	GPIO_OUTPUT_DSP_TVP5150_HI,
	GPIO_OUTPUT_DSP_TVP5150_LOW,
	GPIO_OUTPUT_DSP_3G_HI,
	GPIO_OUTPUT_DSP_3G_LOW,
	GPIO_OUTPUT_3G_LED_GREEN,
	WDQ_GPIO_OUTPUT_RST_LOW_5150,
	WDQ_GPIO_OUTPUT_RST_HIGH_5150,
	GPIO_OUTPUT_END,
}GPIO_OUTPUT_DEFINE;

typedef enum _GPIO_INPUT_DEFINE
{
	GPIO_INPUT_START,
	GPIO_INPUT_DSP_TRIG,
	GPIO_INPUT_DSP_PWR,
	GPIO_INPUT_DSP_KEY_DECT,
	GPIO_INPUT_END,
}GPIO_INPUT_DEFINE;


typedef struct _gpio_cmd_struct
{
	unsigned int cmd;				//cmd index
	unsigned int cmdlen;			//cmd length
	unsigned char cmdinfo[CMDINFO_LEN];		//cmd info
	unsigned int ret_ack;			//reserved
}GPIO_CMD_STRUCT;

#define USB_REG_FORTEST_LEN 0x153 - 0x3B

typedef struct _usb_struct 
{   
	volatile unsigned int usb_reserved0;    //00h

	volatile unsigned int usb_ctrlr;        //04h
	volatile unsigned int usb_statr;        //08h
	volatile unsigned int usb_reserved1;    //0Ch

	volatile unsigned int usb_rndisr;       //10h
	volatile unsigned int usb_autoreg;      //14h
	volatile unsigned int usb_reserved2;    //18h
	volatile unsigned int usb_reserved3;    //1Ch

	volatile unsigned int usb_intsrcr;      //20h
	volatile unsigned int usb_intsetr;      //24h
	volatile unsigned int usb_intclrr;      //28h
	volatile unsigned int usb_intmskr;      //2Ch
	volatile unsigned int usb_intmsksetr;   //30h
	volatile unsigned int usb_intmskclrr;   //34h
	volatile unsigned int usb_intmskkedr;   //38h
	volatile unsigned int usb_eoir;         //3Ch
	volatile unsigned int usb_intvectr;     //40h

	volatile unsigned int usb_reserved4;    //44h
	volatile unsigned int usb_reserved5;    //48h
	volatile unsigned int usb_reserved6;    //4Ch
	volatile unsigned int usb_reserved7;    //50h
	volatile unsigned int usb_reserved8;    //54h
	volatile unsigned int usb_reserved9;    //58h
	volatile unsigned int usb_reserved11;   //5Ch
	volatile unsigned int usb_reserved12;   //60h
	volatile unsigned int usb_reserved13;   //64h
	volatile unsigned int usb_reserved14;   //68h
	volatile unsigned int usb_reserved15;   //6Ch
	volatile unsigned int usb_reserved16;   //70h
	volatile unsigned int usb_reserved17;   //74h
	volatile unsigned int usb_reserved18;   //78h
	volatile unsigned int usb_reserved19;   //7Ch

	volatile unsigned int usb_tcppicr;      //80h
	volatile unsigned int usb_tcppitdr;     //84h
	volatile unsigned int usb_tcppieoir;    //88h
	volatile unsigned int usb_reserved;     //8Ch
	volatile unsigned int usb_tcppimsksr;   //90h
	volatile unsigned int usb_tcppirawsr;   //94h
	volatile unsigned int usb_tcppiiensetr; //98h
	volatile unsigned int usb_tcppiienclrr; //9Ch

	volatile unsigned int usb_reserved20;   //A0h
	volatile unsigned int usb_reserved21;   //A4h
	volatile unsigned int usb_reserved22;   //A8h
	volatile unsigned int usb_reserved23;   //ACh
	volatile unsigned int usb_reserved24;   //B0h
	volatile unsigned int usb_reserved25;   //B4h
	volatile unsigned int usb_reserved26;   //B8h
	volatile unsigned int usb_reserved27;   //BCh
	volatile unsigned int usb_rcppicr;   	//C0h
	volatile unsigned int usb_reserved28;   //C4h
	volatile unsigned int usb_reserved29;   //C8h
	volatile unsigned int usb_reserved30;   //CCh

	volatile unsigned int usb_rcppimsksr;   //D0h
	volatile unsigned int usb_rcppirawsr;   //D4h
	volatile unsigned int usb_rcppiensetr;  //D8h
	volatile unsigned int usb_rcppiienclrr; //DCh
	volatile unsigned int usb_rbufcnt0;	 	//E0h
	volatile unsigned int usb_rbufcnt1;	 	//E4h
	volatile unsigned int usb_rbufcnt2;	 	//E8h
	volatile unsigned int usb_rbufcnt3;	 	//ECh

	volatile unsigned int usb_test[USB_REG_FORTEST_LEN]; //F0h ~ ...
}USB_STRUCT;

typedef struct _LIBGPIO_OBJ
{
	int gpio_fd;
	unsigned int gpio_mmap;
	unsigned int gpio_phyaddr;
	unsigned int pinmux_mmap;
	unsigned int pinmux_phyaddr;
	unsigned int usb_mmap;
	unsigned int usb_phyaddr;

	unsigned int i2c_mmap;
	unsigned int i2c_phyaddr;

	GPIOREG_STRUCT *pgpio_reg;
	PINMUX_STRUCT *ppinmux_reg;
	I2C_STRUCT *pi2c_reg;
	USB_STRUCT *pusb_reg;

	int reserved;
}LIBGPIO_OBJ;

#define USB_PHYADDR_BASE            0x01C64000
#define USB_STRUCT_LEN              sizeof(USB_STRUCT)

succeed_type GPIO_5150_SETLOW(LIBGPIO_OBJ *phandle);
succeed_type GPIO_5150_SETHIGH(LIBGPIO_OBJ *phandle);

#ifdef __cplusplus
}
#endif
#endif  
