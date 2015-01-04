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
#include "log2.h"
#include "mode.h"

/*************************************************************************
 *
 * FUNCTION: calc_filt_energies
 *
 * PURPOSE:  calculation of several energy coefficients for filtered
 *           excitation signals
 *
 *     Compute coefficients need for the quantization and the optimum
 *     codebook gain gcu (for MR475 only).
 *
 *      coeff[0] =    y1 y1
 *      coeff[1] = -2 xn y1
 *      coeff[2] =    y2 y2
 *      coeff[3] = -2 xn y2
 *      coeff[4] =  2 y1 y2
 *
 *
 *      gcu = <xn2, y2> / <y2, y2> (0 if <xn2, y2> <= 0)
 *
 *     Product <y1 y1> and <xn y1> have been computed in G_pitch() and
 *     are in vector g_coeff[].
 *
 *************************************************************************/
void
calc_filt_energies(
    enum Mode mode,     /* i  : coder mode                                   */
    Word16 xn[],        /* i  : LTP target vector,                       Q0  */
    Word16 xn2[],       /* i  : CB target vector,                        Q0  */
    Word16 y1[],        /* i  : Adaptive codebook,                       Q0  */
    Word16 Y2[],        /* i  : Filtered innovative vector,              Q12 */
    Word16 g_coeff[],   /* i  : Correlations <xn y1> <y1 y1>                 */
                        /*      computed in G_pitch()                        */

    Word16 frac_coeff[],/* o  : energy coefficients (5), fraction part,  Q15 */
    Word16 exp_coeff[], /* o  : energy coefficients (5), exponent part,  Q0  */
    Word16 *cod_gain_frac,/* o: optimum codebook gain (fraction part),   Q15 */
    Word16 *cod_gain_exp  /* o: optimum codebook gain (exponent part),   Q0  */
)
{
    Word32 s, ener_init;
    Word16 i, exp, frac;
    Word16 y2[L_SUBFR];

    if ((mode == MR795) || (mode == MR475))
    {
        ener_init = 0L; 
    }
    else
    {
        ener_init = 1L; 
    }
    
    for (i = 0; i < L_SUBFR; i++) 
	{
        y2[i] = Y2[i] >> 3;          
    }

    frac_coeff[0] = g_coeff[0];          
    exp_coeff[0] = g_coeff[1];           
    frac_coeff[1] = negate(g_coeff[2]);     /* coeff[1] = -2 xn y1 */
    exp_coeff[1] = add(g_coeff[3], 1);   


    /* Compute scalar product <y2[],y2[]> */

    s = L_mac(ener_init, y2[0], y2[0]);
    for (i = 1; i < L_SUBFR; i++)
        s = L_mac(s, y2[i], y2[i]);

    exp = norm_l(s);
    frac_coeff[2] = extract_h(L_shl(s, exp)); 
    exp_coeff[2] = 15 - 18 - exp;    

    /* Compute scalar product -2*<xn[],y2[]> */

    s = L_mac(ener_init, xn[0], y2[0]);
    for (i = 1; i < L_SUBFR; i++)
        s = L_mac(s, xn[i], y2[i]);

    exp = norm_l(s);
    frac_coeff[3] = negate(extract_h(L_shl(s, exp))); 
    exp_coeff[3] = sub(15 - 9 + 1, exp);         


    /* Compute scalar product 2*<y1[],y2[]> */

    s = L_mac(ener_init, y1[0], y2[0]);
    for (i = 1; i < L_SUBFR; i++)
        s = L_mac(s, y1[i], y2[i]);

    exp = norm_l(s);
    frac_coeff[4] = extract_h(L_shl(s, exp)); 
    exp_coeff[4] = 15 - 9 + 1 - exp;  

    if ((mode == MR475) || (mode == MR795))
    {
        /* Compute scalar product <xn2[],y2[]> */

        s = L_mac(ener_init, xn2[0], y2[0]);
        for (i = 1; i < L_SUBFR; i++)
            s = L_mac(s, xn2[i], y2[i]);
        
        exp = norm_l(s);
        frac = extract_h(L_shl(s, exp));
        exp = 15 - 9 - exp;

        
        if ( frac <= 0)
        {
            *cod_gain_frac = 0; 
            *cod_gain_exp = 0;  
        }
        else
        {
            /*
              gcu = <xn2, y2> / c[2]
                  = (frac>>1)/frac[2]             * 2^(exp+1-exp[2])
                  = div_s(frac>>1, frac[2])*2^-15 * 2^(exp+1-exp[2])
                  = div_s * 2^(exp-exp[2]-14)
             */  
            *cod_gain_frac = div_s (shr (frac,1), frac_coeff[2]); 
            *cod_gain_exp = sub (sub (exp, exp_coeff[2]), 14);    

        }
    }
}
