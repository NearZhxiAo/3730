/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "gc_pred.h"
#include "copy.h"

/*************************************************************************
 *
 * FUNCTION:  gc_pred_copy()
 *
 * PURPOSE: Copy MA predictor state variable
 *
 *************************************************************************/
void
gc_pred_copy(
    gc_predState *st_src,  /* i : State struct                           */
    gc_predState *st_dest  /* o : State struct                           */
)
{
    Copy (st_src->past_qua_en, st_dest->past_qua_en, NPRED);
    Copy (st_src->past_qua_en_MR122, st_dest->past_qua_en_MR122, NPRED);
}
