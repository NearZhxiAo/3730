#ifndef	_LIB_AD_H 
#define	_LIB_AD_H

#ifdef __cplusplus
extern "C"{
#endif

#define CMDINFO_LEN 128

typedef struct _AD_STRUCT_
{
	volatile unsigned int ad_adctl;	 	//00h
	volatile unsigned int ad_cmptgt; 	//04h
	volatile unsigned int ad_cmpldat; 	//08h
	volatile unsigned int ad_cmpudat; 	//0Ch
	volatile unsigned int ad_setdiv; 	//10h
	volatile unsigned int ad_chsel; 	//14h
	volatile unsigned int ad_ad0dat; 	//18h
	volatile unsigned int ad_ad1dat; 	//1Ch
	volatile unsigned int ad_ad2dat; 	//20h
	volatile unsigned int ad_ad3dat; 	//24h
	volatile unsigned int ad_ad4dat; 	//28h
	volatile unsigned int ad_ad5dat; 	//2Ch
	volatile unsigned int ad_emuctrl; 	//30h
}AD_STRUCT;

typedef struct _LIBAD_OBJ
{
    int ad_fd;
	unsigned int ad_mmap;
	unsigned int ad_phyaddr;

	AD_STRUCT *pad_reg;
	int reserved;
}LIBAD_OBJ;

typedef struct _ad_cmd_struct
{
    unsigned int cmd;				//cmd index
    unsigned int cmdlen;			//cmd length
    unsigned char cmdinfo[CMDINFO_LEN];		//cmd info
    unsigned int ret_ack;			//reserved
}AD_CMD_STRUCT;

#define AD_PHYADDR_BASE            0x01C64000
#define AD_STRUCT_LEN              sizeof(AD_STRUCT)

LIBAD_OBJ *AD_INIT();
succeed_type AD_DESTROY( LIBAD_OBJ *);

#ifdef __cplusplus
}
#endif
#endif  
