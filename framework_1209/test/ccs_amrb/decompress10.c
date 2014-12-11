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

void decompress10 (
   Word16 MSBs,        /* i : MSB part of the index                 */
   Word16 LSBs,        /* i : LSB part of the index                 */
   Word16 index1,      /* i : index for first pos in pos_index[]    */ 
   Word16 index2,      /* i : index for second pos in pos_index[]   */ 
   Word16 index3,      /* i : index for third pos in pos_index[]    */ 
   Word16 pos_indx[])  /* o : position of 3 pulses (decompressed)   */
{
   Word16 ia, ib, ic;

   /*
     pos_indx[index1] = ((MSBs-25*(MSBs/25))%5)*2 + (LSBs-4*(LSBs/4))%2;
     pos_indx[index2] = ((MSBs-25*(MSBs/25))/5)*2 + (LSBs-4*(LSBs/4))/2;
     pos_indx[index3] = (MSBs/25)*2 + LSBs/4;
     */

   
   if (sub(MSBs, 124) > 0)
   {
      MSBs = 124;                                               
   }
   
   ia = mult(MSBs, 1311);
   ia = sub(MSBs, extract_l(L_shr(L_mult(ia, 25), 1)));    
   ib = shl(sub(ia, extract_l(L_shr(L_mult(mult(ia, 6554), 5), 1))), 1);
   
   ic = shl(shr(LSBs, 2), 2);
   ic = sub(LSBs, ic);
   pos_indx[index1] = add(ib, (ic & 1));                        
   
   ib = shl(mult(ia, 6554), 1);
   pos_indx[index2] = add(ib, shr(ic, 1));
   
   pos_indx[index3] = add(shl(mult(MSBs, 1311), 1), shr(LSBs, 2));    

   return;
}    

