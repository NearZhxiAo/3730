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
 *     Function     : first_filter_stage
 *     Purpose      : Scale input down by one bit. Calculate 5th order
 *                    half-band lowpass/highpass filter pair with
 *                    decimation.
 *
 ***************************************************************************/
void first_filter_stage(Word16 in[],  /* i   : input signal                  */
                               Word16 out[], /* o   : output values, every other    */
                                             /*       output is low-pass part and   */
                                             /*       other is high-pass part every */
                               Word16 data[] /* i/o : filter memory                 */
                               )
{
  Word16 temp0, temp1, temp2, temp3, i;
  Word16 data0, data1;

  data0 = data[0];                                          
  data1 = data[1];                                          
 
  for (i = 0; i < FRAME_LEN/4; i++)
  {
     temp0 = sub(shr(in[4*i+0], 2), mult(COEFF5_1, data0));
     temp1 = add(data0, mult(COEFF5_1, temp0));
     
     temp3 = sub(shr(in[4*i+1], 2), mult(COEFF5_2, data1));
     temp2 = add(data1, mult(COEFF5_2, temp3));
     
     out[4*i+0] = add(temp1, temp2);                        
     out[4*i+1] = sub(temp1, temp2);                        
     
     data0 = sub(shr(in[4*i+2], 2), mult(COEFF5_1, temp0));
     temp1 = add(temp0, mult(COEFF5_1, data0));
     
     data1 = sub(shr(in[4*i+3], 2), mult(COEFF5_2, temp3));
     temp2 = add(temp3, mult(COEFF5_2, data1));
     
     out[4*i+2] = add(temp1, temp2);                       
     out[4*i+3] = sub(temp1, temp2);                       
  } 
  
  data[0] = data0;                                         
  data[1] = data1;                                         
}
