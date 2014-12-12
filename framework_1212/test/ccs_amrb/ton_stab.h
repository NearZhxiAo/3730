/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef ton_stab_h
#define ton_stab_h "$Id $"
 
#include "typedef.h"
#include "mode.h"
#include "cnst.h"

/* state variable */
typedef struct {

   /* counters */
   Word16 count;
   
   /* gain history Q11 */
   Word16 gp[N_FRAME];
   
} tonStabState;

#endif
