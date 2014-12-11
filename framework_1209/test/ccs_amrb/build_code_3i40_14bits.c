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

#define NB_PULSE3  3
/*************************************************************************
 *
 *  FUNCTION:  build_code_3i40_14bits()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 3 pulses.
 *
 *************************************************************************/

Word16 build_code_3i40_14bits(
    Word16 codvec[],  /* i : position of pulses                            */
    Word16 dn_sign[], /* i : sign of pulses                                */
    Word16 cod[],     /* o : innovative code vector                        */
    Word16 h[],       /* i : impulse response of weighted synthesis filter */
    Word16 y[],       /* o : filtered innovative code                      */
    Word16 sign[]     /* o : sign of 3 pulses                              */
)
{
    Word16 i, j, k, track, index, _sign[NB_PULSE3], indx, rsign;
    Word16 *p0, *p1, *p2;
    Word32 s;

    for (i = 0; i < L_CODE; i++) {
       cod[i] = 0;         
    }
    
    indx = 0;               
    rsign = 0;              
    for (k = 0; k < NB_PULSE3; k++)
    {
       i = codvec[k];        /* read pulse position */
       j = dn_sign[i];       /* read sign           */
       
       index = mult(i, 6554);    /* index = pos/5 */
                                 /* track = pos%5 */
       track = sub(i, extract_l(L_shr(L_mult(index, 5), 1)));
       
       
       if (track == 1)
          index = index << 4;
       else if (track == 2)
       {
          
          track = 2;                          
          index = index << 8;
       }
       else if (track == 3)
       {
           
          track = 1;                                   
          index = (index << 4) + 8;
       }
       else if (track == 4)
       {
            
          track = 2;                          
          index = (index << 8) + 128;
       }
       
       
       if (j > 0)
       {
          cod[i] = 8191;                       
          _sign[k] = 32767;                    
          rsign = rsign +(1 << track);
       } else {
          cod[i] = -8192;                      
          _sign[k] = (Word16) - 32768L;        
       }
       
       indx = indx + index;
    }
    *sign = rsign;                             

    p0 = h - codvec[0];                        
    p1 = h - codvec[1];                        
    p2 = h - codvec[2];                        
    
    for (i = 0; i < L_CODE; i++)
    {
       s = 0;                                  
       s = L_mac(s, *p0++, _sign[0]);
       s = L_mac(s, *p1++, _sign[1]);
       s = L_mac(s, *p2++, _sign[2]);
       y[i] = round(s);                        
    }
    
    return indx;
}
