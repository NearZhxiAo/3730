/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef decompress10_h
#define decompress10_h "$Id $"

#include "typedef.h"

void decompress10 (
   Word16 MSBs,        /* i : MSB part of the index                 */
   Word16 LSBs,        /* i : LSB part of the index                 */
   Word16 index1,      /* i : index for first pos in pos_index[]    */ 
   Word16 index2,      /* i : index for second pos in pos_index[]   */ 
   Word16 index3,      /* i : index for third pos in pos_index[]    */ 
   Word16 pos_indx[]  /* o : position of 3 pulses (decompressed)   */
   );
#endif
