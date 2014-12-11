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

/****************************************************************************
 *
 *     Function   : update_cntrl
 *     Purpose    : Control update of the background noise estimate.
 *     Inputs     : pitch:      flags for pitch detection
 *                  stat_count: stationary counter
 *                  tone:       flags indicating presence of a tone
 *                  complex:      flags for complex  detection
 *                  vadreg:     intermediate VAD flags
 *     Output     : stat_count: stationary counter
 *
 ***************************************************************************/
void update_cntrl(vadState1 *st,  /* i/o : State struct                       */
                         Word16 level[] /* i   : sub-band levels of the input frame */
                         )
{
  Word16 i, temp, stat_rat, exp;
  Word16 num, denom;
  Word16 alpha; 

  /* handle highband complex signal input  separately       */
  /* if ther has been highband correlation for some time    */
  /* make sure that the VAD update speed is low for a while */
  
  if (st->complex_warning != 0)
  {
     
     if (sub(st->stat_count, CAD_MIN_STAT_COUNT) < 0)
     {
        st->stat_count = CAD_MIN_STAT_COUNT;                  
     }
  }
  /* NB stat_count is allowed to be decreased by one below again  */
  /* deadlock in speech is not possible unless the signal is very */
  /* complex and need a high rate                                 */

  /* if fullband pitch or tone have been detected for a while, initialize stat_count */
     
  if ((sub((st->pitch & 0x6000), 0x6000) == 0) ||
      (sub((st->tone & 0x7c00), 0x7c00) == 0))
  {
     st->stat_count = STAT_COUNT;                            
  }
  else
  {
     /* if 8 last vad-decisions have been "0", reinitialize stat_count */
      
     if ((st->vadreg & 0x7f80) == 0) 
     { 
        st->stat_count = STAT_COUNT;                       
     }
     else
     {
        stat_rat = 0;                                      
        for (i = 0; i < COMPLEN; i++)
        {
           
           if (sub(level[i], st->ave_level[i]) > 0)
           {
              num = level[i];                              
              denom = st->ave_level[i];                    
           }
           else
           {
              num = st->ave_level[i];                      
              denom = level[i];                            
           }
           /* Limit nimimum value of num and denom to STAT_THR_LEVEL */
           
           if (sub(num, STAT_THR_LEVEL) < 0)
           {
              num = STAT_THR_LEVEL;                        
           }
           
           if (sub(denom, STAT_THR_LEVEL) < 0)
           {
              denom = STAT_THR_LEVEL;                      
           }
           
           exp = norm_s(denom);
           denom = shl(denom, exp);
           
           /* stat_rat = num/denom * 64 */
           temp = div_s(shr(num, 1), denom);
           stat_rat = add(stat_rat, shr(temp, sub(8, exp)));
        }
        
        /* compare stat_rat with a threshold and update stat_count */
        
        if (sub(stat_rat, STAT_THR) > 0)
        {
           st->stat_count = STAT_COUNT;                    
        }
        else
        {
           
           if ((st->vadreg & 0x4000) != 0)
           {
              
              if (st->stat_count != 0)
              {
                 st->stat_count = sub(st->stat_count, 1);  
              }
           }
        }
     }
  }
  
  /* Update average amplitude estimate for stationarity estimation */
  alpha = ALPHA4;                                          
  
  if (sub(st->stat_count, STAT_COUNT) == 0) 
  {
     alpha = 32767;                                        
  }
  else if ((st->vadreg & 0x4000) == 0) 
  {
      
     alpha = ALPHA5;                                       
  }
  
  for (i = 0; i < COMPLEN; i++)
  {
     st->ave_level[i] = add(st->ave_level[i],
                            mult_r(alpha, sub(level[i], st->ave_level[i])));
     
  }  
}
