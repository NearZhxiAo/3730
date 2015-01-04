/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef ec_gain_code_h
#define ec_gain_code_h "$Id $"

#include "typedef.h"
#include "ec_gain_code.h"
#include "gc_pred.h"

 
typedef struct {
  Word16 gbuf[5];
  Word16 past_gain_code;
  Word16 prev_gc;
} ec_gain_codeState;

/*
**************************************************************************
*
*  Function    : ec_gain_code
*  Purpose     : conceal the codebook gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_code (
    ec_gain_codeState *st,    /* i/o : State struct                     */
    gc_predState *pred_state, /* i/o : MA predictor state               */
    Word16 state,             /* i   : state of the state machine       */
    Word16 *gain_code         /* o   : decoded innovation gain          */
);

#endif

