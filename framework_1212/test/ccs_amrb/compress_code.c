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
#include "inv_sqrt.h"
#include "compress10.h"
/*************************************************************************
 *
 *  FUNCTION:  compress_code()
 *
 *  PURPOSE: compression of the linear codewords to 4+three indeces  
 *           one bit from each pulse is made robust to errors by 
 *           minimizing the phase shift of a bit error.
 *           4 signs (one for each track) 
 *           i0,i4,i1 => one index (7+3) bits, 3   LSBs more robust
 *           i2,i6,i5 => one index (7+3) bits, 3   LSBs more robust
 *           i3,i7    => one index (5+2) bits, 2-3 LSbs more robust
 *
 *************************************************************************/
void compress_code (
    Word16 sign_indx[], /* i : signs of 4 pulses (signs only)             */
    Word16 pos_indx[],  /* i : position index of 8 pulses (position only) */
    Word16 indx[])      /* o : position and sign of 8 pulses (compressed) */
{
   Word16 i, ia, ib, ic;

   for (i = 0; i < NB_TRACK_MR102; i++)
   {
      indx[i] = sign_indx[i];                             
   }
    
    /* First index 
      indx[NB_TRACK] = (ia/2+(ib/2)*5 +(ic/2)*25)*8 + ia%2 + (ib%2)*2 + (ic%2)*4; */
    
   indx[NB_TRACK_MR102] = compress10(pos_indx[0],pos_indx[4],pos_indx[1]);

    /* Second index       
      indx[NB_TRACK+1] = (ia/2+(ib/2)*5 +(ic/2)*25)*8 + ia%2 + (ib%2)*2 + (ic%2)*4; */
    
    
   indx[NB_TRACK_MR102+1]= compress10(pos_indx[2],pos_indx[6],pos_indx[5]);
    
    /*
      Third index      
      if ((ib/2)%2 == 1)
        indx[NB_TRACK+2] = ((((4-ia/2) + (ib/2)*5)*32+12)/25)*4 + ia%2 + (ib%2)*2;
      else   
        indx[NB_TRACK+2] = ((((ia/2) +   (ib/2)*5)*32+12)/25)*4 + ia%2 + (ib%2)*2;
        */
    
    ib = shr(pos_indx[7], 1) & 1;                        
    
    if (sub(ib, 1) == 0)
       ia = sub(4, shr(pos_indx[3], 1));
    else
       ia = shr(pos_indx[3], 1);

    ib = extract_l(L_shr(L_mult(shr(pos_indx[7], 1), 5), 1));       
    ib = add(shl(add(ia, ib), 5), 12);
    ic = shl(mult(ib, 1311), 2);
    ia = pos_indx[3] & 1;                             
    ib = shl((pos_indx[7] & 1), 1);                   
    indx[NB_TRACK_MR102+2] = add(ia, add(ib, ic));
}
