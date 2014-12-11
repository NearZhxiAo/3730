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
#include "cnst.h"
#include "basic_op.h"

#include "pitch_ol.h"
#include "pitch_ol_wgh.h"


int ol_ltp(
    pitchOLWghtState *st, /* i/o : State struct                            */
    vadState1 *vadSt,      /* i/o : VAD state struct                        */
    enum Mode mode,       /* i   : coder mode                              */
    Word16 wsp[],         /* i   : signal used to compute the OL pitch, Q0 */
                          /*       uses signal[-pit_max] to signal[-1]     */
    Word16 *T_op,         /* o   : open loop pitch lag,                 Q0 */
    Word16 old_lags[],    /* i   : history with old stored Cl lags         */
    Word16 ol_gain_flg[], /* i   : OL gain flag                            */
    Word16 idx,           /* i   : index                                   */
    Flag dtx              /* i   : dtx flag; use dtx=1, do not use dtx=0   */
    )
{
      
   if (mode != MR102)
   {
      ol_gain_flg[0] = 0;                                       
      ol_gain_flg[1] = 0;                                       
   }
   
    
   if ((mode == MR475) || (mode == MR515))
   {
      *T_op = Pitch_ol(vadSt, mode, wsp, PIT_MIN, PIT_MAX, L_FRAME, idx, dtx);
                                                                
   }
   else
   {
      if (mode <= MR795)
      {
         
         *T_op = Pitch_ol(vadSt, mode, wsp, PIT_MIN, PIT_MAX, L_FRAME_BY2,
                          idx, dtx);
                                                                
      }
      else if (mode == MR102)
      {
          
         *T_op = Pitch_ol_wgh(st, vadSt, wsp, PIT_MIN, PIT_MAX, L_FRAME_BY2,
                              old_lags, ol_gain_flg, idx, dtx);
                                                                
      }
      else
      {
                    
         *T_op = Pitch_ol(vadSt, mode, wsp, PIT_MIN_MR122, PIT_MAX,
                          L_FRAME_BY2, idx, dtx);
                                                                
      }
   }
   return 0;
}
