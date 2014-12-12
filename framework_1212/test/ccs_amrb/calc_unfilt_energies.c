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

/*************************************************************************
 *
 * FUNCTION: calc_unfilt_energies
 *
 * PURPOSE:  calculation of several energy coefficients for unfiltered
 *           excitation signals and the LTP coding gain
 *
 *       frac_en[0]*2^exp_en[0] = <res res>   // LP residual energy
 *       frac_en[1]*2^exp_en[1] = <exc exc>   // LTP residual energy
 *       frac_en[2]*2^exp_en[2] = <exc code>  // LTP/CB innovation dot product
 *       frac_en[3]*2^exp_en[3] = <lres lres> // LTP residual energy
 *                                            // (lres = res - gain_pit*exc)
 *       ltpg = log2(LP_res_en / LTP_res_en)
 *
 *************************************************************************/
void calc_unfilt_energies(
    Word16 res[],     /* i  : LP residual,                               Q0  */
    Word16 exc[],     /* i  : LTP excitation (unfiltered),               Q0  */
    Word16 code[],    /* i  : CB innovation (unfiltered),                Q13 */
    Word16 gain_pit,  /* i  : pitch gain,                                Q14 */
    Word16 L_subfr,   /* i  : Subframe length                                */

    Word16 frac_en[], /* o  : energy coefficients (4), fraction part,    Q15 */
    Word16 exp_en[],  /* o  : energy coefficients (4), exponent part,    Q0  */
    Word16 *ltpg      /* o  : LTP coding gain (log2()),                  Q13 */
)
{
    Word32 s, L_temp;
    Word16 i, exp, tmp;
    Word16 ltp_res_en, pred_gain;
    Word16 ltpg_exp, ltpg_frac;

    /* Compute residual energy */
    s = L_mac((Word32) 0, res[0], res[0]);
    for (i = 1; i < L_subfr; i++)
        s = L_mac(s, res[i], res[i]);

    /* ResEn := 0 if ResEn < 200.0 (= 400 Q1) */
    
    if (s < 400L)
    {
        frac_en[0] = 0;                      
        exp_en[0] = -15;                     
    }
    else
    {
        exp = norm_l(s);
        frac_en[0] = extract_h(L_shl(s, exp));   
        exp_en[0] = 15 - exp;                
    }
    
    /* Compute ltp excitation energy */
    s = L_mac((Word32) 0, exc[0], exc[0]);
    for (i = 1; i < L_subfr; i++)
        s = L_mac(s, exc[i], exc[i]);

    exp = norm_l(s);
    frac_en[1] = extract_h(L_shl(s, exp));   
    exp_en[1] = 15 - exp;                

    /* Compute scalar product <exc[],code[]> */
    s = L_mac((Word32) 0, exc[0], code[0]);
    for (i = 1; i < L_subfr; i++)
        s = L_mac(s, exc[i], code[i]);

    exp = norm_l(s);
    frac_en[2] = extract_h(L_shl(s, exp));   
    exp_en[2] = 16-14 - exp;             

    /* Compute energy of LTP residual */
    s = 0L;                                  
    for (i = 0; i < L_subfr; i++)
    {
        L_temp = L_mult(exc[i], gain_pit);
        L_temp = L_shl(L_temp, 1);
        tmp = sub(res[i], round(L_temp));           /* LTP residual, Q0 */
        s = L_mac (s, tmp, tmp);
    }

    exp = norm_l(s);
    ltp_res_en = extract_h (L_shl (s, exp));
    exp = 15 - exp;

    frac_en[3] = ltp_res_en;                 
    exp_en[3] = exp;                         
    
    /* calculate LTP coding gain, i.e. energy reduction LP res -> LTP res */
     
    if (ltp_res_en > 0 && frac_en[0] != 0)
    {
        /* gain = ResEn / LTPResEn */
        pred_gain = div_s (shr (frac_en[0], 1), ltp_res_en);
        exp = sub (exp, exp_en[0]);

        /* L_temp = ltpGain * 2^(30 + exp) */
        L_temp = L_deposit_h (pred_gain);
        /* L_temp = ltpGain * 2^27 */
        L_temp = L_shr (L_temp, add (exp, 3));

        /* Log2 = log2() + 27 */
        Log2(L_temp, &ltpg_exp, &ltpg_frac);

        /* ltpg = log2(LtpGain) * 2^13 --> range: +- 4 = +- 12 dB */
        L_temp = L_Comp (sub (ltpg_exp, 27), ltpg_frac);
        *ltpg = round (L_shl (L_temp, 13)); /* Q13 */
    }
    else
    {
        *ltpg = 0;                           
    }
}
