/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst_vad.h"
#include "vad1.h"
#include "update_cntrl.h"
/****************************************************************************
 *
 *     Function   : noise_estimate_update
 *     Purpose    : Update of background noise estimate
 *     Inputs     : bckr_est:   background noise estimate
 *                  pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *     Outputs    : bckr_est:   background noise estimate
 *
 ***************************************************************************/
void noise_estimate_update(
                  vadState1 *st,    /* i/o : State struct                       */
                  Word16 level[]   /* i   : sub-band levels of the input frame */
                  )
{
   Word16 i, alpha_up, alpha_down, bckr_add;
   
   /* Control update of bckr_est[] */
   update_cntrl(st, level);
   
   /* Choose update speed */
   bckr_add = 2;                                           
   
       
   if (((0x7800 & st->vadreg) == 0) && 
       ((st->pitch & 0x7800) == 0) 
       &&  (st->complex_hang_count == 0))
   {
      alpha_up = ALPHA_UP1;                                
      alpha_down = ALPHA_DOWN1;                            
   }
   else 
   {
       
      if ((st->stat_count == 0) 
          && (st->complex_hang_count == 0))
      {
         alpha_up = ALPHA_UP2;                             
         alpha_down = ALPHA_DOWN2;                         
      }
      else
      {
         alpha_up = 0;                                     
         alpha_down = ALPHA3;                              
         bckr_add = 0;                                     
      }
   }
   
   /* Update noise estimate (bckr_est) */
   for (i = 0; i < COMPLEN; i++)
   {
      Word16 temp;
      temp = sub(st->old_level[i], st->bckr_est[i]);
      
      
      if (temp < 0)
      { /* update downwards*/
         st->bckr_est[i] = add(-2, add(st->bckr_est[i], mult_r(alpha_down, temp)));
         
         
         /* limit minimum value of the noise estimate to NOISE_MIN */
         
         if (sub(st->bckr_est[i], NOISE_MIN) < 0)
         {
            st->bckr_est[i] = NOISE_MIN;                  
         }
      }
      else
      { /* update upwards */
         st->bckr_est[i] = add(bckr_add, add(st->bckr_est[i], mult_r(alpha_up, temp)));
         
         
         /* limit maximum value of the noise estimate to NOISE_MAX */
         
         if (sub(st->bckr_est[i], NOISE_MAX) > 0)
         {
            st->bckr_est[i] = NOISE_MAX;                  
         }
      }
   }
   
   /* Update signal levels of the previous frame (old_level) */
   for(i = 0; i < COMPLEN; i++)
   {
      st->old_level[i] = level[i];                        
   }
}

