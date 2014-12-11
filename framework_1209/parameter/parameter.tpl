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

enum unxi_filename_type {
	[+ FOR category ","+]
	UNIX_FILENAME_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
	,UNIX_FILENAME_END
};


[+ FOR category "" +]
#define UNIX_SOCKET_[+ (string-upcase! (get "cat_name")) +]  "/var/unix_socket_[+ (string-upcase! (get "cat_name")) +]"
[+ ENDFOR category +]
#define MAX_UNIXFILENAME_LEN	64
static char find_unix_name[UNIX_FILENAME_END][MAX_UNIXFILENAME_LEN] = {[+ FOR category "," +]
	UNIX_SOCKET_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
};

enum para_cat_type {
	[+ FOR category ","+]
	PARA_CATEGORY_[+ (string-upcase! (get "cat_name")) +]
	[+ ENDFOR category +]
};
#define MAX_CATEGORY_BIT		(6)
#define MAX_CATEGORY_PARA_NUM	(1<<(MAX_CATEGORY_BIT))
enum para_cmd_indicator_type {[+ FOR parameter "," +]
	PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "para_name")) +] = (PARA_CATEGORY_[+ (string-upcase! (get "cat_name")) +] << MAX_CATEGORY_BIT) + [+ para_index +][+ ENDFOR parameter +]
	,PARA_CMD_END
};


static inline void produce_para_cmd_value(void)
{
	printf("/**********************parameter_value define**********************/\n");
	printf("enum para_cmd_value \n{ \n");
	[+ FOR parameter  +]
	printf("PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "para_name")) +] = 0x%x,", PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_[+ (string-upcase! (get "para_name")) +]);
	printf("      /* %s */         \n",  "[+ para_descript +]");
	[+ ENDFOR parameter +]
	printf("};");
}
enum para_cmd_category_base {[+ FOR category "," +]
	PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_BASE = (PARA_CATEGORY_[+ (string-upcase! (get "cat_name")) +] << MAX_CATEGORY_BIT) + 1
	[+ ENDFOR category +]
};

enum para_cmd_category_end {[+ FOR category "," +]
    PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_END = PARA_CMD_[+ (string-upcase! (get "cat_name")) +]_BASE  + (1<<MAX_CATEGORY_BIT)
    [+ ENDFOR category +]
};

#define PARA_CMD_BASE_GET(PARA_CMD)			(((PARA_CMD >> MAX_CATEGORY_BIT) << MAX_CATEGORY_BIT) + 1)
#define PARA_CATEGORY_GET(PARA_CMD)			((PARA_CMD >> MAX_CATEGORY_BIT))

#ifdef __cplusplus
}
#endif

#endif // _[+ (string-upcase!(base-name)) +]_AUTOGEN_

[+ ESAC +]

