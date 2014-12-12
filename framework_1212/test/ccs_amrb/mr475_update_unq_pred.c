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
 * FUNCTION:  MR475_update_unq_pred()
 *
 * PURPOSE:   use optimum codebook gain and update "unquantized"
 *            gain predictor with the (bounded) prediction error
 *
 *************************************************************************/
void
MR475_update_unq_pred(
    gc_predState *pred_st, /* i/o: gain predictor state struct            */
    Word16 exp_gcode0,     /* i  : predicted CB gain (exponent MSW),  Q0  */
    Word16 frac_gcode0,    /* i  : predicted CB gain (exponent LSW),  Q15 */
    Word16 cod_gain_exp,   /* i  : optimum codebook gain (exponent),  Q0  */
    Word16 cod_gain_frac   /* i  : optimum codebook gain (fraction),  Q15 */
)
{
    Word16 tmp, exp, frac;
    Word16 qua_ener, qua_ener_MR122;
    Word32 L_tmp;

    /* calculate prediction error factor (given optimum CB gain gcu):
     *
     *   predErrFact = gcu / gcode0
     *   (limit to MIN_PRED_ERR_FACT <= predErrFact <= MAX_PRED_ERR_FACT
     *    -> limit qua_ener*)
     *
     * calculate prediction error (log):
     *
     *   qua_ener_MR122 = log2(predErrFact)
     *   qua_ener       = 20*log10(predErrFact)
     *
     */

    if ( cod_gain_frac <= 0)
    {
        /* if gcu <= 0 -> predErrFact = 0 < MIN_PRED_ERR_FACT */
        /* -> set qua_ener(_MR122) directly                   */
        qua_ener = MIN_QUA_ENER;             
        qua_ener_MR122 = MIN_QUA_ENER_MR122; 
    }
    else
    {
        /* convert gcode0 from DPF to standard fraction/exponent format */
        /* with normalized frac, i.e. 16384 <= frac <= 32767            */
        /* Note: exponent correction (exp=exp-14) is done after div_s   */
        frac_gcode0 = extract_l (Pow2 (14, frac_gcode0));

        /* make sure cod_gain_frac < frac_gcode0  for div_s */
        if ( sub(cod_gain_frac, frac_gcode0) >= 0)
        {
            cod_gain_frac = shr (cod_gain_frac, 1);
            cod_gain_exp = add (cod_gain_exp, 1);
        }

        /*
          predErrFact
             = gcu / gcode0
             = cod_gain_frac/frac_gcode0 * 2^(cod_gain_exp-(exp_gcode0-14))
             = div_s (c_g_f, frac_gcode0)*2^-15 * 2^(c_g_e-exp_gcode0+14)
             = div_s * 2^(cod_gain_exp-exp_gcode0 - 1)
        */
        frac = div_s (cod_gain_frac, frac_gcode0);
        tmp = sub (sub (cod_gain_exp, exp_gcode0), 1);

        Log2 (L_deposit_l (frac), &exp, &frac);
        exp = add (exp, tmp);

        /* calculate prediction error (log2, Q10) */
        qua_ener_MR122 = shr_r (frac, 5);
        qua_ener_MR122 = add (qua_ener_MR122, shl (exp, 10));

        if ( sub(qua_ener_MR122, MIN_QUA_ENER_MR122) < 0)
        {
            qua_ener = MIN_QUA_ENER;             
            qua_ener_MR122 = MIN_QUA_ENER_MR122; 
        }
        else if ( sub(qua_ener_MR122, MAX_QUA_ENER_MR122) > 0)
        {
            qua_ener = MAX_QUA_ENER;             
            qua_ener_MR122 = MAX_QUA_ENER_MR122; 
        }
        else
        {
            /* calculate prediction error (20*log10, Q10) */
            L_tmp = Mpy_32_16(exp, frac, 24660);
            /* 24660 Q12 ~= 6.0206 = 20*log10(2) */
            qua_ener = round (L_shl (L_tmp, 13));
            /* Q12 * Q0 = Q13 -> Q26 -> Q10     */
        }
    }

    /* update MA predictor memory */
    gc_pred_update(pred_st, qua_ener_MR122, qua_ener);
}
