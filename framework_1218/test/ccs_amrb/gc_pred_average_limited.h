/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef gc_pred_average_limited_h
#define gc_pred_average_limited_h "$Id $"

#include "typedef.h"
#include "gc_pred.h"
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
);
#endif
