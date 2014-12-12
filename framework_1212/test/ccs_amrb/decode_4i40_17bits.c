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
#include "cnst.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define NB_PULSE4 4

#include "table.h"
 
/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  decod_ACELP()
 *
 *  PURPOSE:   Algebraic codebook decoder
 *
 *************************************************************************/

void decode_4i40_17bits(
    Word16 sign,   /* i : signs of 4 pulses.                       */
    Word16 index,  /* i : Positions of the 4 pulses.               */
    Word16 cod[]   /* o : algebraic (fixed) codebook excitation    */
)
{
    Word16 i, j;
    Word16 pos[NB_PULSE4];

    /* Decode the positions */

    i = index & 7;                                       
    i = dgray[i];                                        

    pos[0] = add(i, shl(i, 2));   /* pos0 =i*5 */        

    index = shr(index, 3);
    i = index & 7;                                       
    i = dgray[i];                                        

    i = add(i, shl(i, 2));        /* pos1 =i*5+1 */
    pos[1] = add(i, 1);                                  

    index = shr(index, 3);
    i = index & 7;                                       
    i = dgray[i];                                        

    i = add(i, shl(i, 2));        /* pos2 =i*5+1 */
    pos[2] = add(i, 2);                                  

    index = shr(index, 3);
    j = index & 1;                                       
    index = shr(index, 1);
    i = index & 7;                                       
    i = dgray[i];                                        

    i = add(i, shl(i, 2));        /* pos3 =i*5+3+j */
    i = add(i, 3);
    pos[3] = add(i, j);                                  

    /* decode the signs  and build the codeword */

    for (i = 0; i < L_SUBFR; i++) {
        cod[i] = 0;                                      
    }

    for (j = 0; j < NB_PULSE4; j++) {
        i = sign & 1;                                    
        sign = shr(sign, 1);

        
        if (i != 0) {
            cod[pos[j]] = 8191;                          
        } else {
            cod[pos[j]] = -8192;                         
        }
    }

    return;
}
