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
#include "inv_sqrt.h"
#include "cnst.h" 
 
/*************************************************************************
 *
 *  FUNCTION  set_sign()
 *
 *  PURPOSE: Builds sign[] vector according to "dn[]" and "cn[]".
 *           Also finds the position of maximum of correlation in each track
 *           and the starting position for each pulse.
 *
 *************************************************************************/
void set_sign(Word16 dn[],   /* i/o : correlation between target and h[]    */
              Word16 sign[], /* o   : sign of dn[]                          */
              Word16 dn2[],  /* o   : maximum of correlation in each track. */
              Word16 n       /* i   : # of maximum correlations in dn2[]    */
)
{
   Word16 i, j, k;
   Word16 val, min;
   Word16 pos = 0; /* initialization only needed to keep gcc silent */
   
   /* set sign according to dn[] */
   
   for (i = 0; i < L_CODE; i++) 
   {
      val = dn[i];                                 
      
      
      if (val >= 0)
	  {
         sign[i] = 32767;                          
      } 
	  else 
	  {
         sign[i] = -32767;                         
         val = negate(val);
      }
      dn[i] = val;     /* modify dn[] according to the fixed sign */
      dn2[i] = val;   
   }
   
   /* keep 8-n maximum positions/8 of each track and store it in dn2[] */
   
   for (i = 0; i < NB_TRACK; i++)
   {
      for (k = 0; k < (8-n); k++)
      {
         min = 0x7fff;                             
         for (j = i; j < L_CODE; j += STEP)
         {
                                           
            if (dn2[j] >= 0)
            {
               if (dn2[j] < min)
               {
                  min = dn2[j];                    
                  pos = j;                         
               }
            }
         }
         dn2[pos] = -1;                            
      }
   }
   
   return;
}

