/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef dhf_test_h
#define dhf_test_h "$Id $"

#include "typedef.h"
#include "mode.h"

/*
********************************************************************************
*
*     Function        : dhf_test
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*                       nparms         number of parameters to check
*     Out             : none
*     Calls           : Bits2prm
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing
*                          frame pattern (up to nparms)
*                       1  input frame matches the decoder homing frame pattern
*                          (for the first nparms parameters)
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are compared
*                       with all parameters of the corresponding decoder homing frame.
*
********************************************************************************
*/

Word16 dhf_test (Word16 input_frame[], enum Mode mode, Word16 nparms);

#endif
