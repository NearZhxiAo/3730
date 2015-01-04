/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef dec_gain_h
#define dec_gain_h "$Id $"

#include "typedef.h"
#include "gc_pred.h"
#include "mode.h"

/*************************************************************************
 *
 *   FUNCTION:  Dec_gain()
 *
 *   PURPOSE: Decode the pitch and codebook gains
 *
 ************************************************************************/
void Dec_gain(
    gc_predState *pred_state, /* i/o: MA predictor state           */
    enum Mode mode,           /* i  : AMR mode                     */
    Word16 index,             /* i  : index of quantization.       */
    Word16 code[],            /* i  : Innovative vector.           */
    Word16 evenSubfr,         /* i  : Flag for even subframes      */     
    Word16 * gain_pit,        /* o  : Pitch gain.                  */
    Word16 * gain_cod         /* o  : Code gain.                   */
);

#endif
