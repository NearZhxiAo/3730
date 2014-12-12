/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/

#include "typedef.h"
#include "mode.h"
#include "bits2prm.h"
#include "cnst.h"
#include "table.h"

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

Word16 dhf_test (Word16 input_frame[], enum Mode mode, Word16 nparms)
{
    Word16 i, j;
    Word16 param[MAX_PRM_SIZE];

        
    /* retrieve the encoded parameters from the received serial bits */
    Bits2prm(mode, input_frame, param);
    
    j = 0;

    /* check if the encoded parameters matches the parameters
       of the corresponding decoder homing frame */
    for (i = 0; i < nparms; i++)
    {
        j = param[i] ^ dhf[mode][i];

        if (j)
            break;
    }

    return !j;
}
