/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"

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
    )
{
   Word16 i;
   const Word16 *p;

   *seed = extract_l(L_add(L_shr(L_mult(*seed, 31821), 1), 13849L));

   p = &window_200_40[*seed & 0x7F]; 
   for(i=0; i< n_param;i++){
        
     parm[i] = *p++ & ~(0xFFFF<<param_size_table[i]);  
   }
}
