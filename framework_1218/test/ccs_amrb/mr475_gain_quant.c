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
#include "mac_32.h"
#include "mode.h"
#include "cnst.h"
#include "pow2.h"
#include "log2.h"
#include "gc_pred_update.h"
#include "mr475_quant_store_results.h"
/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/* minimum allowed gain code prediction error: 102.887/4096 = 0.0251189 */
#define MIN_QUA_ENER         ( -5443) /* Q10 <->    log2 (0.0251189) */
#define MIN_QUA_ENER_MR122   (-32768) /* Q10 <-> 20*log10(0.0251189) */

/* minimum allowed gain code prediction error: 32000/4096 = 7.8125 */
#define MAX_QUA_ENER         (  3037) /* Q10 <->    log2 (7.8125)    */
#define MAX_QUA_ENER_MR122   ( 18284) /* Q10 <-> 20*log10(7.8125)    */


#include "table.h"

/*************************************************************************
 *
 * FUNCTION:  MR475_gain_quant()
 *
 * PURPOSE: Quantization of pitch and codebook gains for two subframes
 *          (using predicted codebook gain)
 *
 *************************************************************************/
Word16
MR475_gain_quant(              /* o  : index of quantization.                 */
    gc_predState *pred_st,     /* i/o: gain predictor state struct            */

                               /* data from subframe 0 (or 2) */
    Word16 sf0_exp_gcode0,     /* i  : predicted CB gain (exponent),      Q0  */
    Word16 sf0_frac_gcode0,    /* i  : predicted CB gain (fraction),      Q15 */
    Word16 sf0_exp_coeff[],    /* i  : energy coeff. (5), exponent part,  Q0  */
    Word16 sf0_frac_coeff[],   /* i  : energy coeff. (5), fraction part,  Q15 */
                               /*      (frac_coeff and exp_coeff computed in  */
                               /*       calc_filt_energies())                 */
    Word16 sf0_exp_target_en,  /* i  : exponent of target energy,         Q0  */
    Word16 sf0_frac_target_en, /* i  : fraction of target energy,         Q15 */

                               /* data from subframe 1 (or 3) */
    Word16 sf1_code_nosharp[], /* i  : innovative codebook vector (L_SUBFR)   */
                               /*      (whithout pitch sharpening)            */
    Word16 sf1_exp_gcode0,     /* i  : predicted CB gain (exponent),      Q0  */
    Word16 sf1_frac_gcode0,    /* i  : predicted CB gain (fraction),      Q15 */
    Word16 sf1_exp_coeff[],    /* i  : energy coeff. (5), exponent part,  Q0  */
    Word16 sf1_frac_coeff[],   /* i  : energy coeff. (5), fraction part,  Q15 */
                               /*      (frac_coeff and exp_coeff computed in  */
                               /*       calc_filt_energies())                 */
    Word16 sf1_exp_target_en,  /* i  : exponent of target energy,         Q0  */
    Word16 sf1_frac_target_en, /* i  : fraction of target energy,         Q15 */

    Word16 gp_limit,           /* i  : pitch gain limit                       */

    Word16 *sf0_gain_pit,      /* o  : Pitch gain,                        Q14 */
    Word16 *sf0_gain_cod,      /* o  : Code gain,                         Q1  */

    Word16 *sf1_gain_pit,      /* o  : Pitch gain,                        Q14 */
    Word16 *sf1_gain_cod       /* o  : Code gain,                         Q1  */
)
{
    const Word16 *p;
    Word16 i, index = 0;
    Word16 tmp;
    Word16 exp;
    Word16 sf0_gcode0, sf1_gcode0;
    Word16 g_pitch, g2_pitch, g_code, g2_code, g_pit_cod;
    Word16 coeff[10], coeff_lo[10], exp_max[10];  /* 0..4: sf0; 5..9: sf1 */
    Word32 L_tmp, dist_min;

    /*-------------------------------------------------------------------*
     *  predicted codebook gain                                          *
     *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
     *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
     *                                                                   *
     *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
     *-------------------------------------------------------------------*/

    sf0_gcode0 = extract_l(Pow2(14, sf0_frac_gcode0));
    sf1_gcode0 = extract_l(Pow2(14, sf1_frac_gcode0));

    /*
     * For each subframe, the error energy (sum) to be minimized consists
     * of five terms, t[0..4].
     *
     *                      t[0] =    gp^2  * <y1 y1>
     *                      t[1] = -2*gp    * <xn y1>
     *                      t[2] =    gc^2  * <y2 y2>
     *                      t[3] = -2*gc    * <xn y2>
     *                      t[4] =  2*gp*gc * <y1 y2>
     *
     */

    /* sf 0 */
    /* determine the scaling exponent for g_code: ec = ec0 - 11 */
    exp = sub(sf0_exp_gcode0, 11);

    /* calculate exp_max[i] = s[i]-1 */
    exp_max[0] = sub(sf0_exp_coeff[0], 13);                        
    exp_max[1] = sub(sf0_exp_coeff[1], 14);                        
    exp_max[2] = add(sf0_exp_coeff[2], add(15, shl(exp, 1)));      
    exp_max[3] = add(sf0_exp_coeff[3], exp);                       
    exp_max[4] = add(sf0_exp_coeff[4], add(1, exp));               

    /* sf 1 */
    /* determine the scaling exponent for g_code: ec = ec0 - 11 */
    exp = sub(sf1_exp_gcode0, 11);

    /* calculate exp_max[i] = s[i]-1 */
    exp_max[5] = sub(sf1_exp_coeff[0], 13);                        
    exp_max[6] = sub(sf1_exp_coeff[1], 14);                        
    exp_max[7] = add(sf1_exp_coeff[2], add(15, shl(exp, 1)));      
    exp_max[8] = add(sf1_exp_coeff[3], exp);                       
    exp_max[9] = add(sf1_exp_coeff[4], add(1, exp));               



    /*-------------------------------------------------------------------*
     *  Gain search equalisation:                                        *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~                                        *
     *  The MSE for the two subframes is weighted differently if there   *
     *  is a big difference in the corresponding target energies         *
     *-------------------------------------------------------------------*/

    /* make the target energy exponents the same by de-normalizing the
       fraction of the smaller one. This is necessary to be able to compare
       them
     */
    exp = sf0_exp_target_en - sf1_exp_target_en;
    
    if (exp > 0)
    {
        sf1_frac_target_en = shr (sf1_frac_target_en, exp);
    }
    else
    {
        sf0_frac_target_en = shl (sf0_frac_target_en, exp);
    }

    /* assume no change of exponents */
    exp = 0; 

    /* test for target energy difference; set exp to +1 or -1 to scale
     * up/down coefficients for sf 1
     */
    tmp = shr_r (sf1_frac_target_en, 1);   /* tmp = ceil(0.5*en(sf1)) */
    
    if (sub (tmp, sf0_frac_target_en) > 0) /* tmp > en(sf0)? */
    {
        /*
         * target_energy(sf1) > 2*target_energy(sf0)
         *   -> scale up MSE(sf0) by 2 by adding 1 to exponents 0..4
         */
        exp = 1; 
    }
    else
    {
        tmp = shr (add (sf0_frac_target_en, 3), 2); /* tmp=ceil(0.25*en(sf0)) */
        
        if (sub (tmp, sf1_frac_target_en) > 0)      /* tmp > en(sf1)? */
        {
            /*
             * target_energy(sf1) < 0.25*target_energy(sf0)
             *   -> scale down MSE(sf0) by 0.5 by subtracting 1 from
             *      coefficients 0..4
             */
            exp = -1; 
        }
    }
    
    for (i = 0; i < 5; i++)
    {
        exp_max[i] = add (exp_max[i], exp); 
    }
                                                                       
    /*-------------------------------------------------------------------*
     *  Find maximum exponent:                                           *
     *  ~~~~~~~~~~~~~~~~~~~~~~                                           *
     *                                                                   *
     *  For the sum operation, all terms must have the same scaling;     *
     *  that scaling should be low enough to prevent cgOverflow. There-    *
     *  fore, the maximum scale is determined and all coefficients are   *
     *  re-scaled:                                                       *
     *                                                                   *
     *    exp = max(exp_max[i]) + 1;                                     *
     *    e = exp_max[i]-exp;         e <= 0!                            *
     *    c[i] = c[i]*2^e                                                *
     *-------------------------------------------------------------------*/

    exp = exp_max[0];                                        
    for (i = 1; i < 10; i++)
    {
         
        if (sub(exp_max[i], exp) > 0)
        {
            exp = exp_max[i];                                
        }
    }
    exp = add(exp, 1);      /* To avoid cgOverflow */

    p = &sf0_frac_coeff[0]; 
    for (i = 0; i < 5; i++) {
        tmp = sub(exp, exp_max[i]);
        L_tmp = L_deposit_h(*p++);
        L_tmp = L_shr(L_tmp, tmp);
        L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
    }
    p = &sf1_frac_coeff[0]; 
    for (; i < 10; i++) {
        tmp = sub(exp, exp_max[i]);
        L_tmp = L_deposit_h(*p++);
        L_tmp = L_shr(L_tmp, tmp);
        L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
    }


    /*-------------------------------------------------------------------*
     *  Codebook search:                                                 *
     *  ~~~~~~~~~~~~~~~~                                                 *
     *                                                                   *
     *  For each pair (g_pitch, g_fac) in the table calculate the        *
     *  terms t[0..4] and sum them up; the result is the mean squared    *
     *  error for the quantized gains from the table. The index for the  *
     *  minimum MSE is stored and finally used to retrieve the quantized *
     *  gains                                                            *
     *-------------------------------------------------------------------*/

    /* start with "infinite" MSE */
    dist_min = MAX_32;        

    p = &table_gain_MR475[0]; 

    for (i = 0; i < MR475_VQ_SIZE; i++)
    {
        /* subframe 0 (and 2) calculations */
        g_pitch = *p++;       
        g_code = *p++;        

        g_code = mult(g_code, sf0_gcode0);
        g2_pitch = mult(g_pitch, g_pitch);
        g2_code = mult(g_code, g_code);
        g_pit_cod = mult(g_code, g_pitch);
        
        L_tmp = Mpy_32_16(       coeff[0], coeff_lo[0], g2_pitch);
        L_tmp = Mac_32_16(L_tmp, coeff[1], coeff_lo[1], g_pitch);
        L_tmp = Mac_32_16(L_tmp, coeff[2], coeff_lo[2], g2_code);
        L_tmp = Mac_32_16(L_tmp, coeff[3], coeff_lo[3], g_code);
        L_tmp = Mac_32_16(L_tmp, coeff[4], coeff_lo[4], g_pit_cod);

        tmp = sub (g_pitch, gp_limit);

        /* subframe 1 (and 3) calculations */
        g_pitch = *p++;      
        g_code = *p++;       

          
        if (tmp <= 0 && sub(g_pitch, gp_limit) <= 0)
        {
            g_code = mult(g_code, sf1_gcode0);
            g2_pitch = mult(g_pitch, g_pitch);
            g2_code = mult(g_code, g_code);
            g_pit_cod = mult(g_code, g_pitch);
            
            L_tmp = Mac_32_16(L_tmp, coeff[5], coeff_lo[5], g2_pitch);
            L_tmp = Mac_32_16(L_tmp, coeff[6], coeff_lo[6], g_pitch);
            L_tmp = Mac_32_16(L_tmp, coeff[7], coeff_lo[7], g2_code);
            L_tmp = Mac_32_16(L_tmp, coeff[8], coeff_lo[8], g_code);
            L_tmp = Mac_32_16(L_tmp, coeff[9], coeff_lo[9], g_pit_cod);
            
            /* store table index if MSE for this index is lower
               than the minimum MSE seen so far */
            
            if (L_sub(L_tmp, dist_min) < (Word32) 0)
            {
                dist_min = L_tmp; 
                index = i;        
            }
        }
    }

    /*------------------------------------------------------------------*
     *  read quantized gains and update MA predictor memories           *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           *
     *------------------------------------------------------------------*/

    /* for subframe 0, the pre-calculated gcode0/exp_gcode0 are the same
       as those calculated from the "real" predictor using quantized gains */
    tmp = shl(index, 2);
    MR475_quant_store_results(pred_st,
                              &table_gain_MR475[tmp],
                              sf0_gcode0,
                              sf0_exp_gcode0,
                              sf0_gain_pit,
                              sf0_gain_cod);

    /* calculate new predicted gain for subframe 1 (this time using
       the real, quantized gains)                                   */
    gc_pred(pred_st, MR475, sf1_code_nosharp,
            &sf1_exp_gcode0, &sf1_frac_gcode0,
            &sf0_exp_gcode0, &sf0_gcode0); /* last two args are dummy */
    sf1_gcode0 = extract_l(Pow2(14, sf1_frac_gcode0));


    tmp = add (tmp, 2);
    MR475_quant_store_results(pred_st,
                              &table_gain_MR475[tmp],
                              sf1_gcode0,
                              sf1_exp_gcode0,
                              sf1_gain_pit,
                              sf1_gain_cod);

    return index;
}
