#include "system_includes.h"
#include "system_debug.h"
#include "libgpio.h"

int main(int argc, char **argv)
{
    LIBGPIO_OBJ *p = GPIO_INIT();
	int iLoop = 0;
    if (p == NULL)
    {
		SYS_ERROR("libgpio_init failed.\r\n");
		return 0;
    }
	printf("USB_STRUCT_LEN/sizeof(int) = 0x%x\r\n",USB_STRUCT_LEN/sizeof(int));

	for(iLoop = 0; iLoop < USB_STRUCT_LEN/sizeof(int) ; iLoop ++)
	{
		if(iLoop < 0x3C)
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, ((unsigned int*)(p->pusb_reg))[iLoop]);
		}
		else
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, ((unsigned int*)(p->pusb_reg->usb_test))[iLoop - 0x3C]);
		}
	}
	SYS_WARNING("\r\nBefore...\r\n\r\n");

#if 1
	((unsigned int*)(p->pusb_reg))[0x00] = 0x140901;
	((unsigned int*)(p->pusb_reg))[0x01] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x02] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x03] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x04] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x05] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x06] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x07] = 0x77685f74;
	((unsigned int*)(p->pusb_reg))[0x08] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x09] = 0x12;
	((unsigned int*)(p->pusb_reg))[0x0A] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x0B] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x0C] = 0x0;
	((unsigned int*)(p->pusb_reg))[0x0D] = 0x97;
	((unsigned int*)(p->pusb_reg))[0x0E] = 0x80f9d22c;
	((unsigned int*)(p->pusb_reg))[0x0F] = 0x12;
	((unsigned int*)(p->pusb_reg))[0x10] = 0xd012;
	((unsigned int*)(p->pusb_reg))[0x11] = 0xd012;
	((unsigned int*)(p->pusb_reg))[0x12] = 0xd0022;
	((unsigned int*)(p->pusb_reg))[0x13] = 0x736f6c63;
	((unsigned int*)(p->pusb_reg))[0x14] = 0x61687370;
	((unsigned int*)(p->pusb_reg))[0x15] = 0x7000706e;
	((unsigned int*)(p->pusb_reg))[0x16] = 0x963cf85;
	((unsigned int*)(p->pusb_reg))[0x17] = 0xe5bcf614;
	((unsigned int*)(p->pusb_reg))[0x18] = 0xe3e03a0f;
	((unsigned int*)(p->pusb_reg))[0x19] = 0xaffffee;
	((unsigned int*)(p->pusb_reg))[0x1A] = 0xe1a0000c;
	((unsigned int*)(p->pusb_reg))[0x1B] = 0xe798200c;
	((unsigned int*)(p->pusb_reg))[0x1C] = 0xe3570000;
	((unsigned int*)(p->pusb_reg))[0x1D] = 0xe2405e4a;
	((unsigned int*)(p->pusb_reg))[0x1E] = 0xe1570003;
	((unsigned int*)(p->pusb_reg))[0x1F] = 0xe5b27004;
	((unsigned int*)(p->pusb_reg))[0x20] = 0xe3a04000;
	((unsigned int*)(p->pusb_reg))[0x21] = 0xe28da004;
	((unsigned int*)(p->pusb_reg))[0x22] = 0xe1a02009;
	((unsigned int*)(p->pusb_reg))[0x23] = 0xe1570003;
	((unsigned int*)(p->pusb_reg))[0x24] = 0xe3a03000;
	((unsigned int*)(p->pusb_reg))[0x25] = 0xe3a01001;
	((unsigned int*)(p->pusb_reg))[0x26] = 0x11a09000;
	((unsigned int*)(p->pusb_reg))[0x27] = 0xe59d1008;
	((unsigned int*)(p->pusb_reg))[0x28] = 0xebffe980;
	((unsigned int*)(p->pusb_reg))[0x29] = 0xaffffcb;
	((unsigned int*)(p->pusb_reg))[0x2A] = 0xe1a01002;
	((unsigned int*)(p->pusb_reg))[0x2B] = 0xe1a03004;
	((unsigned int*)(p->pusb_reg))[0x2C] = 0xe3a070f0;
	((unsigned int*)(p->pusb_reg))[0x2D] = 0xe5956024;
	((unsigned int*)(p->pusb_reg))[0x2E] = 0xe1a02005;
	((unsigned int*)(p->pusb_reg))[0x2F] = 0xe2850010;
	((unsigned int*)(p->pusb_reg))[0x30] = 0xe1a05082;
	((unsigned int*)(p->pusb_reg))[0x31] = 0xdaffffec;
	((unsigned int*)(p->pusb_reg))[0x32] = 0xe1a02007;
	((unsigned int*)(p->pusb_reg))[0x33] = 0xe50b3098;
	((unsigned int*)(p->pusb_reg))[0x34] = 0xe7803002;
	((unsigned int*)(p->pusb_reg))[0x35] = 0xe92d48f0;
	((unsigned int*)(p->pusb_reg))[0x36] = 0xe7802003;
	((unsigned int*)(p->pusb_reg))[0x37] = 0x1afffff2;
	((unsigned int*)(p->pusb_reg))[0x38] = 0xe1a03004;
	((unsigned int*)(p->pusb_reg))[0x39] = 0xe1a0c007;
	((unsigned int*)(p->pusb_reg))[0x3A] = 0xe1a00007;
	((unsigned int*)(p->pusb_reg))[0x3B] = 0xeaffffeb;
#endif 

#if 1
	((unsigned int*)(p->pusb_reg->usb_test))[0x00] = 0x0;
#endif

#if 0
	((unsigned int*)(p->pusb_reg->usb_test))[0x01] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x02] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x03] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x04] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x05] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x06] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x07] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x08] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x09] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0A] = 0x66d5c456;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0B] = 0x3cfb4c69;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x0F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x10] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x11] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x12] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x13] = 0xd9162c83;
	((unsigned int*)(p->pusb_reg->usb_test))[0x14] =	0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x15] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x16] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x17] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x18] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x19] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1A] = 0xed54e329;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1B] = 0x237fb2ab;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x1F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x20] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x21] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x22] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x23] = 0x48c57997;
	((unsigned int*)(p->pusb_reg->usb_test))[0x24] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x25] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x26] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x27] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x28] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x29] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2A] = 0x8e83b837;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2B] = 0x35dc1ec;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x2F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x30] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x31] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x32] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x33] = 0xd6526185;
	((unsigned int*)(p->pusb_reg->usb_test))[0x34] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x35] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x36] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x37] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x38] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x39] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3A] = 0xe0e1ef67;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3B] = 0xadb4ce8c;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x3F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x40] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x41] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x42] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x43] = 0xdb24bcd2;
	((unsigned int*)(p->pusb_reg->usb_test))[0x44] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x45] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x46] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x47] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x48] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x49] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x4F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x50] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x51] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x52] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x53] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x54] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x55] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x56] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x57] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x58] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x59] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x5F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x60] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x61] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x62] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x63] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x64] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x65] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x66] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x67] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x68] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x69] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x6F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x70] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x71] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x72] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x73] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x74] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x75] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x76] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x77] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x78] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x79] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x7F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x80] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x81] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x82] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x83] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x84] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x85] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x86] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x87] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x88] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x89] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x8F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x90] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x91] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x92] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x93] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x94] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x95] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x96] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x97] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x98] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x99] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9A] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9B] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9C] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9D] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9E] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x9F] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA0] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA2] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA3] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA4] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA5] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA6] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA7] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xA9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAA] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAB] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAC] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAD] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAE] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xAF] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB0] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB2] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB3] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB4] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB5] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB6] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB7] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xB9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBA] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBB] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBC] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBD] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBE] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xBF] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC0] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC2] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC3] = 0x0;

	((unsigned int*)(p->pusb_reg->usb_test))[0xC4] = 0xe000;	
	((unsigned int*)(p->pusb_reg->usb_test))[0xC5] = 0x1f0000;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC6] = 0xf700001e;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC7] = 0x40000;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xC9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCA] = 0x1ff4;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCB] = 0x33000000;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCC] = 0x4a3f83b;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCD] = 0x66a4ac31;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCE] = 0xcb45b757;
	((unsigned int*)(p->pusb_reg->usb_test))[0xCF] = 0x5e9d36fc;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD0] = 0x8ddf8d0e;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD2] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD3] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD4] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD5] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD6] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD7] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xD9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDA] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDB] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDC] = 0x5050019;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDD] = 0x1880188;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDE] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xDF] = 0x5f4;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE0] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE2] = 0x3c5c0a44;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE3] = 0x727780;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE4] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE5] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE6] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE7] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xE9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xEA] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xEB] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xEC] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xED] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xEE] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xEF] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF0] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF1] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF2] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF3] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF4] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF5] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF6] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF7] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF8] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xF9] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFA] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFB] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFC] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFD] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFE] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0xFF] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x100] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x101] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x102] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x103] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x104] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x105] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x106] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x107] = 0x6000000;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x108] = 0x0;     
	((unsigned int*)(p->pusb_reg->usb_test))[0x109] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x10A] = 0x1ff0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x10B] = 0x55000000;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x10C] = 0x0;    
	((unsigned int*)(p->pusb_reg->usb_test))[0x10D] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x10E] = 0x1ff0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x10F] = 0x55000000;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x110] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x111] = 0x0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x112] = 0x1ff0;
	((unsigned int*)(p->pusb_reg->usb_test))[0x113] = 0x33000000;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x114] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x115] = 0x0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x116] = 0x1ff0;        
	((unsigned int*)(p->pusb_reg->usb_test))[0x117] = 0x33000000;            
#endif
	SYS_WARNING("\r\nAfter...\r\n\r\n");

	for(iLoop = 0; iLoop < USB_STRUCT_LEN/sizeof(int) ; iLoop ++)
	{
		if(iLoop < 0x3C)
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, ((unsigned int*)(p->pusb_reg))[iLoop]);
		}
		else
		{
			printf("USB-REG0x%x:0x%x\r\n",iLoop, ((unsigned int*)(p->pusb_reg->usb_test))[iLoop - 0x3C]);
		}
	}
		                      
    GPIO_DESTROY(p);    
    return 0;           
}                       
                        
                        
                        
                        
                        
                        
                        
