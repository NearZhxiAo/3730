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
#include "table.h"
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




/*
********************************************************************************
*                         PRIVATE PROGRAM CODE
********************************************************************************
*/
void MR475_quant_store_results(

    gc_predState *pred_st, /* i/o: gain predictor state struct               */
    const Word16 *p,       /* i  : pointer to selected quantizer table entry */
    Word16 gcode0,         /* i  : predicted CB gain,     Q(14 - exp_gcode0) */
    Word16 exp_gcode0,     /* i  : exponent of predicted CB gain,        Q0  */
    Word16 *gain_pit,      /* o  : Pitch gain,                           Q14 */
    Word16 *gain_cod       /* o  : Code gain,                            Q1  */
)
{

    Word16 g_code, exp, frac, tmp;
    Word32 L_tmp;

    Word16 qua_ener_MR122; /* o  : quantized energy error, MR122 version Q10 */
    Word16 qua_ener;       /* o  : quantized energy error,               Q10 */

    /* Read the quantized gains */
    *gain_pit = *p++;                
    g_code = *p++;                   

    /*------------------------------------------------------------------*
     *  calculate final fixed codebook gain:                            *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
     *                                                                  *
     *   gc = gc0 * g                                                   *
     *------------------------------------------------------------------*/

    L_tmp = L_mult(g_code, gcode0);
    L_tmp = L_shr(L_tmp, sub(10, exp_gcode0));
    *gain_cod = extract_h(L_tmp);

    /*------------------------------------------------------------------*
     *  calculate predictor update values and update gain predictor:    *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    *
     *                                                                  *
     *   qua_ener       = log2(g)                                       *
     *   qua_ener_MR122 = 20*log10(g)                                   *
     *------------------------------------------------------------------*/

    Log2 (L_deposit_l (g_code), &exp, &frac); /* Log2(x Q12) = log2(x) + 12 */
    exp = sub(exp, 12);

    tmp = shr_r (frac, 5);
    qua_ener_MR122 = add (tmp, shl (exp, 10));

    L_tmp = Mpy_32_16(exp, frac, 24660); /* 24660 Q12 ~= 6.0206 = 20*log10(2) */
    qua_ener = round (L_shl (L_tmp, 13)); /* Q12 * Q0 = Q13 -> Q10 */

    gc_pred_update(pred_st, qua_ener_MR122, qua_ener);
}

