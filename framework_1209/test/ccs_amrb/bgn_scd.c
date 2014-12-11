/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "cnst.h"
#include "copy.h"
#include "set_zero.h"
#include "gmed_n.h"
#include "sqrt_l_exp.h"
#include "bgn_scd.h"

/*
**************************************************************************
*
*  Function    : Bgn_scd
*  Purpose     : Charaterice synthesis speech and detect background noise
*  Returns     : background noise decision; 0 = no bgn, 1 = bgn
*
**************************************************************************
*/
Word16 Bgn_scd (Bgn_scdState *st,      /* i : State variables for bgn SCD */
                Word16 ltpGainHist[],  /* i : LTP gain history            */
                Word16 speech[],       /* o : synthesis speech frame      */
                Word16 *voicedHangover /* o : # of frames after last 
                                              voiced frame                */
                )
{
   Word16 i;
   Word16 prevVoiced, inbgNoise;
   Word16 temp;
   Word16 ltpLimit, frameEnergyMin;
   Word16 currEnergy, noiseFloor, maxEnergy, maxEnergyLastPart;
   Word32 s;
   
   /* Update the inBackgroundNoise flag (valid for use in next frame if BFI) */
   /* it now works as a energy detector floating on top                      */ 
   /* not as good as a VAD.                                                  */

   currEnergy = 0;                                   
   s = (Word32) 0;                                   

   for (i = 0; i < L_FRAME; i++)
   {
       s = L_mac (s, speech[i], speech[i]);
   }

   s = L_shl(s, 2);  

   currEnergy = extract_h (s);

   frameEnergyMin = 32767;                     

   for (i = 0; i < L_ENERGYHIST; i++)
   {
      
      if (st->frameEnergyHist[i] < frameEnergyMin)
         frameEnergyMin = st->frameEnergyHist[i];           
   }

   noiseFloor = shl (frameEnergyMin, 4); /* Frame Energy Margin of 16 */

   maxEnergy = st->frameEnergyHist[0];               
   for (i = 1; i < L_ENERGYHIST-4; i++)
   {
      
      if (maxEnergy < st->frameEnergyHist[i])
      {
         maxEnergy = st->frameEnergyHist[i];         
      }
   }
   
   maxEnergyLastPart = st->frameEnergyHist[2*L_ENERGYHIST/3]; 
   for (i = 2*L_ENERGYHIST/3+1; i < L_ENERGYHIST; i++)
   {
      
      if (maxEnergyLastPart < st->frameEnergyHist[i])
      {
         maxEnergyLastPart = st->frameEnergyHist[i];      
      }
   }

   inbgNoise = 0;        /* false */                  

   /* Do not consider silence as noise */
   /* Do not consider continuous high volume as noise */
   /* Or if the current noise level is very low */
   /* Mark as noise if under current noise limit */
   /* OR if the maximum energy is below the upper limit */

        
   if ( (maxEnergy > LOWERNOISELIMIT) &&
        (currEnergy < FRAMEENERGYLIMIT) &&
        (currEnergy > LOWERNOISELIMIT) &&
        ( (currEnergy < noiseFloor) ||
          (maxEnergyLastPart < UPPERNOISELIMIT)))
   {
      
      if ((st->bgHangover + 1) > 30)
      {
         st->bgHangover = 30;                         
      } else
      {
         st->bgHangover = st->bgHangover + 1;
      }
   }
   else
   {
      st->bgHangover = 0;                                 
   }
   
   /* make final decision about frame state , act somewhat cautiosly */
   
   if (st->bgHangover > 1)
      inbgNoise = 1;       /* true  */                 

   for (i = 0; i < L_ENERGYHIST-1; i++)
   {
      st->frameEnergyHist[i] = st->frameEnergyHist[i+1]; 
   }
   st->frameEnergyHist[L_ENERGYHIST-1] = currEnergy;              
   
   /* prepare for voicing decision; tighten the threshold after some 
      time in noise */
   ltpLimit = 13926;             /* 0.85  Q14 */      
   
   if (st->bgHangover > 8)
   {
      ltpLimit = 15565;          /* 0.95  Q14 */     
   }
   
   if (st->bgHangover > 15)
   {
      ltpLimit = 16383;          /* 1.00  Q14 */     
   }

   /* weak sort of voicing indication. */
   prevVoiced = 0;        /* false */                
   

   if (gmed_n(&ltpGainHist[4], 5) > ltpLimit)
   {
      prevVoiced = 1;     /* true  */                
   }
      
   if (st->bgHangover > 20) 
   {
      if (gmed_n(ltpGainHist, 9) > ltpLimit)
      {
         prevVoiced = 1;  /* true  */                
      }
      else
      {
         prevVoiced = 0;  /* false  */                
      }
   }
   
   
   if (prevVoiced)
   {
      *voicedHangover = 0;                        
   }
   else
   {
      temp = *voicedHangover + 1;
      
      if (temp > 10)
      {
         *voicedHangover = 10;                    
      }
      else
      {
         *voicedHangover = temp;                  
      }
   }

   return inbgNoise;
}
