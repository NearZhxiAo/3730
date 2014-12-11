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
#include "table.h"

#define NB_PULSE10  10            /* number of pulses  */

/*************************************************************************
 *
 *  FUNCTION:   dec_10i40_35bits()
 *
 *  PURPOSE:  Builds the innovative codevector from the received
 *            index of algebraic codebook.
 *
 *   See  c1035pf.c  for more details about the algebraic codebook structure.
 *
 *************************************************************************/
void dec_10i40_35bits (
    Word16 index[],    /* (i)     : index of 10 pulses (sign+position)       */
    Word16 cod[]       /* (o)     : algebraic (fixed) codebook excitation    */
)
{
    Word16 i, j, pos1, pos2, sign, tmp;

    for (i = 0; i < L_CODE; i++)
    {
        cod[i] = 0;                                      
    }

    /* decode the positions and signs of pulses and build the codeword */

    for (j = 0; j < NB_TRACK; j++)
    {
        /* compute index i */

        tmp = index[j];                                 
        i = tmp & 7;                                     
        i = dgray[i];                                    

        i = extract_l (L_shr (L_mult (i, 5), 1));
        pos1 = add (i, j); /* position of pulse "j" */

        i = shr (tmp, 3) & 1;                            
         
        if (i == 0)
        {
            sign = 4096;                                 /* +1.0 */
        }
        else
        {
            sign = -4096;                                /* -1.0 */
        }

        cod[pos1] = sign;                                

        /* compute index i */

        i = index[add (j, 5)] & 7;                       
        i = dgray[i];                                    
        i = extract_l (L_shr (L_mult (i, 5), 1));

        pos2 = add (i, j);      /* position of pulse "j+5" */

         
        if (sub (pos2, pos1) < 0)
        {
            sign = negate (sign);
        }
        cod[pos2] = add (cod[pos2], sign);               
    }

    return;
}
