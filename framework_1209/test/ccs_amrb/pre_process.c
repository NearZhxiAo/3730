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
#include "table.h"
#include "pre_process.h"
/*************************************************************************
 *
 *  FUNCTION:  Pre_Process()
 *
 *  PURPOSE: Preprocessing of input speech.
 *
 *  DESCRIPTION:
 *     - 2nd order high pass filtering with cut off frequency at 80 Hz.
 *     - Divide input by two.
 *
 *                                                                        
 * Algorithm:                                                             
 *                                                                        
 *  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b[2]*x[i-2]/2                    
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];                     
 *                                                                        
 *                                                                        
 *  Input is divided by two in the filtering process.
 *
 *************************************************************************/
int Pre_Process (
    Pre_ProcessState *st,
    Word16 signal[], /* input/output signal */
    Word16 lg)       /* lenght of signal    */
{
    Word16 i, x2;
    Word32 L_tmp;

    for (i = 0; i < lg; i++)
    {
        x2 = st->x1;                    
        st->x1 = st->x0;                
        st->x0 = signal[i];             

        /*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */
        /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */

        L_tmp = Mpy_32_16 (st->y1_hi, st->y1_lo, a_enc[1]);
        L_tmp = L_add (L_tmp, Mpy_32_16 (st->y2_hi, st->y2_lo, a_enc[2]));
        L_tmp = L_mac (L_tmp, st->x0, b_enc[0]);
        L_tmp = L_mac (L_tmp, st->x1, b_enc[1]);
        L_tmp = L_mac (L_tmp, x2, b_enc[2]);
        L_tmp = L_shl (L_tmp, 3);
        signal[i] = round (L_tmp);  

        st->y2_hi = st->y1_hi;              
        st->y2_lo = st->y1_lo;              
        L_Extract (L_tmp, &st->y1_hi, &st->y1_lo);
    }
    return 0;
}
