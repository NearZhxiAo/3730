/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "cnst.h"
#include "energy_old.h"

Word32 energy_new( /* o : return energy of signal     */
    Word16 in[],          /* i : input signal (length l_trm) */
    Word16 l_trm          /* i : signal length               */
)
{
    Word32 s;
    Word16 i;
    Flag ov_save;

    ov_save = cgOverflow;  /* save cgOverflow flag in case energy_old */
                                   /* must be called                        */
    s = L_mult(in[0], in[0]);
    for (i = 1; i < l_trm; i++)
    {
        s = L_mac(s, in[i], in[i]);
    }
    
    /* check for cgOverflow */
     
    if (L_sub (s, MAX_32) == 0L)
    {
        cgOverflow = ov_save;  /* restore cgOverflow flag */
        s = energy_old (in, l_trm);  /* function result */
    }
    else
    {
       s = L_shr(s, 4);
    }

    return s;
}
