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
#include "cor_h.h"
#include "set_sign.h"
#include "table.h"

#define NB_PULSE4  4

/*************************************************************************
 *
 *  FUNCTION  search_4i40_17bits()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 4 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/

#define _1_2    (Word16)(32768L/2)
#define _1_4    (Word16)(32768L/4)
#define _1_8    (Word16)(32768L/8)
#define _1_16   (Word16)(32768L/16)

void search_4i40_17bits(
    Word16 dn[],         /* i : correlation between target and h[]  */
    Word16 dn2[],        /* i : maximum of corr. in each track.     */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation           */
    Word16 codvec[]      /* o : algebraic codebook vector           */
)
{
    Word16 i0, i1, i2, i3;
    Word16 ix = 0; /* initialization only needed to keep gcc silent */
    Word16 ps = 0; /* initialization only needed to keep gcc silent */
    Word16 i, pos, track, ipos[NB_PULSE4];
    Word16 psk, ps0, ps1, sq, sq1;
    Word16 alpk, alp, alp_16;
    Word32 s, alp0, alp1;
    
    /* Default value */
    psk = -1;            
    alpk = 1;            
    for (i = 0; i < NB_PULSE4; i++)
    {
       codvec[i] = i;    
    }

    for (track = 3; track < 5; track++) {
    /* fix starting position */

       ipos[0] = 0;     
       ipos[1] = 1;     
       ipos[2] = 2;     
       ipos[3] = track; 
       
       /*------------------------------------------------------------------*
        * main loop: try 4 tracks.                                         *
        *------------------------------------------------------------------*/
       
       for (i = 0; i < NB_PULSE4; i++)
       {
          /*----------------------------------------------------------------*
           * i0 loop: try 4 positions (use position with max of corr.).     *
           *----------------------------------------------------------------*/
          
           /* account for ptr. init. (rr[io]) */
          for (i0 = ipos[0]; i0 < L_CODE; i0 += STEP)
          {
              
             if (dn2[i0] >= 0)
             {
                ps0 = dn[i0];     
                alp0 = L_mult(rr[i0][i0], _1_4);
                
                /*----------------------------------------------------------------*
                 * i1 loop: 8 positions.                                          *
                 *----------------------------------------------------------------*/
                
                sq = -1;          
                alp = 1;          
                ps = 0;           
                ix = ipos[1];     
                    
                /* initialize 4 index for next loop. */
                /*-------------------------------------------------------------------*
                 *  These index have low complexity address computation because      *
                 *  they are, in fact, pointers with fixed increment.  For example,  *
                 *  "rr[i0][i3]" is a pointer initialized to "&rr[i0][ipos[3]]"      *
                 *  and incremented by "STEP".                                       *
                 *-------------------------------------------------------------------*/
                
                 /* account for ptr. init. (rr[i1]) */
                 /* account for ptr. init. (dn[i1]) */
                 /* account for ptr. init. (rr[io]) */
                for (i1 = ipos[1]; i1 < L_CODE; i1 += STEP)
                {
                   ps1 = add(ps0, dn[i1]);   /* idx increment = STEP */
                   
                   /* alp1 = alp0 + rr[i0][i1] + 1/2*rr[i1][i1]; */
                   
                   alp1 = L_mac(alp0, rr[i1][i1], _1_4); /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i0][i1], _1_2); /* idx incr = STEP */
                   
                   sq1 = mult(ps1, ps1);
                   
                   alp_16 = round(alp1);
                   
                   s = L_msu(L_mult(alp, sq1), sq, alp_16);
                   
                   
                   if (s > 0)
                   {
                      sq = sq1;         
                      ps = ps1;         
                      alp = alp_16;     
                      ix = i1;          
                   }
                }
                i1 = ix;                  
                
                /*----------------------------------------------------------------*
                 * i2 loop: 8 positions.                                          *
                 *----------------------------------------------------------------*/
                
                ps0 = ps;                 
                alp0 = L_mult(alp, _1_4);
                
                sq = -1;                  
                alp = 1;                  
                ps = 0;                   
                ix = ipos[2];             
                                    
                /* initialize 4 index for next loop (see i1 loop) */
                
                 /* account for ptr. init. (rr[i2]) */
                 /* account for ptr. init. (rr[i1]) */
                 /* account for ptr. init. (dn[i2]) */
                 /* account for ptr. init. (rr[io]) */
                for (i2 = ipos[2]; i2 < L_CODE; i2 += STEP)
                {
                   ps1 = add(ps0, dn[i2]); /* index increment = STEP */
                   
                   /* alp1 = alp0 + rr[i0][i2] + rr[i1][i2] + 1/2*rr[i2][i2]; */
                   
                   alp1 = L_mac(alp0, rr[i2][i2], _1_16); /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i1][i2], _1_8);  /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i0][i2], _1_8);  /* idx incr = STEP */
                   
                   sq1 = mult(ps1, ps1);
                   
                   alp_16 = round(alp1);
                   
                   s = L_msu(L_mult(alp, sq1), sq, alp_16);
                   
                   
                   if (s > 0)
                   {
                      sq = sq1;         
                      ps = ps1;         
                      alp = alp_16;     
                      ix = i2;          
                   }
                }
                i2 = ix;                  
                
                /*----------------------------------------------------------------*
                 * i3 loop: 8 positions.                                          *
                 *----------------------------------------------------------------*/
                
                ps0 = ps;                 
                alp0 = L_deposit_h(alp);
                
                sq = -1;                  
                alp = 1;                  
                ps = 0;                   
                ix = ipos[3];             
                                    
                /* initialize 5 index for next loop (see i1 loop) */
                
                 /* account for ptr. init. (rr[i3]) */
                 /* account for ptr. init. (rr[i2]) */
                 /* account for ptr. init. (rr[i1]) */
                 /* account for ptr. init. (dn[i3]) */
                 /* account for ptr. init. (rr[io]) */
                for (i3 = ipos[3]; i3 < L_CODE; i3 += STEP)
                {
                   ps1 = add(ps0, dn[i3]); /* index increment = STEP */
                   
                   /* alp1 = alp0 + rr[i0][i3] + rr[i1][i3] + rr[i2][i3] + 1/2*rr[i3][i3]; */
                   
                   alp1 = L_mac(alp0, rr[i3][i3], _1_16); /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i2][i3], _1_8);  /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i1][i3], _1_8);  /* idx incr = STEP */
                   alp1 = L_mac(alp1, rr[i0][i3], _1_8);  /* idx incr = STEP */
                   
                   sq1 = mult(ps1, ps1);
                   
                   alp_16 = round(alp1);
                   
                   s = L_msu(L_mult(alp, sq1), sq, alp_16);
                   
                   
                   if (s > 0)
                   {
                      sq = sq1;         
                      ps = ps1;         
                      alp = alp_16;     
                      ix = i3;          
                   }
                }
                
                
                /*----------------------------------------------------------------*
                 * memorise codevector if this one is better than the last one.   *
                 *----------------------------------------------------------------*/
                
                s = L_msu(L_mult(alpk, sq), psk, alp);
                
                
                if (s > 0)
                {
                   psk = sq;            
                   alpk = alp;          
                   codvec[0] = i0;      
                   codvec[1] = i1;      
                   codvec[2] = i2;      
                   codvec[3] = ix;      
                }
             }
          }
          
          /*----------------------------------------------------------------*
           * Cyclic permutation of i0,i1,i2 and i3.                         *
           *----------------------------------------------------------------*/
          
          pos = ipos[3];                   
          ipos[3] = ipos[2];               
          ipos[2] = ipos[1];               
          ipos[1] = ipos[0];               
          ipos[0] = pos;                   
       }
    }
    
    return;
}
