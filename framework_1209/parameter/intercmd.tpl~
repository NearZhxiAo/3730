[+ AutoGen5 template h +]
[+ CASE (suffix) +]
[+ == h +]#ifndef _[+ (string-upcase!(base-name)) +]_AUTOGEN_
#define _[+ (string-upcase!(base-name)) +]_AUTOGEN_
/*	This file is a autogenerate file .
 *	DONOT EDIT THIS FILE!!
 *  Edit [+ (def-file) +] and [+ (tpl-file) +] instead
 */
#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <stdlib.h>

enum unix_type {
	[+ FOR category ","+]
	UNIX_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
	,UNIX_END
};


[+ FOR category "" +]
#define UNIXFILE_SOCKET_[+ (string-upcase! (get "cat_name")) +]  "/tmp/unix_socket_[+ (string-upcase! (get "cat_name")) +]"
[+ ENDFOR category +]
#define MAX_UNIXFILENAME_LENGTH	64
static char findcmd_unix_name[UNIX_END][MAX_UNIXFILENAME_LENGTH] = {[+ FOR category "," +]
	UNIXFILE_SOCKET_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
};

enum cmd_cat_type {
	[+ FOR category ","+]
	CMD_CATEGORY_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
};
#define MAX_CATEGORY_BITDEF		(6)
#define MAX_CATEGORY_INTERPARA_NUM	(1<<(MAX_CATEGORY_BITDEF))
enum paracmd_indicator_type {[+ FOR parameter "," +]
	PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +] = (CMD_CATEGORY_[+ (string-upcase! (get "cat_name")) +] << MAX_CATEGORY_BITDEF) + [+ para_index +][+ ENDFOR parameter +]
	,PARACMD_END
};


static inline void produce_paracmd_value(void)
{
	printf("/**********************parameter_value define**********************/\n");
	printf("enum para_cmd_value \n{ \n");
	[+ FOR parameter  +]
	printf("PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +] = 0x%x,", PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]);
	printf("      /* %s */         \n",  "[+ para_descript +]");
	[+ ENDFOR parameter +]
	printf("};");
}
enum paracmd_category_base {[+ FOR category "," +]
	PARACMD_[+ (string-upcase! (get "cat_name")) +]_BASE = (CMD_CATEGORY_[+ (string-upcase! (get "cat_name")) +] << MAX_CATEGORY_BITDEF) + 1
	[+ ENDFOR category +]
};

enum paracmd_category_end {[+ FOR category "," +]
    PARACMD_[+ (string-upcase! (get "cat_name")) +]_END = PARACMD_[+ (string-upcase! (get "cat_name")) +]_BASE  + (1<<MAX_CATEGORY_BITDEF)
    [+ ENDFOR category +]
};

#define PARACMD_BASE_GET(PARACMD)			(((PARACMD >> MAX_CATEGORY_BITDEF) << MAX_CATEGORY_BITDEF) + 1)
#define PARACATEGORY_GET(PARACMD)			((PARACMD >> MAX_CATEGORY_BITDEF))

enum paracmd_sendto_type {[+ FOR parameter "," +]
	[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]_SENDTO_DEST = [+ sendto_dest +][+ ENDFOR parameter +]
	,PARACMD_SENDTO_DEST_END
};
enum paracmd_sendto_flag_type {[+ FOR parameter "," +]
	[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]_SENDTO_FLAG = [+ sendto_flag +][+ ENDFOR parameter +]
	,PARACMD_SENDTO_FLAG_END
};

static inline void produce_cmd_value(void)
{
	printf("/**********************parameter_value define**********************/\n");
	printf("enum para_cmd_value \n{ \n");
	[+ FOR parameter  +]
	printf("PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +] = 0x%x,", PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]);
	printf("      /* %s */         \n",  "[+ para_descript +]");
	[+ ENDFOR parameter +]
	printf("};");
}

typedef struct paracmd_sendto_array_
{
	unsigned int para_cmd;
	unsigned int para_send;
	unsigned int sendto_flag;
}Paracmd_Sendto_array; 
static  Paracmd_Sendto_array paracmd_sendto_array[]={[+ FOR parameter "," +]
   { PARACMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +],
	 [+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]_SENDTO_DEST,
	 [+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "cmd_name")) +]_SENDTO_FLAG
	}
	[+ ENDFOR parameter +],
	{0,0,0}
};

static inline unsigned int  find_sendto_dest_throw_cmd(unsigned int para_cmd)
{
	unsigned int index = 0;
	for ( index = 0; index < (sizeof(paracmd_sendto_array)/sizeof(Paracmd_Sendto_array)); index++ )
	{
		if ( paracmd_sendto_array[index].para_cmd == para_cmd )
		{
			return paracmd_sendto_array[index].para_send;
		}
	}
	return 0;
}

static inline unsigned int  find_sendflag_ofcmd(unsigned int para_cmd)
{
    unsigned int index = 0;
    for ( index = 0; index < (sizeof(paracmd_sendto_array)/sizeof(Paracmd_Sendto_array)); index++ )
    {
        if ( paracmd_sendto_array[index].para_cmd == para_cmd )
        {
            return paracmd_sendto_array[index].sendto_flag;
        }
    }
    return 0;
}
#ifdef __cplusplus
}
#endif

#endif // _[+ (string-upcase!(base-name)) +]_AUTOGEN_

[+ ESAC +]

