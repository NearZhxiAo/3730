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
 *     Function     : filter5
 *     Purpose      : Fifth-order half-band lowpass/highpass filter pair with
 *                    decimation.
 *
 ***************************************************************************/
void filter5(Word16 *in0,    /* i/o : input values; output low-pass part  */
                    Word16 *in1,    /* i/o : input values; output high-pass part */
                    Word16 data[]   /* i/o : updated filter memory               */
                    )
{
  Word16 temp0, temp1, temp2;

  temp0 = sub(*in0, mult(COEFF5_1, data[0]));
  temp1 = add(data[0], mult(COEFF5_1, temp0));
  data[0] = temp0;                                

  temp0 = sub(*in1, mult(COEFF5_2, data[1]));
  temp2 = add(data[1], mult(COEFF5_2, temp0));
  data[1] = temp0;                                

  *in0 = shr(add(temp1, temp2), 1);               
  *in1 = shr(sub(temp1, temp2), 1);               
}
