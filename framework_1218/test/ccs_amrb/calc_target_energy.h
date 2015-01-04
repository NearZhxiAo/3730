/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef calc_target_energy_h
#define calc_target_energy_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 * FUNCTION: calc_target_energy
 *
 * PURPOSE:  calculation of target energy
 *
 *      en = <xn, xn>
 *
 *************************************************************************/
void
calc_target_energy(
    Word16 xn[],     /* i: LTP target vector,                       Q0  */
    Word16 *en_exp,  /* o: optimum codebook gain (exponent part),   Q0  */
    Word16 *en_frac  /* o: optimum codebook gain (fraction part),   Q15 */
);
#endif