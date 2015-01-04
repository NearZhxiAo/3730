/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef pitch_ol_wgh_h
#define pitch_ol_wgh_h "$Id $"
 
#include "typedef.h"
#include "mode.h"
#include "vad1.h"

/* state variable */
typedef struct {
   Word16 old_T0_med;
   Word16 ada_w;
   Word16 wght_flg; 
} pitchOLWghtState;

Word16 Pitch_ol_wgh (     /* o   : open loop pitch lag                            */
    pitchOLWghtState *st, /* i/o : State struct                                   */
    vadState1 *vadSt,      /* i/o : VAD state struct                               */
    Word16 signal[],      /* i   : signal used to compute the open loop pitch     */
                          /*       signal[-pit_max] to signal[-1] should be known */
    Word16 pit_min,       /* i   : minimum pitch lag                              */
    Word16 pit_max,       /* i   : maximum pitch lag                              */
    Word16 L_frame,       /* i   : length of frame to compute pitch               */
    Word16 old_lags[],    /* i   : history with old stored Cl lags                */
    Word16 ol_gain_flg[], /* i   : OL gain flag                                   */
    Word16 idx,           /* i   : index                                          */
    Flag dtx              /* i   : dtx flag; use dtx=1, do not use dtx=0          */
    );
 
#endif
