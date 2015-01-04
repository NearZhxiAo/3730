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
#include "table.h"

/*************************************************************************
 *
 *  FUNCTION:  compress10()
 *
 *  PURPOSE: compression of three indeces [0..9] to one 10 bit index   
 *           minimizing the phase shift of a bit error.
 *
 *************************************************************************/


Word16 compress10 (
       Word16 pos_indxA, /* i : signs of 4 pulses (signs only)             */
       Word16 pos_indxB,  /* i : position index of 8 pulses (pos only)     */
       Word16 pos_indxC) /* i : position and sign of 8 pulses (compressed) */
{
   Word16 indx, ia,ib,ic;

   ia = shr(pos_indxA, 1);
   ib = extract_l(L_shr(L_mult(shr(pos_indxB, 1), 5), 1));
   ic = extract_l(L_shr(L_mult(shr(pos_indxC, 1), 25), 1));            
   indx = shl(add(ia, add(ib, ic)), 3);
   ia = pos_indxA & 1;                                
   ib = shl((pos_indxB & 1), 1);                      
   ic = shl((pos_indxC & 1), 2);                      
   indx = add(indx , add(ia, add(ib, ic)));  
   
   return indx;

}

