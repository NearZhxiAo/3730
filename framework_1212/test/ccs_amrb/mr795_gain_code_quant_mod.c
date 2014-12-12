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
#include "pow2.h"
#include "sqrt_l_exp.h"
#include "table.h"
#include "mac_32.h"

#include "gain_adapt.h"
#include "q_gain_pitch.h"
#include "calc_unfilt_energies.h"


/*************************************************************************
 *
 * FUNCTION:  MR795_gain_code_quant_mod
 *
 * PURPOSE: Modified quantization of the MR795 codebook gain
 *
 * Uses pre-computed energy coefficients in frac_en[]/exp_en[]
 *
 *       frac_en[0]*2^exp_en[0] = <res res>   // LP residual energy
 *       frac_en[1]*2^exp_en[1] = <exc exc>   // LTP residual energy
 *       frac_en[2]*2^exp_en[2] = <exc code>  // LTP/CB innovation dot product
 *       frac_en[3]*2^exp_en[3] = <code code> // CB innovation energy
 *
 *************************************************************************/
Word16
MR795_gain_code_quant_mod(  /* o  : index of quantization.            */
    Word16 gain_pit,        /* i  : pitch gain,                   Q14 */
    Word16 exp_gcode0,      /* i  : predicted CB gain (exponent), Q0  */
    Word16 gcode0,          /* i  : predicted CB gain (norm.),    Q14 */
    Word16 frac_en[],       /* i  : energy coefficients (4),
                                    fraction part,                Q15 */
    Word16 exp_en[],        /* i  : energy coefficients (4),
                                    eponent part,                 Q0  */
    Word16 alpha,           /* i  : gain adaptor factor (>0),     Q15 */
    Word16 gain_cod_unq,    /* i  : Code gain (unquantized)           */
                            /*      (scaling: Q10 - exp_gcode0)       */
    Word16 *gain_cod,       /* i/o: Code gain (pre-/quantized),   Q1  */
    Word16 *qua_ener_MR122, /* o  : quantized energy error,       Q10 */
                            /*      (for MR122 MA predictor update)   */
    Word16 *qua_ener        /* o  : quantized energy error,       Q10 */
                            /*      (for other MA predictor update)   */
 )
{
    const Word16 *p;
    Word16 i, index, tmp;
    Word16 one_alpha;
    Word16 exp, e_max;
    Word16 g2_pitch, g_code;
    Word16 g2_code_h, g2_code_l;
    Word16 d2_code_h, d2_code_l;
    Word16 coeff[5], coeff_lo[5], exp_coeff[5];
    Word32 L_tmp, L_t0, L_t1, dist_min;
    Word16 gain_code;

    /*
      Steps in calculation of the error criterion (dist):
      ---------------------------------------------------

      underlined = constant; alp = FLP value of alpha, alpha = FIP
      ----------


        ExEn = gp^2 * LtpEn + 2.0*gp*gc[i] * XC + gc[i]^2 * InnEn;
               ------------   ------         --             -----

        aExEn= alp * ExEn
             = alp*gp^2*LtpEn + 2.0*alp*gp*XC* gc[i] + alp*InnEn* gc[i]^2
               --------------   -------------          ---------

             =         t[1]   +              t[2]    +          t[3]

        dist = d1 + d2;

          d1 = (1.0 - alp) * InnEn * (gcu - gc[i])^2 = t[4]
               -------------------    ---

          d2 =        alp  * (ResEn - 2.0 * sqrt(ResEn*ExEn) + ExEn);
                      ---     -----   ---        -----

             =        alp  * (sqrt(ExEn) - sqrt(ResEn))^2
                      ---                  -----------

             =               (sqrt(aExEn) - sqrt(alp*ResEn))^2
                                            ---------------

             =               (sqrt(aExEn) -       t[0]     )^2
                                                  ----

     */

    /*
     * calculate scalings of the constant terms
     */
    gain_code = shl (*gain_cod, sub (10, exp_gcode0)); /* Q1  -> Q11 (-ec0) */
    g2_pitch = mult (gain_pit, gain_pit);              /* Q14 -> Q13        */
    /* 0 < alpha <= 0.5 => 0.5 <= 1-alpha < 1, i.e one_alpha is normalized  */
    one_alpha = add (sub (32767, alpha), 1); /* 32768 - alpha */


    /*  alpha <= 0.5 -> mult. by 2 to keep precision; compensate in exponent */
    tmp = extract_h (L_shl (L_mult (alpha, frac_en[1]), 1));
    /* directly store in 32 bit variable because no further mult. required */
    L_t1 = L_mult (tmp, g2_pitch);                    
    exp_coeff[1] = sub (exp_en[1], 15);               


    tmp = extract_h (L_shl (L_mult (alpha, frac_en[2]), 1));
    coeff[2] = mult (tmp, gain_pit);                  
    exp = sub (exp_gcode0, 10);
    exp_coeff[2] = add (exp_en[2], exp);              


    /* alpha <= 0.5 -> mult. by 2 to keep precision; compensate in exponent */
    coeff[3] = extract_h (L_shl (L_mult (alpha, frac_en[3]), 1));
    exp = sub (shl (exp_gcode0, 1), 7);
    exp_coeff[3] = add (exp_en[3], exp);              


    coeff[4] = mult (one_alpha, frac_en[3]);          
    exp_coeff[4] = add (exp_coeff[3], 1);             


    L_tmp = L_mult (alpha, frac_en[0]);
    /* sqrt_l returns normalized value and 2*exponent
       -> result = val >> (exp/2)
       exp_coeff holds 2*exponent for c[0]            */
    /* directly store in 32 bit variable because no further mult. required */
    L_t0 = sqrt_l_exp (L_tmp, &exp); /* normalization included in sqrt_l_exp */
                                      /* function result */
    exp = add (exp, 47);
    exp_coeff[0] = sub (exp_en[0], exp);              

    /*
     * Determine the maximum exponent occuring in the distance calculation
     * and adjust all fractions accordingly (including a safety margin)
     *
     */

    /* find max(e[1..4],e[0]+31) */
    e_max = add (exp_coeff[0], 31);
    for (i = 1; i <= 4; i++)
    {
        
        if (sub (exp_coeff[i], e_max) > 0)
        {
            e_max = exp_coeff[i];                     
        }
    }

    /* scale c[1]         (requires no further multiplication) */
    tmp = sub (e_max, exp_coeff[1]);
    L_t1 = L_shr(L_t1, tmp);

    /* scale c[2..4] (used in Mpy_32_16 in the quantizer loop) */
    for (i = 2; i <= 4; i++)
    {
        tmp = sub (e_max, exp_coeff[i]);
        L_tmp = L_deposit_h(coeff[i]);
        L_tmp = L_shr(L_tmp, tmp);
        L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
    }

    /* scale c[0]         (requires no further multiplication) */
    exp = sub (e_max, 31);             /* new exponent */
    tmp = sub (exp, exp_coeff[0]);
    L_t0 = L_shr (L_t0, shr (tmp, 1));
    /* perform correction by 1/sqrt(2) if exponent difference is odd */
     
    if ((tmp & 0x1) != 0)
    {
        L_Extract(L_t0, &coeff[0], &coeff_lo[0]);
        L_t0 = Mpy_32_16(coeff[0], coeff_lo[0],
                         23170);                    /* 23170 Q15 = 1/sqrt(2)*/
    }

    /* search the quantizer table for the lowest value
       of the search criterion                           */
    dist_min = MAX_32; 
    index = 0;         
    p = &qua_gain_code[0]; 

    for (i = 0; i < NB_QUA_CODE; i++)
    {
        g_code = *p++;         /* this is g_fac (Q11)  */
        p++;                             /* skip log2(g_fac)     */
        p++;                             /* skip 20*log10(g_fac) */
        g_code = mult (g_code, gcode0);

        /* only continue if    gc[i]            < 2.0*gc
           which is equiv. to  g_code (Q10-ec0) < gain_code (Q11-ec0) */
        
        if (sub (g_code, gain_code) >= 0)
            break;

        L_tmp = L_mult (g_code, g_code);
        L_Extract (L_tmp, &g2_code_h, &g2_code_l);

        tmp = sub (g_code, gain_cod_unq);
        L_tmp = L_mult (tmp, tmp);
        L_Extract (L_tmp, &d2_code_h, &d2_code_l);

        /* t2, t3, t4 */
        L_tmp = Mac_32_16 (L_t1, coeff[2], coeff_lo[2], g_code);
        L_tmp = Mac_32(L_tmp,    coeff[3], coeff_lo[3], g2_code_h, g2_code_l);

        L_tmp = sqrt_l_exp (L_tmp, &exp);
        L_tmp = L_shr (L_tmp, shr (exp, 1));

        /* d2 */
        tmp = round (L_sub (L_tmp, L_t0));
        L_tmp = L_mult (tmp, tmp);

        /* dist */
        L_tmp = Mac_32(L_tmp, coeff[4], coeff_lo[4], d2_code_h, d2_code_l);

        /* store table index if distance measure for this
            index is lower than the minimum seen so far   */
        
        if (L_sub (L_tmp, dist_min) < (Word32) 0)
        {
            dist_min = L_tmp;                
            index = i;                       
        }
    }

    /*------------------------------------------------------------------*
     *  read quantized gains and new values for MA predictor memories   *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
     *------------------------------------------------------------------*/

    /* Read the quantized gains */
    p = &qua_gain_code[add (add (index, index), index)]; 
    g_code = *p++;            
    *qua_ener_MR122 = *p++;   
    *qua_ener = *p;           

    /*------------------------------------------------------------------*
     *  calculate final fixed codebook gain:                            *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
     *                                                                  *
     *   gc = gc0 * g                                                   *
     *------------------------------------------------------------------*/

    L_tmp = L_mult(g_code, gcode0);
    L_tmp = L_shr(L_tmp, sub(9, exp_gcode0));
    *gain_cod = extract_h(L_tmp);

    return index;
}

