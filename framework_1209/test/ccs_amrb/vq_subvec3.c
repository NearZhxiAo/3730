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
#include "basic_op.h"
#include "cnst.h"

Word16 Vq_subvec3(             /* o: quantization index,            Q0  */
    Word16 * lsf_r1,    /* i: 1st LSF residual vector,       Q15 */
    const Word16 * dico,      /* i: quantization codebook,         Q15 */
    Word16 * wf1,       /* i: 1st LSF weighting factors,     Q13 */
    Word16 dico_size,   /* i: size of quantization codebook, Q0  */
    Flag use_half)      /* i: use every second entry in codebook */
{
    Word16 i, index = 0;
    const Word16 *p_dico;
    Word16 temp;
    Word32 dist_min, dist;

    dist_min = MAX_32;                                             
    p_dico = dico;                                                 

    
    if (use_half == 0) 
	{
       for (i = 0; i < dico_size; i++)
       {
          temp = sub(lsf_r1[0], *p_dico++);
          temp = mult(wf1[0], temp);
          dist = L_mult(temp, temp);
          
          temp = sub(lsf_r1[1], *p_dico++);
          temp = mult(wf1[1], temp);
          dist = L_mac(dist, temp, temp);
          
          temp = sub(lsf_r1[2], *p_dico++);
          temp = mult(wf1[2], temp);
          dist = L_mac(dist, temp, temp);

          
          if (dist < dist_min)
		  {
             dist_min = dist;                                   
             index = i;                                         
          }
       }
       p_dico = &dico[add(index, add(index, index))];             
    }
    else
    {
       for (i = 0; i < dico_size; i++)
       {
          temp = sub(lsf_r1[0], *p_dico++);
          temp = mult(wf1[0], temp);
          dist = L_mult(temp, temp);

          temp = sub(lsf_r1[1], *p_dico++);
          temp = mult(wf1[1], temp);
          dist = L_mac(dist, temp, temp);
          
          temp = sub(lsf_r1[2], *p_dico++);
          temp = mult(wf1[2], temp);
          dist = L_mac(dist, temp, temp);
          
          
          if (dist < dist_min)
          {
             dist_min = dist;                                   
             index = i;                                         
          }
          p_dico = p_dico + 3; 
       }
       p_dico = &dico[index * 6];      
    }
    

    /* Reading the selected vector */
    lsf_r1[0] = *p_dico++;                                         
    lsf_r1[1] = *p_dico++;                                         
    lsf_r1[2] = *p_dico++;                                         

    return index;
}
