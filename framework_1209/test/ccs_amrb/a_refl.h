/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef a_refl_h
#define a_refl_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 *   FUNCTION:  A_Refl()
 *
 *   PURPOSE: Convert from direct form coefficients to reflection coefficients
 *
 *   DESCRIPTION:
 *       Directform coeffs in Q12 are converted to 
 *       reflection coefficients Q15 
 *
 *************************************************************************/
void A_Refl(
   Word16 a[],	      /* i   : Directform coefficients */
   Word16 refl[]      /* o   : Reflection coefficients */
);

#endif
