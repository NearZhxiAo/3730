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
#include "mode.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"

#include "table.h"
/*
**************************************************************************
*
*  Function    : d_gain_pitch
*  Purpose     : Decodes the pitch gain using the received index.
*                output is in Q14 
*
**************************************************************************
*/
Word16 d_gain_pitch (      /* return value: gain (Q14)                */
    enum Mode mode,        /* i   : AMR mode                          */   
    Word16 index           /* i   : index of quantization             */
)
{
    Word16 gain;

    
    if (sub(mode, MR122) == 0)
    {
       /* clear 2 LSBits */
       gain = shl (shr (qua_gain_pitch[index], 2), 2);    
    }
    else
    {
       gain = qua_gain_pitch[index];                       
    }
    
    return gain;
}
