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
#include "cnst.h"
#include "lsp_az.h" 

/*************************************************************************
 * Function Int_lpc_1to3_2()
 * Interpolation of the LPC parameters.
 * Same as the previous function but we do not recompute Az() for
 * subframe 4 because it is already available.
 *************************************************************************/

void Int_lpc_1to3_2(
     Word16 lsp_old[],  /* input : LSP vector at the 4th SF of past frame    */
     Word16 lsp_new[],  /* input : LSP vector at the 4th SF of present frame */
     Word16 Az[]        /* output: interpolated LP parameters in SFs 1,2,3   */
)
{
    Word16 i;
    Word16 lsp[M];

    for (i = 0; i < M; i++) 
	{
        lsp[i] = (lsp_new[i] >> 2) + (lsp_old[i] - (lsp_old[i] >> 2));
		                    
    }

    Lsp_Az(lsp, Az);        /* Subframe 1 */
    Az += MP1;              

    for (i = 0; i < M; i++) 
	{
        lsp[i] = (lsp_old[i] >> 1) + (lsp_new[i] >> 1);
		                    
    }

    Lsp_Az(lsp, Az);        /* Subframe 2 */
    Az += MP1;              

    for (i = 0; i < M; i++) 
	{
        lsp[i] = (lsp_old[i] >> 2) + (lsp_new[i] - (lsp_new[i] >> 2));
		                    
    }

    Lsp_Az(lsp, Az);        /* Subframe 3 */

    return;
}
