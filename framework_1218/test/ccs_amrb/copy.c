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

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *   FUNCTION:   Copy
 *
 *   PURPOSE:   Copy vector x[] to y[]
 *
 *
 *************************************************************************/
/*
**************************************************************************
*
*  Function    : Copy
*  Purpose     : Copy vector x[] to y[]
*
**************************************************************************
*/
void Copy (
    const Word16 x[],   /* i : input vector (L)  */
    Word16 y[],         /* o : output vector (L) */
    Word16 L            /* i : vector length     */
)
{
    Word16 i;

    for (i = 0; i < L; i++)
    {
        y[i] = x[i];             
    }

    return;
}
