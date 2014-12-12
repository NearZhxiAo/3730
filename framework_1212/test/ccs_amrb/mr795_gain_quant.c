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
#include "mr795_gain_code_quant3.h"
#include "mr795_gain_code_quant_mod.h"

/*************************************************************************
 *
 * FUNCTION:  MR795_gain_quant
 *
 * PURPOSE:   pitch and codebook quantization for MR795
 *
 *************************************************************************/
void
MR795_gain_quant(
    GainAdaptState *adapt_st, /* i/o: gain adapter state structure       */
    Word16 res[],             /* i  : LP residual,                  Q0   */
    Word16 exc[],             /* i  : LTP excitation (unfiltered),  Q0   */
    Word16 code[],            /* i  : CB innovation (unfiltered),   Q13  */
    Word16 frac_coeff[],      /* i  : coefficients (5),             Q15  */
    Word16 exp_coeff[],       /* i  : energy coefficients (5),      Q0   */
                              /*      coefficients from calc_filt_ener() */
    Word16 exp_code_en,       /* i  : innovation energy (exponent), Q0   */
    Word16 frac_code_en,      /* i  : innovation energy (fraction), Q15  */
    Word16 exp_gcode0,        /* i  : predicted CB gain (exponent), Q0   */
    Word16 frac_gcode0,       /* i  : predicted CB gain (fraction), Q15  */
    Word16 L_subfr,           /* i  : Subframe length                    */
    Word16 cod_gain_frac,     /* i  : opt. codebook gain (fraction),Q15  */
    Word16 cod_gain_exp,      /* i  : opt. codebook gain (exponent), Q0  */
    Word16 gp_limit,          /* i  : pitch gain limit                   */
    Word16 *gain_pit,         /* i/o: Pitch gain,                   Q14  */
    Word16 *gain_cod,         /* o  : Code gain,                    Q1   */
    Word16 *qua_ener_MR122,   /* o  : quantized energy error,       Q10  */
                              /*      (for MR122 MA predictor update)    */
    Word16 *qua_ener,         /* o  : quantized energy error,       Q10  */
                              /*      (for other MA predictor update)    */
    Word16 **anap             /* o  : Index of quantization              */
                              /*      (first gain pitch, then code pitch)*/
)
{
    Word16 frac_en[4];
    Word16 exp_en[4];
    Word16 ltpg, alpha, gcode0;
    Word16 g_pitch_cand[3];      /* pitch gain candidates   Q14 */
    Word16 g_pitch_cind[3];      /* pitch gain indices      Q0  */
    Word16 gain_pit_index;
    Word16 gain_cod_index;
    Word16 exp;
    Word16 gain_cod_unq;         /* code gain (unq.) Q(10-exp_gcode0)  */


    /* get list of candidate quantized pitch gain values
     * and corresponding quantization indices
     */
    gain_pit_index = q_gain_pitch (MR795, gp_limit, gain_pit,
                                   g_pitch_cand, g_pitch_cind); 
                      /* function result */

    /*-------------------------------------------------------------------*
     *  predicted codebook gain                                          *
     *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
     *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
     *                                                                   *
     *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
     *-------------------------------------------------------------------*/
    gcode0 = extract_l(Pow2(14, frac_gcode0));          /* Q14 */

    /* pre-quantization of codebook gain
     * (using three pitch gain candidates);
     * result: best guess of pitch gain and code gain
     */
    MR795_gain_code_quant3(
        exp_gcode0, gcode0, g_pitch_cand, g_pitch_cind,
        frac_coeff, exp_coeff,
        gain_pit, &gain_pit_index, gain_cod, &gain_cod_index,
        qua_ener_MR122, qua_ener);

    /* calculation of energy coefficients and LTP coding gain */
    calc_unfilt_energies(res, exc, code, *gain_pit, L_subfr,
                         frac_en, exp_en, &ltpg);

    /* run gain adaptor, calculate alpha factor to balance LTP/CB gain
     * (this includes the gain adaptor update)
     * Note: ltpg = 0 if frac_en[0] == 0, so the update is OK in that case
     */
    gain_adapt(adapt_st, ltpg, *gain_cod, &alpha);

    /* if this is a very low energy signal (threshold: see
     * calc_unfilt_energies) or alpha <= 0 then don't run the modified quantizer
     */
      
    if (frac_en[0] != 0 && alpha > 0)
    {
        /* innovation energy <cod cod> was already computed in gc_pred() */
        /* (this overwrites the LtpResEn which is no longer needed)      */
        frac_en[3] = frac_code_en;          
        exp_en[3] = exp_code_en;            
        
        /* store optimum codebook gain in Q(10-exp_gcode0) */
        exp = add (sub (cod_gain_exp, exp_gcode0), 10);
        gain_cod_unq = shl (cod_gain_frac, exp);
        
        /* run quantization with modified criterion */
        gain_cod_index = MR795_gain_code_quant_mod(
            *gain_pit, exp_gcode0, gcode0,
            frac_en, exp_en, alpha, gain_cod_unq,
            gain_cod, qua_ener_MR122, qua_ener);  /* function result */
    }

    *(*anap)++ = gain_pit_index;        
    *(*anap)++ = gain_cod_index;        
}
