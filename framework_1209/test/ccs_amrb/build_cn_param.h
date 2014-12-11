/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef build_CN_param_h
#define build_CN_param_h "$Id $"

#include "typedef.h"
/*************************************************************************
 *
 *   FUNCTION NAME: build_CN_param
 *
 *************************************************************************/
void build_CN_param (
    Word16 *seed,             /* i/o : Old CN generator shift register state */
    const Word16 n_param,           /* i  : number of params */  
    const Word16 param_size_table[],/* i : size of params */   
    Word16 parm[]             /* o : CN Generated params */
    );
#endif
