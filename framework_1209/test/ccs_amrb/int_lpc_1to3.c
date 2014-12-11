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
 *
 *  FUNCTION:  Int_lpc_1to3()
 *
 *  PURPOSE:  Interpolates the LSPs and convert to LP parameters to get
 *            a different LP filter in each subframe.
 *
 *  DESCRIPTION:
 *     The 20 ms speech frame is divided into 4 subframes.
 *     The LSPs are quantized and transmitted at the 4th subframe
 *     (once per frame) and interpolated at the 1st, 2nd and 3rd subframe.
 *
 *          |------|------|------|------|
 *             sf1    sf2    sf3    sf4
 *       F0                          F1
 *
 *     sf1:   3/4 F0 + 1/4 F1         sf3:   1/4 F0 + 3/4 F1
 *     sf2:   1/2 F0 + 1/2 F1         sf4:       F1
 *
 *************************************************************************/
void Int_lpc_1to3(
    Word16 lsp_old[], /* input : LSP vector at the 4th SF of past frame    */
    Word16 lsp_new[], /* input : LSP vector at the 4th SF of present frame */
    Word16 Az[]       /* output: interpolated LP parameters in all SFs     */
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

    Lsp_Az(lsp, Az);       /* Subframe 3 */
    Az += MP1;              

    Lsp_Az(lsp_new, Az);        /* Subframe 4 */

    return;
}
