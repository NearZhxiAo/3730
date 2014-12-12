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
#include "cnst_vad.h"
#include "vad1.h"
/*************************************************************************
*
*  Function:   vad1_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int vad1_reset (vadState1 *state)
{
   Word16 i, j;
   
   /* Initialize pitch detection variables */
   state->oldlag_count = 0;
   state->oldlag = 0;         
   state->pitch = 0;
   state->tone = 0;            

   state->complex_high = 0;            
   state->complex_low = 0;            
   state->complex_hang_timer = 0;

   state->vadreg = 0;         

   state->stat_count = 0;    
   state->burst_count = 0;    
   state->hang_count = 0;     
   state->complex_hang_count = 0;     
   
   /* initialize memory used by the filter bank */
   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 2; j++) 
      {
         state->a_data5[i][j] = 0;  
      }
   }
   
   for (i = 0; i < 5; i++)
   {
      state->a_data3[i] = 0;        
   }
   
   /* initialize the rest of the memory */
   for (i = 0; i < COMPLEN; i++)
   {
      state->bckr_est[i] = NOISE_INIT;  
      state->old_level[i] = NOISE_INIT; 
      state->ave_level[i] = NOISE_INIT; 
      state->sub_level[i] = 0;          
   }
   
   state->best_corr_hp = CVAD_LOWPOW_RESET; 

   state->speech_vad_decision = 0;
   state->complex_warning = 0;
   state->sp_burst_count = 0;        

   state->corr_hp_fast = CVAD_LOWPOW_RESET;
 
   return 0;
}

