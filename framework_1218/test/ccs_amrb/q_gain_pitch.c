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
#include "mode.h"
 
#include "table.h"
 
Word16 q_gain_pitch (   /* Return index of quantization                      */
    enum Mode mode,     /* i  : AMR mode                                     */
    Word16 gp_limit,    /* i  : pitch gain limit                             */
    Word16 *gain,       /* i/o: Pitch gain (unquant/quant),              Q14 */
    Word16 gain_cand[], /* o  : pitch gain candidates (3),   MR795 only, Q14 */ 
    Word16 gain_cind[]  /* o  : pitch gain cand. indices (3),MR795 only, Q0  */ 
)
{
    Word16 i, index, err, err_min;

    err_min = abs_s (sub (*gain, qua_gain_pitch[0]));
    index = 0;                                               

    for (i = 1; i < NB_QUA_PITCH; i++)
    {
        
        if (qua_gain_pitch[i] <= gp_limit)
        {
            err = abs_s (sub (*gain, qua_gain_pitch[i]));
            
             
            if (err < err_min)
            {
                err_min = err;                                   
                index = i;                                       
            }
        }
    }

    
    if (mode == MR795)
    {
        /* in MR795 mode, compute three gain_pit candidates around the index
         * found in the quantization loop: the index found and the two direct
         * neighbours, except for the extreme cases (i=0 or i=NB_QUA_PITCH-1),
         * where the direct neighbour and the neighbour to that is used.
         */
        Word16 ii;

        
        if (index == 0)
        {
            ii = index;                                     
        }
        else
        {
             
            if ( (index == NB_QUA_PITCH-1)|| (qua_gain_pitch[index+1] > gp_limit))
            {
                ii = index - 2;
            }
            else
            {
                ii = index - 1;
            }
        }

        /* store candidate indices and values */
        for (i = 0; i < 3; i++)
        {
            gain_cind[i] = ii;                              
            gain_cand[i] = qua_gain_pitch[ii];              
            ii = ii + 1;
        }
        
        *gain = qua_gain_pitch[index];                       
    }
    else
    {
        /* in MR122 mode, just return the index and gain pitch found.
         * If bitexactness is required, mask away the two LSBs (because
         * in the original EFR, gain_pit was scaled Q12)
         */
       
       if (mode == MR122)
       {
          /* clear 2 LSBits */
          *gain = qua_gain_pitch[index] & 0xFFFC;  
       }
       else
       {
          *gain = qua_gain_pitch[index];                       
       }
    }
    return index;
}
