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
#include "table.h"

void q_p (
    Word16 *ind,        /* Pulse position */
    Word16 n            /* Pulse number   */
)
{
    Word16 tmp;
    
    tmp = *ind;                                   
    
    
    if (n < 5)
    {
        *ind = (tmp & 0x8) | gray[tmp & 0x7];      
                                                  
    }
    else
    {
        *ind = gray[tmp & 0x7];                     
    }
}
