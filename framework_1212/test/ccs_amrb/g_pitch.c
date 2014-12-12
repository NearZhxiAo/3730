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
#include "mode.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
 
/*************************************************************************
 *
 *  FUNCTION:  G_pitch
 *
 *  PURPOSE:  Compute the pitch (adaptive codebook) gain.
 *            Result in Q14 (NOTE: 12.2 bit exact using Q12) 
 *
 *  DESCRIPTION:
 *      The adaptive codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered adaptive
 *      codevector, and <> denotes dot product.
 *      The gain is limited to the range [0,1.2] (=0..19661 Q14)
 *
 *************************************************************************/
Word16 G_pitch     (    /* o : Gain of pitch lag saturated to 1.2       */
    enum Mode mode,     /* i : AMR mode                                 */
    Word16 xn[],        /* i : Pitch target.                            */
    Word16 y1[],        /* i : Filtered adaptive codebook.              */
    Word16 g_coeff[],   /* i : Correlations need for gain quantization  */
    Word16 L_subfr      /* i : Length of subframe.                      */
)
{
    Word16 i;
    Word16 xy, yy, exp_xy, exp_yy, gain;
    Word32 s;

    Word16 scaled_y1[L_SUBFR];   /* Usually dynamic allocation of (L_subfr) */

    /* divide "y1[]" by 4 to avoid cgOverflow */

    for (i = 0; i < L_subfr; i++)
    {
        scaled_y1[i] = shr (y1[i], 2);  
    }

    /* Compute scalar product <y1[],y1[]> */

    /* Q12 scaling / MR122 */
    cgOverflow = 0;                   
    s = 1L;                          /* Avoid case of all zeros */
    for (i = 0; i < L_subfr; i++)
    {
        s = L_mac (s, y1[i], y1[i]);
    }
     
    if (cgOverflow == 0)       /* Test for cgOverflow */
    {
        exp_yy = norm_l (s);
        yy = round (L_shl (s, exp_yy));
    }
    else
    {
        s = 1L;                      /* Avoid case of all zeros */
        for (i = 0; i < L_subfr; i++)
        {
            s = L_mac (s, scaled_y1[i], scaled_y1[i]);
        }
        exp_yy = norm_l (s);
        yy = round (L_shl (s, exp_yy));
        exp_yy = sub (exp_yy, 4);
    }
        
    /* Compute scalar product <xn[],y1[]> */
        
    cgOverflow = 0;                    
    s = 1L;                          /* Avoid case of all zeros */
        
    for (i = 0; i < L_subfr; i++)
    {
        s = L_mac(s, xn[i], y1[i]);
    }
     
    if (cgOverflow == 0)
    {
        exp_xy = norm_l (s);
        xy = round (L_shl (s, exp_xy));
    }
    else
    {
        s = 1L;                      /* Avoid case of all zeros */
        for (i = 0; i < L_subfr; i++)
        {
            s = L_mac (s, xn[i], scaled_y1[i]);
        }
        exp_xy = norm_l (s);
        xy = round (L_shl (s, exp_xy));
        exp_xy = sub (exp_xy, 2);
    }

    g_coeff[0] = yy;                 
    g_coeff[1] = sub (15, exp_yy);   
    g_coeff[2] = xy;                 
    g_coeff[3] = sub (15, exp_xy);   
    
    /* If (xy < 4) gain = 0 */

    i = sub (xy, 4);

     
    if (i < 0)
        return ((Word16) 0);

    /* compute gain = xy/yy */

    xy = shr (xy, 1);                  /* Be sure xy < yy */
    gain = div_s (xy, yy);

    i = sub (exp_xy, exp_yy);      /* Denormalization of division */        
    gain = shr (gain, i);
    
    /* if(gain >1.2) gain = 1.2 */
    
     
    if (sub (gain, 19661) > 0)
    {
        gain = 19661;                    
    }

    
    if (sub(mode, MR122) == 0)
    {
       /* clear 2 LSBits */
       gain = gain & 0xfffC;            
    }
    
    return (gain);
}
