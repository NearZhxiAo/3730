/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/

#include "dhf_test.h"
#include "table.h"

/*
********************************************************************************
*
*     Function        : decoder_homing_frame_test_first
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*     Out             : none
*     Calls           : Bits2prm
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing frame
*                          pattern (up to and including the first subframe)
*                       1  input frame matches the decoder homing frame pattern
*                          (up to and including the first subframe)
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are
*                       compared with the parameters for LPC and first subframe
*                       of the decoder homing frame.
*
********************************************************************************
*/

Word16 decoder_homing_frame_test_first (Word16 input_frame[], enum Mode mode)
{
    /* perform test for FIRST SUBFRAME of parameter frame ONLY */
    return dhf_test(input_frame, mode, prmnofsf[mode]);
}
