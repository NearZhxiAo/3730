/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef pitch_fr_h
#define pitch_fr_h "$Id $"
 
#include "typedef.h"
#include "mode.h"

typedef struct {
   Word16 T0_prev_subframe;   /* integer pitch lag of previous sub-frame */
} Pitch_frState;
 

Word16 Pitch_fr (        /* o   : pitch period (integer)                    */
    Pitch_frState *st,   /* i/o : State struct                              */
    enum Mode mode,      /* i   : codec mode                                */
    Word16 T_op[],       /* i   : open loop pitch lags                      */
    Word16 exc[],        /* i   : excitation buffer                         */
    Word16 xn[],         /* i   : target vector                             */
    Word16 h[],          /* i   : impulse response of synthesis and   
                                  weighting filters                         */
    Word16 L_subfr,      /* i   : Length of subframe                        */
    Word16 i_subfr,      /* i   : subframe offset                           */
    Word16 *pit_frac,    /* o   : pitch period (fractional)                 */
    Word16 *resu3,       /* o   : subsample resolution 1/3 (=1) or 1/6 (=0) */
    Word16 *ana_index    /* o   : index of encoding                         */
);
 
#endif
