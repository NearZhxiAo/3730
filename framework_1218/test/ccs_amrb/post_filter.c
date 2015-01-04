/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "mode.h"
#include "basic_op.h"
#include "set_zero.h"
#include "weight_ai.h"
#include "residu.h"
#include "copy.h"
#include "syn_filt.h"
#include "preemphasis.h"
#include "cnst.h"
#include "table.h"
#include "post_filter.h"

#define L_H 22  /* size of truncated impulse response of A(z/g1)/A(z/g2) */

/*
**************************************************************************
*  Function:  Post_Filter
*  Purpose:   postfiltering of synthesis speech.
*  Description:
*      The postfiltering process is described as follows:
*
*          - inverse filtering of syn[] through A(z/0.7) to get res2[]
*          - tilt compensation filtering; 1 - MU*k*z^-1
*          - synthesis filtering through 1/A(z/0.75)
*          - adaptive gain control
*
**************************************************************************
*/
int Post_Filter (
    Post_FilterState *st, /* i/o : post filter states                        */
    enum Mode mode,       /* i   : AMR mode                                  */
    Word16 *syn,          /* i/o : synthesis speech (postfiltered is output) */
    Word16 *Az_4          /* i   : interpolated LPC parameters in all subfr. */
)
{
    /*-------------------------------------------------------------------*
     *           Declaration of parameters                               *
     *-------------------------------------------------------------------*/

    Word16 Ap3[MP1], Ap4[MP1];  /* bandwidth expanded LP parameters */
    Word16 *Az;                 /* pointer to Az_4:                 */
                                /*  LPC parameters in each subframe */
    Word16 i_subfr;             /* index for beginning of subframe  */
    Word16 h[L_H];

    Word16 i;
    Word16 temp1, temp2;
    Word32 L_tmp;
    Word16 *syn_work = &st->synth_buf[M]; 
    

    /*-----------------------------------------------------*
     * Post filtering                                      *
     *-----------------------------------------------------*/

    Copy (syn, syn_work , L_FRAME);

    Az = Az_4;

    for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
    {
       /* Find weighted filter coefficients Ap3[] and ap[4] */

        
       if ((mode == MR122) || (mode == MR102))
       {
          Weight_Ai (Az, gamma3_MR122, Ap3);
          Weight_Ai (Az, gamma4_MR122, Ap4);
       }
       else
       {
          Weight_Ai (Az, gamma3, Ap3);
          Weight_Ai (Az, gamma4, Ap4);
       }
       
       /* filtering of synthesis speech by A(z/0.7) to find res2[] */
       
       Residu (Ap3, &syn_work[i_subfr], st->res2, L_SUBFR);
       
       /* tilt compensation filter */
       
       /* impulse response of A(z/0.7)/A(z/0.75) */

       Copy (Ap3, h, M + 1);
       Set_zero (&h[M + 1], L_H - M - 1);
       Syn_filt (Ap4, h, h, L_H, &h[M + 1], 0);
        
       /* 1st correlation of h[] */
       
       L_tmp = L_mult (h[0], h[0]);
       for (i = 1; i < L_H; i++)
       {
          L_tmp = L_mac (L_tmp, h[i], h[i]);
       }
       temp1 = extract_h (L_tmp);
       
       L_tmp = L_mult (h[0], h[1]);
       for (i = 1; i < L_H - 1; i++)
       {
          L_tmp = L_mac (L_tmp, h[i], h[i + 1]);
       }
       temp2 = extract_h (L_tmp);
       
        
       if (temp2 <= 0)
       {
          temp2 = 0;           
       }
       else
       {
          temp2 = mult (temp2, MU);
          temp2 = div_s (temp2, temp1);
       }
       
       preemphasis (st->preemph_state, st->res2, temp2, L_SUBFR);
       
       /* filtering through  1/A(z/0.75) */
       
       Syn_filt (Ap4, st->res2, &syn[i_subfr], L_SUBFR, st->mem_syn_pst, 1);
       
       /* scale output to input */

       agc (st->agc_state, &syn_work[i_subfr], &syn[i_subfr],
            AGC_FAC, L_SUBFR);
       
       Az += MP1;
    }
    
    /* update syn_work[] buffer */
    
    Copy (&syn_work[L_FRAME - M], &syn_work[-M], M);
    
    return 0;
}
