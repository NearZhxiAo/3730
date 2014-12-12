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
#include "decompress_code.h"

#define NB_PULSE8  8           /* number of pulses  */

/* define values/representation for output codevector and sign */
#define POS_CODE  8191 
#define NEG_CODE  8191 

/*************************************************************************
 *
 *  FUNCTION:   dec_8i40_31bits()
 *
 *  PURPOSE:  Builds the innovative codevector from the received
 *            index of algebraic codebook.
 *
 *   See  c8_31pf.c  for more details about the algebraic codebook structure.
 *
 *************************************************************************/

void dec_8i40_31bits (
    Word16 index[],    /* i : index of 8 pulses (sign+position)         */
    Word16 cod[]       /* o : algebraic (fixed) codebook excitation     */
)
{
    Word16 i, j, pos1, pos2, sign;
    Word16 linear_signs[NB_TRACK_MR102];
    Word16 linear_codewords[NB_PULSE8];
    
    for (i = 0; i < L_CODE; i++)
    {
        cod[i] = 0;                                     
    }
    
    decompress_code (index, linear_signs, linear_codewords);
    
    /* decode the positions and signs of pulses and build the codeword */

    for (j = 0; j < NB_TRACK_MR102; j++)
    {
       /* compute index i */
       
       i = linear_codewords[j];
       i = extract_l (L_shr (L_mult (i, 4), 1));
       pos1 = add (i, j);   /* position of pulse "j" */
       
        
       if (linear_signs[j] == 0)
       {
          sign = POS_CODE;                              /* +1.0 */
       }
       else
       {
          sign = -NEG_CODE;                             /* -1.0 */
       }
       
       cod[pos1] = sign;                                
       
       /* compute index i */
       
       i = linear_codewords[add (j, 4)];        
       i = extract_l (L_shr (L_mult (i, 4), 1));
       pos2 = add (i, j);      /* position of pulse "j+4" */
       
        
       if (sub (pos2, pos1) < 0)
       {
          sign = negate (sign);
       }
       cod[pos2] = add (cod[pos2], sign);               
    }
    
    return;
}
