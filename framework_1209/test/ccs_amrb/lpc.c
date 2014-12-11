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
#include "mode.h"
#include "table.h"

#include "autocorr.h"
#include "lag_window.h"
#include "levinson.h"
#include "lpc.h"

int lpc(
    lpcState *st,     /* i/o: State struct                */
    enum Mode mode,   /* i  : coder mode                  */
    Word16 x[],       /* i  : Input signal           Q15  */
    Word16 x_12k2[],  /* i  : Input signal (EFR)     Q15  */
    Word16 a[]        /* o  : predictor coefficients Q12  */
)
{
   Word16 rc[4];                  /* First 4 reflection coefficients Q15 */
   Word16 rLow[MP1], rHigh[MP1];  /* Autocorrelations low and hi      */
                                  /* No fixed Q value but normalized  */
                                  /* so that cgOverflow is avoided      */
   
   
   if (mode == MR122)
   {
       /* Autocorrelations */
       Autocorr(x_12k2, M, rHigh, rLow, window_160_80);              
       /* Lag windowing    */
       Lag_window(M, rHigh, rLow);                                   
       /* Levinson Durbin  */
       Levinson(st->levinsonSt, rHigh, rLow, &a[MP1], rc);     

       /* Autocorrelations */
       Autocorr(x_12k2, M, rHigh, rLow, window_232_8);                  
       /* Lag windowing    */
       Lag_window(M, rHigh, rLow);                                  
       /* Levinson Durbin  */
       Levinson(st->levinsonSt, rHigh, rLow, &a[MP1 * 3], rc); 
   }
   else
   {
       /* Autocorrelations */
       Autocorr(x, M, rHigh, rLow, window_200_40);                 
       /* Lag windowing    */
       Lag_window(M, rHigh, rLow);                                   
       /* Levinson Durbin  */
       Levinson(st->levinsonSt, rHigh, rLow, &a[MP1 * 3], rc); 
   }
   
   return 0;
}
