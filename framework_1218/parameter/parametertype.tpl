[+ AutoGen5 template hin +]
[+ CASE (suffix) +]
[+ == hin +]#ifndef _[+ (string-upcase!(base-name)) +]_AUTOGEN_
#define _[+ (string-upcase!(base-name)) +]_AUTOGEN_
/*	This file is a autogenerate file .
 *	DONOT EDIT THIS FILE!!
 *  Edit [+ (def-file) +] and [+ (tpl-file) +] instead
 */
[+ FOR parameter +][+ IF (exist?"remotecall") +][+ IF (exist?"rt_in_type") +]
#include "parameters/parameter_[+ (get "rt_in_type") +].h"[+ ENDIF +][+ IF (exist?"rt_out_type") +]
#include "parameters/parameter_[+ (get "rt_out_type") +].h"[+ ENDIF +][+ ELIF (exist?"var_type") +]
#include "parameters/parameter_[+ (get "var_type") +].h"[+ ELIF (exist?"uint_type") +][+ IF (string-length (get "uint_type")) +]
#include "parameters/parameter_[+ (get "uint_type") +].h"[+ ENDIF +][+ ENDIF +][+ ENDFOR parameter +]
#include "parameter.h"

#ifdef __cplusplus
extern "C"{
#endif

[+ FOR parameter +][+ IF (exist?"remotecall") +]
#define [+ (string-downcase! (get "cat_name")) +]_[+ (string-downcase! (get "para_name")) +]_in_paratype	[+ IF (exist?"rt_in_type") +]struct [+ (get "rt_in_type") +][+ ELSE +]void[+ ENDIF +]
#define [+ (string-downcase! (get "cat_name")) +]_[+ (string-downcase! (get "para_name")) +]_out_paratype	[+ IF (exist?"rt_out_type") +]struct [+ (get "rt_out_type") +][+ ELSE +]void[+ ENDIF +]
[+ ELIF (exist?"var_type") +]
#define [+ (string-downcase! (get "cat_name")) +]_[+ (string-downcase! (get "para_name")) +]_paratype	struct [+ (get "var_type") +]
[+ ELIF (exist?"int_type") +]
#define [+ (string-downcase! (get "cat_name")) +]_[+ (string-downcase! (get "para_name")) +]_paratype	long
[+ ELIF (exist?"uint_type") +]
#define [+ (string-downcase! (get "cat_name")) +]_[+ (string-downcase! (get "para_name")) +]_paratype	unsigned long 
[+ ENDIF +][+ ENDFOR parameter +]

#ifdef __cplusplus
}
#endif

#endif

[+ ESAC +]

