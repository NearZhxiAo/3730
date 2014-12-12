/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Vq_subvec3_h
#define Vq_subvec3_h "$Id $"

#include "typedef.h"

Word16 Vq_subvec3(             /* o: quantization index,            Q0  */
    Word16 * lsf_r1,    /* i: 1st LSF residual vector,       Q15 */
    const Word16 * dico,      /* i: quantization codebook,         Q15 */
    Word16 * wf1,       /* i: 1st LSF weighting factors,     Q13 */
    Word16 dico_size,   /* i: size of quantization codebook, Q0  */
    Flag use_half       /* i: use every second entry in codebook */
	);
#endif
