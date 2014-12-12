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
#include "cnst.h"

#define NB_PULSE10  10

/*************************************************************************
 *
 *  FUNCTION:  build_code_10i40_35bits()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 10 pulses.
 *
 *************************************************************************/

void build_code_10i40_35bits (
    Word16 codvec[], /* (i)  : position of pulses                           */
    Word16 sign[],   /* (i)  : sign of d[n]                                 */
    Word16 cod[],    /* (o)  : innovative code vector                       */
    Word16 h[],      /* (i)  : impulse response of weighted synthesis filter*/
    Word16 y[],      /* (o)  : filtered innovative code                     */
    Word16 indx[]    /* (o)  : index of 10 pulses (sign+position)           */
)
{
    Word16 i, j, k, track, index, _sign[NB_PULSE10];
    Word16 *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
    Word32 s;

    for (i = 0; i < L_CODE; i++)
    {
        cod[i] = 0;                               
    }
    for (i = 0; i < NB_TRACK; i++)
    {
        indx[i] = -1;                             
    }
    
    for (k = 0; k < NB_PULSE10; k++)
    {
        /* read pulse position */            
        i = codvec[k];                           
        /* read sign           */        
        j = sign[i];                              
        
        index = mult (i, 6554);                  /* index = pos/5       */
        /* track = pos%5 */
        track = sub (i, extract_l (L_shr (L_mult (index, 5), 1)));
         
        if (j > 0)
        {
            cod[i] = cod[i] + 4096;         
            _sign[k] = 8192;                      
            
        }
        else
        {
            cod[i] = cod[i] - 4096;         
            _sign[k] = -8192;                     
            index = index + 8;
        }
        
         
        if (indx[track] < 0)
        {
            indx[track] = index;                  
        }
        else
        {
               
            if (((index ^ indx[track]) & 8) == 0)
            {
                /* sign of 1st pulse == sign of 2nd pulse */
                
                 
                if (indx[track] <= index)
                {
                    indx[track + 5] = index;      
                }
                else
                {
                    indx[track + 5] = indx[track];
                                                  
                    indx[track] = index;          
                }
            }
            else
            {
                /* sign of 1st pulse != sign of 2nd pulse */
                
                   
                if (sub ((indx[track] & 7), (index & 7)) <= 0)
                {
                    indx[track + 5] = indx[track];
                                                  
                    indx[track] = index;          
                }
                else
                {
                    indx[track + 5] = index;      
                }
            }
        }
    }
    
    p0 = h - codvec[0];                           
    p1 = h - codvec[1];                           
    p2 = h - codvec[2];                           
    p3 = h - codvec[3];                           
    p4 = h - codvec[4];                           
    p5 = h - codvec[5];                           
    p6 = h - codvec[6];                           
    p7 = h - codvec[7];                           
    p8 = h - codvec[8];                           
    p9 = h - codvec[9];                           
     
    for (i = 0; i < L_CODE; i++)
    {
        s = 0;                                    
        s = L_mac (s, *p0++, _sign[0]);
        s = L_mac (s, *p1++, _sign[1]);
        s = L_mac (s, *p2++, _sign[2]);
        s = L_mac (s, *p3++, _sign[3]);
        s = L_mac (s, *p4++, _sign[4]);
        s = L_mac (s, *p5++, _sign[5]);
        s = L_mac (s, *p6++, _sign[6]);
        s = L_mac (s, *p7++, _sign[7]);
        s = L_mac (s, *p8++, _sign[8]);
        s = L_mac (s, *p9++, _sign[9]);
        y[i] = round (s);                         
    }
}

