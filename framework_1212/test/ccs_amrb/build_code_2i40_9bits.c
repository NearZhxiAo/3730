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
#include "basic_op.h"
#include "cnst.h"
#include "table.h"

#define NB_PULSE29  2

/*************************************************************************
 *
 *  FUNCTION:  build_code_2i40_9bits()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 2 pulses.
 *
 *************************************************************************/

Word16 build_code_2i40_9bits(
    Word16 subNr,     /* i : subframe number                               */
    Word16 codvec[],  /* i : position of pulses                            */
    Word16 dn_sign[], /* i : sign of pulses                                */
    Word16 cod[],     /* o : innovative code vector                        */
    Word16 h[],       /* i : impulse response of weighted synthesis filter */
    Word16 y[],       /* o : filtered innovative code                      */
    Word16 sign[]     /* o : sign of 2 pulses                              */
)
{
    Word16 i, j, k, track, first, index, _sign[NB_PULSE29], indx, rsign;
    Word16 *p0, *p1;
    const Word16 *pt;
    Word32 s;

    pt = &trackTable[add(subNr, shl(subNr, 2))];
	
    for (i = 0; i < L_CODE; i++) {
        cod[i] = 0;                                 
    }
    
    indx = 0; 
    rsign = 0; 
    for (k = 0; k < NB_PULSE29; k++) {
       i = codvec[k];    /* read pulse position */  
       j = dn_sign[i];   /* read sign           */  

       index = mult(i, 6554);    /* index = pos/5 */
                                 /* track = pos%5 */
       track = sub(i, extract_l(L_shr(L_mult(index, 5), 1)));
       
       first = pt[track];                           

       
       if (first == 0) {
          
          if (k == 0) {
             track = 0;                             
          } else {
             track = 1;                                        
             index = index << 3;
          }             
       } else {
          
          if (k == 0) {
             track = 0;                                         
             index = index + 64;  /* table bit is MSB */
          } else {
             track = 1;                                        
             index = index << 3;
          }             
       }

       
       if (j > 0) {
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
    
    for (i = 0; i < L_CODE; i++) {
       s = 0;                                       
       s = L_mac(s, *p0++, _sign[0]);
       s = L_mac(s, *p1++, _sign[1]);
       y[i] = round(s);                             
    }
    
    return indx;
}
