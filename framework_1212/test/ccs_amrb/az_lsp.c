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
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"
#include "chebps.h"
#define NC   M/2                  /* M = LPC order, NC = M/2 */

/*
**************************************************************************
*
*  Function    : Az_lsp
*  Purpose     : Compute the LSPs from  the LP coefficients
*
**************************************************************************
*/
void Az_lsp (
    Word16 a[],         /* (i)  : predictor coefficients (MP1)               */
    Word16 lsp[],       /* (o)  : line spectral pairs (M)                    */
    Word16 old_lsp[]    /* (i)  : old lsp[] (in case not found 10 roots) (M) */
)
{
    Word16 i, j, nf, ip;
    Word16 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
    Word16 x, y, sign, exp;
    Word16 *coef;
    Word16 f1[M / 2 + 1], f2[M / 2 + 1];
    Word32 t0;

    /*-------------------------------------------------------------*
     *  find the sum and diff. pol. F1(z) and F2(z)                *
     *    F1(z) <--- F1(z)/(1+z**-1) & F2(z) <--- F2(z)/(1-z**-1)  *
     *                                                             *
     * f1[0] = 1.0;                                                *
     * f2[0] = 1.0;                                                *
     *                                                             *
     * for (i = 0; i< NC; i++)                                     *
     * {                                                           *
     *   f1[i+1] = a[i+1] + a[M-i] - f1[i] ;                       *
     *   f2[i+1] = a[i+1] - a[M-i] + f2[i] ;                       *
     * }                                                           *
     *-------------------------------------------------------------*/

    f1[0] = 1024;                  /* f1[0] = 1.0 */
    f2[0] = 1024;                 /* f2[0] = 1.0 */

    for (i = 0; i < NC; i++)
    {
        t0 = L_mult (a[i + 1], 8192);   /* x = (a[i+1] + a[M-i]) >> 2  */
        t0 = L_mac (t0, a[M - i], 8192);
        x = extract_h (t0);
        /* f1[i+1] = a[i+1] + a[M-i] - f1[i] */
        f1[i + 1] = sub (x, f1[i]);

        t0 = L_mult (a[i + 1], 8192);   /* x = (a[i+1] - a[M-i]) >> 2 */
        t0 = L_msu (t0, a[M - i], 8192);
        x = extract_h (t0);
        /* f2[i+1] = a[i+1] - a[M-i] + f2[i] */
        f2[i + 1] = add (x, f2[i]);
    }

    /*-------------------------------------------------------------*
     * find the LSPs using the Chebychev pol. evaluation           *
     *-------------------------------------------------------------*/

    nf = 0;                        /* number of found frequencies */
    ip = 0;                         /* indicator for f1 or f2      */

    coef = f1;                     

    xlow = grid[0];                
    ylow = Chebps (xlow, coef, NC);

    j = 0;
    /* while ( (nf < M) && (j < grid_points) ) */
    while ((nf < M) && (j < grid_points))
    {
        j++;
        xhigh = xlow;              
        yhigh = ylow;              
        xlow = grid[j];            
        ylow = Chebps (xlow, coef, NC);
                                   

        if ((ylow * yhigh) <= (Word32) 0L)
        {

            /* divide 4 times the interval */

            for (i = 0; i < 4; i++)
            {
                /* xmid = (xlow + xhigh)/2 */
                xmid = add (shr (xlow, 1), shr (xhigh, 1));
                ymid = Chebps (xmid, coef, NC);


                if ((ylow * ymid) <= (Word32) 0L)
                {
                    yhigh = ymid;  
                    xhigh = xmid;  
                }
                else
                {
                    ylow = ymid;   
                    xlow = xmid;   
                }
            }

            /*-------------------------------------------------------------*
             * Linear interpolation                                        *
             *    xint = xlow - ylow*(xhigh-xlow)/(yhigh-ylow);            *
             *-------------------------------------------------------------*/

            x = sub (xhigh, xlow);
            y = sub (yhigh, ylow);

            
            if (y == 0)
            {
                xint = xlow;        
            }
            else
            {
                sign = y;           
                y = abs_s (y);
                exp = norm_s (y);
                y = shl (y, exp);
                y = div_s ((Word16) 16383, y);
                t0 = L_mult (x, y);
                t0 = L_shr (t0, sub (20, exp));
                y = extract_l (t0);     /* y= (xhigh-xlow)/(yhigh-ylow) */

                 
                if (sign < 0)
                    y = negate (y);

                t0 = L_mult (ylow, y);
                t0 = L_shr (t0, 11);
                xint = sub (xlow, extract_l (t0)); /* xint = xlow - ylow*y */
            }

            lsp[nf] = xint;         
            xlow = xint;            
            nf++;

             
            if (ip == 0)
            {
                ip = 1;             
                coef = f2;          
            }
            else
            {
                ip = 0;             
                coef = f1;          
            }
            ylow = Chebps (xlow, coef, NC);
                                    

        }
          
    }

    /* Check if M roots found */

     
    if (nf < M)
    {
        for (i = 0; i < M; i++)
        {
            lsp[i] = old_lsp[i];    
        }

    }
    return;
}

