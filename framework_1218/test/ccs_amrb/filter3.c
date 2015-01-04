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
 *     Function     : filter3
 *     Purpose      : Third-order half-band lowpass/highpass filter pair with
 *                    decimation.
 *     Return value : 
 *
 ***************************************************************************/
void filter3(Word16 *in0,   /* i/o : input values; output low-pass part  */ 
                    Word16 *in1,   /* i/o : input values; output high-pass part */
                    Word16 *data   /* i/o : updated filter memory               */
                    )
{
  Word16 temp1, temp2;

  temp1 = sub(*in1, mult(COEFF3, *data));
  temp2 = add(*data, mult(COEFF3, temp1));
  *data = temp1;                              

  *in1 = shr(sub(*in0, temp2), 1);            
  *in0 = shr(add(*in0, temp2), 1);            
}
