/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef lsf_wt_h
#define lsf_wt_h "$Id $"
 
#include "typedef.h"
#include "cnst.h"
 
 
void Lsf_wt (
    Word16 *lsf,         /* input : LSF vector                  */
    Word16 *wf);         /* output: square of weighting factors */
 
#endif
