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
#include "preemphasis.h"
/*
**************************************************************************
*  Function:  preemphasis
*  Purpose:   Filtering through 1 - g z^-1 
*
**************************************************************************
*/
int preemphasis (
    preemphasisState *st, /* (i/o)  : preemphasis filter state    */
    Word16 *signal, /* (i/o)   : input signal overwritten by the output */
    Word16 g,       /* (i)     : preemphasis coefficient                */
    Word16 L        /* (i)     : size of filtering                      */
)
{
    Word16 *p1, *p2, temp, i;

    p1 = signal + L - 1;                     
    p2 = p1 - 1;                             
    temp = *p1;                              

    for (i = 0; i <= L - 2; i++)
    {
        *p1 = sub (*p1, mult (g, *p2--));    
        p1--;
    }

    *p1 = sub (*p1, mult (g, st->mem_pre));      

    st->mem_pre = temp;                          

    return 0;
}
