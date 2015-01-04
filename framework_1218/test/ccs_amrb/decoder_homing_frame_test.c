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
*     Function        : decoder_homing_frame_test
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*     Out             : none
*     Calls           : dhf_test
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing frame
*                          pattern
*                       1  input frame matches the decoder homing frame pattern
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are compared
*                       with all parameters of the corresponding decoder homing frame.
*
********************************************************************************
*/

Word16 decoder_homing_frame_test (Word16 input_frame[], enum Mode mode)
{
    /* perform test for COMPLETE parameter frame */
    return dhf_test(input_frame, mode, prmno[mode]);
}
