#ifndef _PHYS_ADDR_H__
#define _PHYS_ADDR_H__
#if defined (__cplusplus)
extern "C" {
#endif

#define KERNEL_PHYS_ADDR_START	0x80000000
#define KERNEL_PHYS_SIZE		0x03C00000	//

#define SHAREMEM_PHYS_ADDR		0x85000000	
#define SHAREMEM_PHYS_SIZE		0x00100000

#define LOGMEM_PHYS_ADDR		0x85100000
#define LOGMEM_PHYS_SIZE		0x00100000

#define VSSHMMEM_PHYS_ADDR		0x85200000
#define VSSHMMEM_PHYS_SIZE		0x00100000

#define ASSHMMEM_PHYS_ADDR		0x85300000
#define ASSHMMEM_PHYS_SIZE		0x00100000


#if defined (__cplusplus)
}
#endif

#endif
