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
 
#include "table.h"          /* Look-up table for transformations */
 
/*************************************************************************
 *
 *   FUNCTIONS:  Lsp_lsf and Lsf_lsp
 *
 *   PURPOSE:
 *      Lsp_lsf:   Transformation lsp to lsf
 *      Lsf_lsp:   Transformation lsf to lsp
 *
 *   DESCRIPTION:
 *         lsp[i] = cos(2*pi*lsf[i]) and lsf[i] = arccos(lsp[i])/(2*pi)
 *
 *   The transformation from lsp[i] to lsf[i] and lsf[i] to lsp[i] are
 *   approximated by a look-up table and interpolation.
 *
 *************************************************************************/
void Lsp_lsf (
    Word16 lsp[],       /* (i)  : lsp[m] (range: -1<=val<1)                */
    Word16 lsf[],       /* (o)  : lsf[m] normalized (range: 0.0<=val<=0.5) */
    Word16 m            /* (i)  : LPC order                                */
)
{
    Word16 i, ind;
    Word32 L_tmp;

    ind = 63;                        /* begin at end of table -1 */

    for (i = m - 1; i >= 0; i--)
    {
        /* find value in table that is just greater than lsp[i] */
         
        while (sub (table_lsp_lsf[ind], lsp[i]) < 0)
        {
            ind--;
             
        }

        /* acos(lsp[i])= ind*256 + ( ( lsp[i]-table[ind] ) *
           slope[ind] )/4096 */

        L_tmp = L_mult (sub (lsp[i], table_lsp_lsf[ind]), slope_lsp_lsf[ind]);
        /*(lsp[i]-table[ind])*slope[ind])>>12*/
        lsf[i] = round (L_shl (L_tmp, 3));       
        lsf[i] = add (lsf[i], shl (ind, 8));     
    }
    return;
}
