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
#include "basic_op.h"
#include "cnst.h"
#include "copy.h"
#include "ph_disp.h"
#include "table.h"


/*************************************************************************
*
*  Function:   ph_disp
*
*              Adaptive phase dispersion; forming of total excitation
*              (for synthesis part of decoder)
*
**************************************************************************
*/
void ph_disp (
      ph_dispState *state, /* i/o     : State struct                     */
      enum Mode mode,      /* i       : codec mode                       */
      Word16 x[],          /* i/o Q0  : in:  LTP excitation signal       */
                           /*           out: total excitation signal     */
      Word16 cbGain,       /* i   Q1  : Codebook gain                    */
      Word16 ltpGain,      /* i   Q14 : LTP gain                         */
      Word16 inno[],       /* i/o Q13 : Innovation vector (Q12 for 12.2) */
      Word16 pitch_fac,    /* i   Q14 : pitch factor used to scale the
                                        LTP excitation (Q13 for 12.2)    */
      Word16 tmp_shift     /* i   Q0  : shift factor applied to sum of   
                                        scaled LTP ex & innov. before
                                        rounding                         */
)
{
   Word16 i, i1;
   Word16 tmp1;
   Word32 L_temp;
   Word16 impNr;           /* indicator for amount of disp./filter used */

   Word16 inno_sav[L_SUBFR];
   Word16 ps_poss[L_SUBFR];
   Word16 j, nze, nPulse, ppos;
   const Word16 *ph_imp;   /* Pointer to phase dispersion filter */

   /* Update LTP gain memory */
   for (i = PHDGAINMEMSIZE-1; i > 0; i--)
   {
       state->gainMem[i] = state->gainMem[i-1];                    
   }
   state->gainMem[0] = ltpGain;                                    
   
   /* basic adaption of phase dispersion */
   
   if (ltpGain < PHDTHR2LTP) 
   {    /* if (ltpGain < 0.9) */
       
       if (ltpGain > PHDTHR1LTP)
       {  /* if (ltpGain > 0.6 */
          impNr = 1; /* medium dispersion */                      
       }
       else
       {
          impNr = 0; /* maximum dispersion */                     
       }
   }
   else
   {
      impNr = 2; /* no dispersion */                              
   }
   
   /* onset indicator */
   /* onset = (cbGain  > onFact * cbGainMem[0]) */
                                                                   
   tmp1 = round(L_shl(L_mult(state->prevCbGain, ONFACTPLUS1), 2));
   
   if (cbGain > tmp1)
   {
       state->onset = ONLENGTH;                                    
   }
   else
   {
        
       if (state->onset > 0)
       {
           state->onset = state->onset - 1;                   
       }
   }
   
   /* if not onset, check ltpGain buffer and use max phase dispersion if
      half or more of the ltpGain-parameters say so */
   
   if (state->onset == 0)
   {
       /* Check LTP gain memory and set filter accordingly */
       i1 = 0;                                                     
       for (i = 0; i < PHDGAINMEMSIZE; i++)
       {
           
           if (state->gainMem[i] < PHDTHR1LTP)
           {
               i1 = i1 + 1;
           }
       }
       
       if (i1 > 2)
       {
           impNr = 0;                                              
       }
       
   }
   /* Restrict decrease in phase dispersion to one step if not onset */
    
   if ((impNr > (state->prevState + 1)) && (state->onset == 0))
   {
       impNr = impNr - 1;
   }
   /* if onset, use one step less phase dispersion */
    
   if((impNr < 2) && (state->onset > 0))
   {
       impNr = impNr + 1;
   }
   
   /* disable for very low levels */
   
   if(cbGain < 10)
   {
       impNr = 2;                                                  
   }
   
   
   if(state->lockFull == 1)
   {
       impNr = 0;                                                  
   }

   /* update static memory */
   state->prevState = impNr;                                       
   state->prevCbGain = cbGain;                                     
  
   /* do phase dispersion for all modes but 12.2 and 7.4;
      don't modify the innovation if impNr >=2 (= no phase disp) */
      
   if ((mode != MR122) && 
       (mode != MR102) &&
       (mode != MR74)  &&
       (impNr < 2))
   {
       /* track pulse positions, save innovation,
          and initialize new innovation          */
       nze = 0;                                                    
       for (i = 0; i < L_SUBFR; i++)
       {
            
           if (inno[i] != 0)
           {
               ps_poss[nze] = i;                                   
               nze = add (nze, 1);
           }
           inno_sav[i] = inno[i];                                  
           inno[i] = 0;                                            
       }
       /* Choose filter corresponding to codec mode and dispersion criterium */
       
       if (mode == MR795)
       {
           
           if (impNr == 0)
           {
               ph_imp = ph_imp_low_MR795;                            
           }
           else
           {
               ph_imp = ph_imp_mid_MR795;                            
           }
       }
       else
       {
           
           if (impNr == 0)
           {
               ph_imp = ph_imp_low;                                  
           }
           else
           {
               ph_imp = ph_imp_mid;                                  
           }
       }
       
       /* Do phase dispersion of innovation */
       for (nPulse = 0; nPulse < nze; nPulse++)
       {
           ppos = ps_poss[nPulse];                                   
           
           /* circular convolution with impulse response */
           j = 0;                                                    
           for (i = ppos; i < L_SUBFR; i++)
           {
               /* inno[i1] += inno_sav[ppos] * ph_imp[i1-ppos] */
               tmp1 = mult(inno_sav[ppos], ph_imp[j++]);
               inno[i] = add(inno[i], tmp1);                         
           }    
           
           for (i = 0; i < ppos; i++)
           {
               /* inno[i] += inno_sav[ppos] * ph_imp[L_SUBFR-ppos+i] */
               tmp1 = mult(inno_sav[ppos], ph_imp[j++]);
               inno[i] = add(inno[i], tmp1);                         
           }
       }
   }
       
   /* compute total excitation for synthesis part of decoder
      (using modified innovation if phase dispersion is active) */
   for (i = 0; i < L_SUBFR; i++)
   {
       /* x[i] = gain_pit*x[i] + cbGain*code[i]; */
       L_temp = L_mult (        x[i],    pitch_fac);
                                                /* 12.2: Q0 * Q13 */
                                                /*  7.4: Q0 * Q14 */
       L_temp = L_mac  (L_temp, inno[i], cbGain);
                                                /* 12.2: Q12 * Q1 */
                                                /*  7.4: Q13 * Q1 */
       L_temp = L_shl (L_temp, tmp_shift);                 /* Q16 */           
       x[i] = round (L_temp);                                         
   }

   return;
}

