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
)
{
    Word32 s;
    Word16 i, exp;

    /* Compute scalar product <xn[], xn[]> */
    s = L_mac(0L, xn[0], xn[0]);
    for (i = 1; i < L_SUBFR; i++)
        s = L_mac(s, xn[i], xn[i]);

    /* s = SUM 2*xn(i) * xn(i) = <xn xn> * 2 */
    exp = norm_l(s);
    *en_frac = extract_h(L_shl(s, exp));
    *en_exp = 16 - exp;    
}
