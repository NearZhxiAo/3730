/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst_vad.h"
/****************************************************************************
 *
 *     Function     : level_calculation
 *     Purpose      : Calculate signal level in a sub-band. Level is calculated
 *                    by summing absolute values of the input data.
 *     Return value : signal level
 *
 ***************************************************************************/
Word16 level_calculation(
    Word16 data[],     /* i   : signal buffer                                    */
    Word16 *sub_level, /* i   : level calculate at the end of the previous frame */
                       /* o   : level of signal calculated from the last         */
                       /*       (count2 - count1) samples                        */
    Word16 count1,     /* i   : number of samples to be counted                  */
    Word16 count2,     /* i   : number of samples to be counted                  */
    Word16 ind_m,      /* i   : step size for the index of the data buffer       */
    Word16 ind_a,      /* i   : starting index of the data buffer                */
    Word16 scale       /* i   : scaling for the level calculation                */
    )
{
  Word32 l_temp1, l_temp2;
  Word16 level, i;

  l_temp1 = 0L;                                           
  for (i = count1; i < count2; i++)
  {
     l_temp1 = L_mac(l_temp1, 1, abs_s(data[ind_m*i+ind_a]));
  }
  
  l_temp2 = L_add(l_temp1, L_shl(*sub_level, sub(16, scale)));
  *sub_level = extract_h(L_shl(l_temp1, scale));
  
  for (i = 0; i < count1; i++)
  {
     l_temp2 = L_mac(l_temp2, 1, abs_s(data[ind_m*i+ind_a]));
  }
  level = extract_h(L_shl(l_temp2, scale));
  
  return level;
}
