/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef MR475_quant_store_results_h
#define MR475_quant_store_results_h "$Id $"

#include "typedef.h"
#include "gc_pred.h"

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
);

#endif

