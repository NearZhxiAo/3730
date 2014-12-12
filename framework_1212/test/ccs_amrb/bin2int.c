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
#include "table.h"

/*
**************************************************************************
*
*  Function    : Bin2int
*  Purpose     : Read "no_of_bits" bits from the array bitstream[] 
*                and convert to integer.
*
**************************************************************************
*/
Word16 Bin2int ( /* Reconstructed parameter                      */
    Word16 no_of_bits,  /* input : number of bits associated with value */
    Word16 *bitstream   /* output: address where bits are written       */
)
{
    Word16 value, i, bit;

    value = 0;                                   
    for (i = 0; i < no_of_bits; i++)
    {
        value = value << 1;
        bit = *bitstream++;                      
         
		if (bit == BIT_1)
            value = value + 1;
    }
    return (value);
}
