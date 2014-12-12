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

#include "table.h"

/*************************************************************************
 *
 *   FUNCTION NAME: pseudonoise
 *
 *************************************************************************/
Word16 pseudonoise (
    Word32 *shift_reg, /* i/o : Old CN generator shift register state */
    Word16 no_bits     /* i   : Number of bits                        */
)
{
   Word16 noise_bits, Sn, i;
   
   noise_bits = 0;                              
   for (i = 0; i < no_bits; i++)
   {
      /* State n == 31 */
      if ((*shift_reg & 0x00000001L) != 0)
      {
         Sn = 1;                                                         
      }
      else
      {
         Sn = 0;                                                        
      }
      
      /* State n == 3 */
      if ((*shift_reg & 0x10000000L) != 0)
      {
         Sn = Sn ^ 1;                            
      }
      else
      {
         Sn = Sn ^ 0;                              
      }
      
      noise_bits = shl (noise_bits, 1);
      noise_bits = noise_bits | (extract_l (*shift_reg) & 1);
        
      
      *shift_reg = L_shr (*shift_reg, 1);         
      
      if (Sn & 1)
      {
         *shift_reg = *shift_reg | 0x40000000L; 
      }
   }
   return noise_bits;
}

