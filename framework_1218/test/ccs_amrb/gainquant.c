/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
 

#include <stdio.h>
#include "typedef.h"
#include "basic_op.h"
#include "cnst.h"
#include "mode.h"
#include "gc_pred.h"
#include "set_zero.h"
#include "gc_pred_copy.h"
#include "gc_pred_update.h"

#include "qua_gain.h"
#include "g_code.h"
#include "gainquant.h"
#include "q_gain_code.h"
#include "calc_filt_energies.h"
#include "calc_target_energy.h"
#include "mr475_update_unq_pred.h"
#include "mr475_gain_quant.h"
#include "mr795_gain_quant.h"


int gainQuant(
    gainQuantState *st,   /* i/o : State struct                      */
    enum Mode mode,       /* i   : coder mode                        */
    Word16 res[],         /* i   : LP residual,                 Q0   */
    Word16 exc[],         /* i   : LTP excitation (unfiltered), Q0   */
    Word16 code[],        /* i   : CB innovation (unfiltered),  Q13  */
                          /*       (unsharpened for MR475)           */
    Word16 xn[],          /* i   : Target vector.                    */
    Word16 xn2[],         /* i   : Target vector.                    */
    Word16 y1[],          /* i   : Adaptive codebook.                */
    Word16 Y2[],          /* i   : Filtered innovative vector.       */
    Word16 g_coeff[],     /* i   : Correlations <xn y1> <y1 y1>      */
                          /*       Compute in G_pitch().             */
    Word16 even_subframe, /* i   : even subframe indicator flag      */
    Word16 gp_limit,      /* i   : pitch gain limit                  */
    Word16 *sf0_gain_pit, /* o   : Pitch gain sf 0.   MR475          */
    Word16 *sf0_gain_cod, /* o   : Code gain sf 0.    MR475          */
    Word16 *gain_pit,     /* i/o : Pitch gain.                       */
    Word16 *gain_cod,     /* o   : Code gain.                        */
                          /*       MR475: gain_* unquantized in even */
                          /*       subframes, quantized otherwise    */
    Word16 **anap         /* o   : Index of quantization             */
)
{
    Word16 exp_gcode0;
    Word16 frac_gcode0;
    Word16 qua_ener_MR122;
    Word16 qua_ener;
    Word16 frac_coeff[5];
    Word16 exp_coeff[5];
    Word16 exp_en, frac_en;
    Word16 cod_gain_exp, cod_gain_frac;
            
    
    if (sub (mode, MR475) == 0)
    {
        
        if (even_subframe != 0)
        {
            /* save position in output parameter stream and current
               state of codebook gain predictor */
            st->gain_idx_ptr = (*anap)++;
            gc_pred_copy(st->gc_predSt, st->gc_predUnqSt);
            
            /* predict codebook gain (using "unquantized" predictor)*/
            /* (note that code[] is unsharpened in MR475)           */
            gc_pred(st->gc_predUnqSt, mode, code,
                    &st->sf0_exp_gcode0, &st->sf0_frac_gcode0,
                    &exp_en, &frac_en);
            
            /* calculate energy coefficients for quantization
               and store them in state structure (will be used
               in next subframe when real quantizer is run) */
            calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
                               st->sf0_frac_coeff, st->sf0_exp_coeff,
                               &cod_gain_frac, &cod_gain_exp);

            /* store optimum codebook gain (Q1) */
            *gain_cod = shl (cod_gain_frac, add (cod_gain_exp, 1));
                                                         

            calc_target_energy(xn,
                               &st->sf0_exp_target_en, &st->sf0_frac_target_en);

            /* calculate optimum codebook gain and update
               "unquantized" predictor                    */
            MR475_update_unq_pred(st->gc_predUnqSt,
                                  st->sf0_exp_gcode0, st->sf0_frac_gcode0,
                                  cod_gain_exp, cod_gain_frac);
            
            /* the real quantizer is not run here... */
        }
        else
        {
            /* predict codebook gain (using "unquantized" predictor) */
            /* (note that code[] is unsharpened in MR475)            */
            gc_pred(st->gc_predUnqSt, mode, code,
                    &exp_gcode0, &frac_gcode0,
                    &exp_en, &frac_en);
            
            /* calculate energy coefficients for quantization */
            calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
                               frac_coeff, exp_coeff,
                               &cod_gain_frac, &cod_gain_exp);

            calc_target_energy(xn, &exp_en, &frac_en);

            /* run real (4-dim) quantizer and update real gain predictor */
            *st->gain_idx_ptr = MR475_gain_quant(
                st->gc_predSt,
                st->sf0_exp_gcode0, st->sf0_frac_gcode0, 
                st->sf0_exp_coeff,  st->sf0_frac_coeff,
                st->sf0_exp_target_en, st->sf0_frac_target_en,
                code,
                exp_gcode0, frac_gcode0, 
                exp_coeff, frac_coeff,
                exp_en, frac_en,
                gp_limit,
                sf0_gain_pit, sf0_gain_cod,   
                gain_pit, gain_cod);
        }
    }
    else
    {
        /*-------------------------------------------------------------------*
         *  predict codebook gain and quantize                               *
         *  (also compute normalized CB innovation energy for MR795)         *
         *-------------------------------------------------------------------*/
        gc_pred(st->gc_predSt, mode, code, &exp_gcode0, &frac_gcode0,
                &exp_en, &frac_en);
        
        
        if (sub(mode, MR122) == 0)
        {
            *gain_cod = G_code (xn2, Y2); 
            *(*anap)++ = q_gain_code (mode, exp_gcode0, frac_gcode0,
                                      gain_cod, &qua_ener_MR122, &qua_ener);
            
        }
        else
        {
            /* calculate energy coefficients for quantization */
            calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
                               frac_coeff, exp_coeff,
                               &cod_gain_frac, &cod_gain_exp);
            
            
            if (sub (mode, MR795) == 0)
            {
                MR795_gain_quant(st->adaptSt, res, exc, code,
                                 frac_coeff, exp_coeff,
                                 exp_en, frac_en,
                                 exp_gcode0, frac_gcode0, L_SUBFR,
                                 cod_gain_frac, cod_gain_exp,
                                 gp_limit, gain_pit, gain_cod,
                                 &qua_ener_MR122, &qua_ener,
                                 anap);
            }
            else
            {
                *(*anap)++ = Qua_gain(mode,
                                      exp_gcode0, frac_gcode0,
                                      frac_coeff, exp_coeff, gp_limit,
                                      gain_pit, gain_cod,
                                      &qua_ener_MR122, &qua_ener);
                
            }
        }
        
        /*------------------------------------------------------------------*
         *  update table of past quantized energies                         *
         *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                         *
         *  st->past_qua_en(Q10) = 20 * Log10(qua_gain_code) / constant     *
         *                       = Log2(qua_gain_code)                      *
         *                       = qua_ener                                 *
         *                                           constant = 20*Log10(2) *
         *------------------------------------------------------------------*/
        gc_pred_update(st->gc_predSt, qua_ener_MR122, qua_ener);
    }
        
    return 0;
}
