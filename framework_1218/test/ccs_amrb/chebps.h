/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Chebps_h
#define Chebps_h "$Id $"

#include "typedef.h"

/*
**************************************************************************
*
*  Function    : Chebps
*  Purpose     : Evaluates the Chebyshev polynomial series
*  Description : - The polynomial order is   n = m/2 = 5
*                - The polynomial F(z) (F1(z) or F2(z)) is given by
*                   F(w) = 2 exp(-j5w) C(x)
*                  where
*                   C(x) = T_n(x) + f(1)T_n-1(x) + ... +f(n-1)T_1(x) + f(n)/2
*                  and T_m(x) = cos(mw) is the mth order Chebyshev
*                  polynomial ( x=cos(w) )
*  Returns     : C(x) for the input x.
*
**************************************************************************
*/
Word16 Chebps (
			   Word16 x,
			   Word16 f[], /* (n) */
			   Word16 n
			   );
#endif
