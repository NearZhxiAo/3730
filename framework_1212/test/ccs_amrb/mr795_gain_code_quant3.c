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
 * FUNCTION:  MR795_gain_code_quant3
 *
 * PURPOSE: Pre-quantization of codebook gains, given three possible
 *          LTP gains (using predicted codebook gain)
 *
 *************************************************************************/
void
MR795_gain_code_quant3(
    Word16 exp_gcode0,        /* i  : predicted CB gain (exponent), Q0  */
    Word16 gcode0,            /* i  : predicted CB gain (norm.),    Q14 */
    Word16 g_pitch_cand[],    /* i  : Pitch gain candidates (3),    Q14 */
    Word16 g_pitch_cind[],    /* i  : Pitch gain cand. indices (3), Q0  */
    Word16 frac_coeff[],      /* i  : coefficients (5),             Q15 */
    Word16 exp_coeff[],       /* i  : energy coefficients (5),      Q0  */
                              /*      coefficients from calc_filt_ener()*/
    Word16 *gain_pit,         /* o  : Pitch gain,                   Q14 */
    Word16 *gain_pit_ind,     /* o  : Pitch gain index,             Q0  */
    Word16 *gain_cod,         /* o  : Code gain,                    Q1  */
    Word16 *gain_cod_ind,     /* o  : Code gain index,              Q0  */
    Word16 *qua_ener_MR122,   /* o  : quantized energy error,       Q10 */
                              /*      (for MR122 MA predictor update)   */
    Word16 *qua_ener          /* o  : quantized energy error,       Q10 */
                              /*      (for other MA predictor update)   */
)
{
    const Word16 *p;
    Word16 i, j, cod_ind, pit_ind;
    Word16 e_max, exp_code;
    Word16 g_pitch, g2_pitch, g_code, g2_code_h, g2_code_l;
    Word16 g_pit_cod_h, g_pit_cod_l;
    Word16 coeff[5], coeff_lo[5];
    Word16 exp_max[5];
    Word32 L_tmp, L_tmp0, dist_min;

    /*
     * The error energy (sum) to be minimized consists of five terms, t[0..4].
     *
     *                      t[0] =    gp^2  * <y1 y1>
     *                      t[1] = -2*gp    * <xn y1>
     *                      t[2] =    gc^2  * <y2 y2>
     *                      t[3] = -2*gc    * <xn y2>
     *                      t[4] =  2*gp*gc * <y1 y2>
     *
     */

    /* determine the scaling exponent for g_code: ec = ec0 - 10 */
    exp_code = sub(exp_gcode0, 10);

    /* calculate exp_max[i] = s[i]-1 */
    exp_max[0] = sub(exp_coeff[0], 13);                        
    exp_max[1] = sub(exp_coeff[1], 14);                        
    exp_max[2] = add(exp_coeff[2], add(15, shl(exp_code, 1))); 
    exp_max[3] = add(exp_coeff[3], exp_code);                  
    exp_max[4] = add(exp_coeff[4], add(exp_code,1));           


    /*-------------------------------------------------------------------*
     *  Find maximum exponent:                                           *
     *  ~~~~~~~~~~~~~~~~~~~~~~                                           *
     *                                                                   *
     *  For the sum operation, all terms must have the same scaling;     *
     *  that scaling should be low enough to prevent cgOverflow. There-    *
     *  fore, the maximum scale is determined and all coefficients are   *
     *  re-scaled:                                                       *
     *                                                                   *
     *    e_max = max(exp_max[i]) + 1;                                   *
     *    e = exp_max[i]-e_max;         e <= 0!                          *
     *    c[i] = c[i]*2^e                                                *
     *-------------------------------------------------------------------*/

    e_max = exp_max[0];                                        
    for (i = 1; i < 5; i++)     /* implemented flattened */
    {
         
        if (sub(exp_max[i], e_max) > 0)
        {
            e_max = exp_max[i];                                
        }
    }

    e_max = add(e_max, 1);      /* To avoid cgOverflow */

    for (i = 0; i < 5; i++) {
        j = sub(e_max, exp_max[i]);
        L_tmp = L_deposit_h(frac_coeff[i]);
        L_tmp = L_shr(L_tmp, j);
        L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
    }


    /*-------------------------------------------------------------------*
     *  Codebook search:                                                 *
     *  ~~~~~~~~~~~~~~~~                                                 *
     *                                                                   *
     *  For each of the candiates LTP gains in g_pitch_cand[], the terms *
     *  t[0..4] are calculated from the values in the table (and the     *
     *  pitch gain candidate) and summed up; the result is the mean      *
     *  squared error for the LPT/CB gain pair. The index for the mini-  *
     *  mum MSE is stored and finally used to retrieve the quantized CB  *
     *  gain                                                             *
     *-------------------------------------------------------------------*/

    /* start with "infinite" MSE */
    dist_min = MAX_32;        
    cod_ind = 0;              
    pit_ind = 0;              

    /* loop through LTP gain candidates */
    for (j = 0; j < 3; j++)
    {
        /* pre-calculate terms only dependent on pitch gain */
        g_pitch = g_pitch_cand[j];    
        g2_pitch = mult(g_pitch, g_pitch);
        L_tmp0 = Mpy_32_16(        coeff[0], coeff_lo[0], g2_pitch);
        L_tmp0 = Mac_32_16(L_tmp0, coeff[1], coeff_lo[1], g_pitch);

        p = &qua_gain_code[0];
        for (i = 0; i < NB_QUA_CODE; i++)
        {
            g_code = *p++;         /* this is g_fac        Q11 */
            p++;                             /* skip log2(g_fac)         */
            p++;                             /* skip 20*log10(g_fac)     */

            g_code = mult(g_code, gcode0);

            L_tmp = L_mult (g_code, g_code);
            L_Extract (L_tmp, &g2_code_h, &g2_code_l);

            L_tmp = L_mult(g_code, g_pitch);
            L_Extract (L_tmp, &g_pit_cod_h, &g_pit_cod_l);

            L_tmp = Mac_32  (L_tmp0, coeff[2], coeff_lo[2],
                                     g2_code_h, g2_code_l);
            L_tmp = Mac_32_16(L_tmp, coeff[3], coeff_lo[3],
                                     g_code);
            L_tmp = Mac_32   (L_tmp, coeff[4], coeff_lo[4],
                                     g_pit_cod_h, g_pit_cod_l);

            /* store table index if MSE for this index is lower
               than the minimum MSE seen so far; also store the
               pitch gain for this (so far) lowest MSE          */
            
            if (L_sub(L_tmp, dist_min) < (Word32) 0)
            {
                dist_min = L_tmp;                
                cod_ind = i;                     
                pit_ind = j;                     
            }
        }
    }

    /*------------------------------------------------------------------*
     *  read quantized gains and new values for MA predictor memories   *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
     *------------------------------------------------------------------*/

    /* Read the quantized gains */
    p = &qua_gain_code[add (add (cod_ind, cod_ind), cod_ind)]; 
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
    *gain_cod_ind = cod_ind;                
    *gain_pit = g_pitch_cand[pit_ind];      
    *gain_pit_ind = g_pitch_cind[pit_ind];  
}
