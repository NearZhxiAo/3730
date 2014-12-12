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
#include "decompress10.h"
#include "cnst.h"

/*************************************************************************
 *
 *  FUNCTION:  decompress_code()
 *
 *  PURPOSE: decompression of the linear codewords to 4+three indeces  
 *           one bit from each pulse is made robust to errors by 
 *           minimizing the phase shift of a bit error.
 *           4 signs (one for each track) 
 *           i0,i4,i1 => one index (7+3) bits, 3   LSBs more robust
 *           i2,i6,i5 => one index (7+3) bits, 3   LSBs more robust
 *           i3,i7    => one index (5+2) bits, 2-3 LSbs more robust
 *
 *************************************************************************/
void decompress_code (
    Word16 indx[],      /* i : position and sign of 8 pulses (compressed) */
    Word16 sign_indx[], /* o : signs of 4 pulses (signs only)             */
    Word16 pos_indx[]   /* o : position index of 8 pulses (position only) */
)
{
    Word16 i, ia, ib, MSBs, LSBs, MSBs0_24;

    for (i = 0; i < NB_TRACK_MR102; i++)
    {
       sign_indx[i] = indx[i];                                   
    }
    
    /*
      First index: 10x10x10 -> 2x5x2x5x2x5-> 125x2x2x2 -> 7+1x3 bits 
      MSBs = indx[NB_TRACK]/8;
      LSBs = indx[NB_TRACK]%8;
      */
    MSBs = shr(indx[NB_TRACK_MR102], 3);
    LSBs = indx[NB_TRACK_MR102] & 7;                            
    decompress10 (MSBs, LSBs, 0, 4, 1, pos_indx);               
    
    /*
      Second index: 10x10x10 -> 2x5x2x5x2x5-> 125x2x2x2 -> 7+1x3 bits       
      MSBs = indx[NB_TRACK+1]/8;
      LSBs = indx[NB_TRACK+1]%8;
      */
    MSBs = shr(indx[NB_TRACK_MR102+1], 3);
    LSBs = indx[NB_TRACK_MR102+1] & 7;                          
    decompress10 (MSBs, LSBs, 2, 6, 5, pos_indx);               
    
    /*
      Third index: 10x10 -> 2x5x2x5-> 25x2x2 -> 5+1x2 bits    
      MSBs = indx[NB_TRACK+2]/4;
      LSBs = indx[NB_TRACK+2]%4;
      MSBs0_24 = (MSBs*25+12)/32;
      if ((MSBs0_24/5)%2==1)
         pos_indx[3] = (4-(MSBs0_24%5))*2 + LSBs%2;
      else
         pos_indx[3] = (MSBs0_24%5)*2 + LSBs%2;
      pos_indx[7] = (MSBs0_24/5)*2 + LSBs/2;
      */
    MSBs = shr(indx[NB_TRACK_MR102+2], 2);
    LSBs = indx[NB_TRACK_MR102+2] & 3;                          

    MSBs0_24 = shr(add(extract_l(L_shr(L_mult(MSBs, 25), 1)), 12), 5);
    
    ia = mult(MSBs0_24, 6554) & 1;
    ib = sub(MSBs0_24, extract_l(L_shr(L_mult(mult(MSBs0_24, 6554), 5), 1)));

    
    if (sub(ia, 1) == 0)
    {
       ib = sub(4, ib);
    }
    pos_indx[3] = add(shl(ib, 1), (LSBs & 1));                          
    
    ia = shl(mult(MSBs0_24, 6554), 1);
    pos_indx[7] = add(ia, shr(LSBs, 1));
}

