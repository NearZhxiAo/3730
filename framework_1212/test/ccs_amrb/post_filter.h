/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Post_Filter_h
#define Post_Filter_h "$Id $"
 
#include "typedef.h"
#include "mode.h"
#include "cnst.h"
#include "preemphasis.h"
#include "agc.h"

typedef struct{
  Word16 res2[L_SUBFR];
  Word16 mem_syn_pst[M];
  preemphasisState* preemph_state;
  agcState* agc_state;
  Word16 synth_buf[M + L_FRAME];  
} Post_FilterState;


int Post_Filter (
    Post_FilterState *st, /* i/o : post filter states                        */
    enum Mode mode,       /* i   : AMR mode                                  */
    Word16 *syn,          /* i/o : synthesis speech (postfiltered is output) */
    Word16 *Az_4          /* i   : interpolated LPC parameters in all subfr. */
);
/* filters the signal syn using the parameters in Az_4 to calculate filter
   coefficients.
   The filter must be set up using Post_Filter_init prior to the first call
   to Post_Filter. Post_FilterState is updated to mirror the current state
   of the filter
 
   return 0 on success
 */
 
#endif
