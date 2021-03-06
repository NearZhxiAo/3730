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
#include "inv_sqrt.h"
#include "cnst.h"
#include "table.h"
#include "cor_h.h"
#include "set_sign.h"


#define NB_PULSE29  2

/*************************************************************************
 *
 *  FUNCTION  search_2i40_9bits()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 2 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/

#define _1_2    (Word16)(32768L/2)
#define _1_4    (Word16)(32768L/4)
#define _1_8    (Word16)(32768L/8)
#define _1_16   (Word16)(32768L/16)

void search_2i40_9bits(
    Word16 subNr,        /* i : subframe number                    */
    Word16 dn[],         /* i : correlation between target and h[] */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation          */
    Word16 codvec[]      /* o : algebraic codebook vector          */
)
{
    Word16 i0, i1;
    Word16 ix = 0; /* initialization only needed to keep gcc silent */
    Word16  track1, ipos[NB_PULSE29];
    Word16 psk, ps0, ps1, sq, sq1;
    Word16 alpk, alp, alp_16;
    Word32 s, alp0, alp1;
    Word16 i;    

    psk = -1;                     
    alpk = 1;                     
    for (i = 0; i < NB_PULSE29; i++)
    {
       codvec[i] = i;             
    }
 
    for (track1 = 0; track1 < 2; track1++) {		
       /* fix starting position */
       
       ipos[0] = startPos[subNr*2+8*track1];     
       ipos[1] = startPos[subNr*2+1+8*track1];             
       

          /*----------------------------------------------------------------*
           * i0 loop: try 8 positions.                                      *
           *----------------------------------------------------------------*/
          
			                   /* account for ptr. init. (rr[io]) */
          for (i0 = ipos[0]; i0 < L_CODE; i0 += STEP) {
             
             ps0 = dn[i0];                    
             alp0 = L_mult(rr[i0][i0], _1_4);
             
          /*----------------------------------------------------------------*
           * i1 loop: 8 positions.                                          *
           *----------------------------------------------------------------*/
             
             sq = -1;                         
             alp = 1;                         
             ix = ipos[1];                    
             
        /*-------------------------------------------------------------------*
        *  These index have low complexity address computation because      *
        *  they are, in fact, pointers with fixed increment.  For example,  *
        *  "rr[i0][i2]" is a pointer initialized to "&rr[i0][ipos[2]]"      *
        *  and incremented by "STEP".                                       *
        *-------------------------------------------------------------------*/
             
              /* account for ptr. init. (rr[i1]) */
              /* account for ptr. init. (dn[i1]) */
              /* account for ptr. init. (rr[io]) */
             for (i1 = ipos[1]; i1 < L_CODE; i1 += STEP) {
                ps1 = add(ps0, dn[i1]);   /* idx increment = STEP */
                
                /* alp1 = alp0 + rr[i0][i1] + 1/2*rr[i1][i1]; */
                   
                alp1 = L_mac(alp0, rr[i1][i1], _1_4); /* idx incr = STEP */
                alp1 = L_mac(alp1, rr[i0][i1], _1_2); /* idx incr = STEP */
                
                sq1 = mult(ps1, ps1);
                
                alp_16 = round(alp1);
                
                s = L_msu(L_mult(alp, sq1), sq, alp_16);

                
                if (s > 0) {
                   sq = sq1;                  
                   alp = alp_16;              
                   ix = i1;                   
                }
             }
             
          /*----------------------------------------------------------------*
           * memorise codevector if this one is better than the last one.   *
           *----------------------------------------------------------------*/
             
             s = L_msu(L_mult(alpk, sq), psk, alp);
             
             
             if (s > 0) {
                psk = sq;                     
                alpk = alp;                   
                codvec[0] = i0;               
                codvec[1] = ix;               
             }
          }
    }
    
    return;
}
