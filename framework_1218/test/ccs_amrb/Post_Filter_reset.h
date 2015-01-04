/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Post_Filter_reset_h
#define Post_Filter_reset_h "$Id $"

#include "post_filter.h"

/*************************************************************************
*
*  Function:   Post_Filter_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Post_Filter_reset (Post_FilterState *state);

#endif
