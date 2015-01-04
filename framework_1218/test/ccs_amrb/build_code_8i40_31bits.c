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


#define NB_PULSE8 8
/* define values/representation for output codevector and sign */
#define POS_CODE  8191 
#define NEG_CODE  8191 
#define POS_SIGN  32767
#define NEG_SIGN  (Word16) (-32768L) 

/*************************************************************************
 *
 *  FUNCTION:  build_code_8i40_31bits()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and a 
 *   linear uncombined version of  the index of the
 *           codevector, based on the signs and positions of 8  pulses.
 *
 *************************************************************************/

void build_code_8i40_31bits (
    Word16 codvec[],    /* i : position of pulses                           */
    Word16 sign[],      /* i : sign of d[n]                                 */
    Word16 cod[],       /* o : innovative code vector                       */
    Word16 h[],         /* i : impulse response of weighted synthesis filter*/
    Word16 y[],         /* o : filtered innovative code                     */
    Word16 sign_indx[], /* o : signs of 4  pulses (signs only)              */
    Word16 pos_indx[]   /* o : position index of 8 pulses(position only)    */
)
{
    Word16 i, j, k, track, sign_index, pos_index, _sign[NB_PULSE8];
    Word16 *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7;
    Word32 s;

    for (i = 0; i < L_CODE; i++)
    {
        cod[i] = 0;                               
    }
    for (i = 0; i < NB_TRACK_MR102; i++)
    {
        pos_indx[i] = -1;                             
        sign_indx[i] = -1;                             
    }
    
    for (k = 0; k < NB_PULSE8; k++)
    {
       /* read pulse position */            
       i = codvec[k];                           
       /* read sign           */        
       j = sign[i];                              
       
       pos_index = i >> 2;                      /* index = pos/4 */
       track = i & 3;                   /* track = pos%4 */
       
        
       if (j > 0)
       {
          cod[i] = cod[i] + POS_CODE;         
          _sign[k] = POS_SIGN;                      
          sign_index = 0;  /* bit=0 -> positive pulse */   
       }
       else
       {
          cod[i] = cod[i] - NEG_CODE;         
          _sign[k] = NEG_SIGN;                      
          sign_index = 1;      /* bit=1 => negative pulse */ 
          /* index = add (index, 8); 1 = negative  old code */
       }
       
        
       if (pos_indx[track] < 0)
       {   /* first set first NB_TRACK pulses  */
          pos_indx[track] = pos_index;                  
          sign_indx[track] = sign_index;               
       }
       else
       {   /* 2nd row of pulses , test if positions needs to be switched */
             
          if (((sign_index ^ sign_indx[track]) & 1) == 0)
          {
             /* sign of 1st pulse == sign of 2nd pulse */
             
              
             if (sub (pos_indx[track], pos_index) <= 0)
             {   /* no swap */
                pos_indx[track + NB_TRACK_MR102] = pos_index;      
             }
             else
             {   /* swap*/
                pos_indx[track + NB_TRACK_MR102] = pos_indx[track];
                 
                
                pos_indx[track] = pos_index;         
                sign_indx[track] = sign_index;       
             }
          }
          else
          {
             /* sign of 1st pulse != sign of 2nd pulse */
             
              
             if (pos_indx[track] <= pos_index)
             {  /*swap*/
                pos_indx[track + NB_TRACK_MR102] = pos_indx[track];
                 
                
                pos_indx[track] = pos_index;          
                sign_indx[track] = sign_index;        
             }
             else
             {   /*no swap */
                pos_indx[track + NB_TRACK_MR102] = pos_index;      
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
       y[i] = round (s);                         
    }
}
