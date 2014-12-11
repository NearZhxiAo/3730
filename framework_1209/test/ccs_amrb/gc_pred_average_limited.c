/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
#include "log2.h"
#include "copy.h"
#include "table.h"
#include "gc_pred.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/* MA prediction coefficients (Q6)  */

/* minimum quantized energy: -14 dB */
#define MIN_ENERGY       -14336       /* 14                 Q10 */
#define MIN_ENERGY_MR122  -2381       /* 14 / (20*log10(2)) Q10 */ 
/*************************************************************************
 *
 * FUNCTION:  gc_pred_average_limited()
 *
 * PURPOSE: get average of MA predictor state values (with a lower limit)
 *          [used in error concealment]
 *
 *************************************************************************/
void gc_pred_average_limited(
    gc_predState *st,       /* i: State struct                    */
    Word16 *ener_avg_MR122, /* o: everaged quantized energy,  Q10 */
                            /*    (log2(qua_err))                 */
    Word16 *ener_avg        /* o: averaged quantized energy,  Q10 */
                            /*    (20*log10(qua_err))             */
)
{
    Word16 av_pred_en;
    Word16 i;

    /* do average in MR122 mode (log2() domain) */
    av_pred_en = 0;                                        
    for (i = 0; i < NPRED; i++)
    {
        av_pred_en = add (av_pred_en, st->past_qua_en_MR122[i]);
    }

    /* av_pred_en = 0.25*av_pred_en */
    av_pred_en = mult (av_pred_en, 8192);

    /* if (av_pred_en < -14/(20Log10(2))) av_pred_en = .. */
    
    if (sub (av_pred_en, MIN_ENERGY_MR122) < 0)
    {
        av_pred_en = MIN_ENERGY_MR122;                     
    }
    *ener_avg_MR122 = av_pred_en;                          

    /* do average for other modes (20*log10() domain) */
    av_pred_en = 0;                                        
    for (i = 0; i < NPRED; i++)
    {
        av_pred_en = add (av_pred_en, st->past_qua_en[i]);
    }

    /* av_pred_en = 0.25*av_pred_en */
    av_pred_en = mult (av_pred_en, 8192);

    /* if (av_pred_en < -14) av_pred_en = .. */
    
    if (sub (av_pred_en, MIN_ENERGY) < 0)
    {
        av_pred_en = MIN_ENERGY;                           
    }
    *ener_avg = av_pred_en;                                
}
