/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "mode.h"
#include "basic_op.h"
#include "cnst.h"

#include "code_2i40_11bits.h"
#include "code_2i40_9bits.h"
#include "code_3i40_14bits.h"
#include "code_4i40_17bits.h"
#include "code_8i40_31bits.h"
#include "code_10i40_35bits.h"

int cbsearch(Word16 x[],    /* i : target vector, Q0                       */
             Word16 h[],    /* i : impulse response of weighted synthesis  */
                            /*     filter h[-L_subfr..-1] must be set to   */
                            /*     zero. Q12                               */
             Word16 T0,     /* i : Pitch lag                               */
             Word16 pitch_sharp, /* i : Last quantized pitch gain, Q14     */
             Word16 gain_pit,    /* i : Pitch gain, Q14                    */ 
             Word16 res2[], /* i : Long term prediction residual, Q0       */
             Word16 code[], /* o : Innovative codebook, Q13                */
             Word16 y[],    /* o : filtered fixed codebook excitation, Q12 */
             Word16 **anap, /* o : Signs of the pulses                     */
             enum Mode mode,/* i : coder mode                              */
             Word16 subNr)  /* i : subframe number                         */
{
   Word16 index;
   Word16 i, temp, pit_sharpTmp;
   
   /* For MR74, the pre and post CB pitch sharpening is included in the
    * codebook search routine, while for MR122 is it not.
    */
   
     
   if ((mode == MR475) || (mode == MR515))
   {   /* MR475, MR515 */ 
      *(*anap)++ = code_2i40_9bits(subNr, x, h, T0, pitch_sharp,
                                   code, y, &index);
      *(*anap)++ = index;    /* sign index */                  
   }
   else if (mode == MR59)
   {   /* MR59 */
       
      *(*anap)++ = code_2i40_11bits(x, h, T0, pitch_sharp, code, y, &index);
      *(*anap)++ = index;    /* sign index */                  
   }
   else if (mode == MR67)
   {   /* MR67 */
        
      *(*anap)++ = code_3i40_14bits(x, h, T0, pitch_sharp, code, y, &index);
      *(*anap)++ = index;    /* sign index */                  
   }
   else if ((mode == MR74) || (mode == MR795))
   {   /* MR74, MR795 */
         
      *(*anap)++ = code_4i40_17bits(x, h, T0, pitch_sharp, code, y, &index);
      *(*anap)++ = index;    /* sign index */                  
   }
   else if (mode == MR102)
   {   /* MR102 */
        
      /*-------------------------------------------------------------*
       * - include pitch contribution into impulse resp. h1[]        *
       *-------------------------------------------------------------*/
      /* pit_sharpTmp = pit_sharp;                     */
      /* if (pit_sharpTmp > 1.0) pit_sharpTmp = 1.0;   */
      pit_sharpTmp = shl (pitch_sharp, 1);
      for (i = T0; i < L_SUBFR; i++)
      {
         temp = mult(h[i - T0], pit_sharpTmp);
         h[i] = add(h[i], temp);                               
      }

      /*--------------------------------------------------------------*
       * - Innovative codebook search (find index and gain)           *
       *--------------------------------------------------------------*/
      code_8i40_31bits (x, res2, h, code, y, *anap);
      *anap += 7;                                              add(0,0);

      /*-------------------------------------------------------*
       * - Add the pitch contribution to code[].               *
       *-------------------------------------------------------*/ 
      for (i = T0; i < L_SUBFR; i++)
      {
         temp = mult (code[i - T0], pit_sharpTmp);
         code[i] = add (code[i], temp);                        
      }
   }
   else
   {  /* MR122 */
      /*-------------------------------------------------------------*
       * - include pitch contribution into impulse resp. h1[]        *
       *-------------------------------------------------------------*/
        
      
      /* pit_sharpTmp = gain_pit;                      */
      /* if (pit_sharpTmp > 1.0) pit_sharpTmp = 1.0;   */
      pit_sharpTmp = shl (gain_pit, 1);
      
      for (i = T0; i < L_SUBFR; i++)
      {
         temp = mult(h[i - T0], pit_sharpTmp);
         h[i] = add(h[i], temp);                               
      }
      /*--------------------------------------------------------------*
       * - Innovative codebook search (find index and gain)           *
       *--------------------------------------------------------------*/
      
      code_10i40_35bits (x, res2, h, code, y, *anap);
      *anap += 10;      	                                   add(0,0);
      
      /*-------------------------------------------------------*
       * - Add the pitch contribution to code[].               *
       *-------------------------------------------------------*/ 
      for (i = T0; i < L_SUBFR; i++)
      {
         temp = mult (code[i - T0], pit_sharpTmp);
         code[i] = add (code[i], temp);                        
      }     
   }

   return 0;
}
