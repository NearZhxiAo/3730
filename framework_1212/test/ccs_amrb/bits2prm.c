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
#include "mode.h"
#include "bin2int.h"
#include "table.h"

/*
**************************************************************************
*
*  Function    : Bits2prm
*  Purpose     : Retrieves the vector of encoder parameters from 
*                the received serial bits in a frame.
*
**************************************************************************
*/
void Bits2prm (
    enum Mode mode,     /* i : AMR mode                                    */
    Word16 bits[],      /* i : serial bits       (size <= MAX_SERIAL_SIZE) */
    Word16 prm[]        /* o : analysis parameters  (size <= MAX_PRM_SIZE) */
)
{
    Word16 i;

               /* account for pointer init (bitno[mode])  */
    
    for (i = 0; i < prmno[mode]; i++)
    {
        prm[i] = Bin2int (bitno[mode][i], bits);       
        bits += bitno[mode][i];
	    /* account for above pointer update  */
    }
    
   return;
}
