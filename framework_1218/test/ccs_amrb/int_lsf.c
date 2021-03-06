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

/*
**************************************************************************
*
*  Function    : Int_lsf
*  Purpose     : Interpolates the LSFs for selected subframe
*  Description : The 20 ms speech frame is divided into 4 subframes.
*                The LSFs are interpolated at the 1st, 2nd and 3rd
*                subframe and only forwarded at the 4th subframe.
*
*                      |------|------|------|------|
*                         sf1    sf2    sf3    sf4
*                   F0                          F1
*      
*                 sf1:   3/4 F0 + 1/4 F1         sf3:   1/4 F0 + 3/4 F1
*                 sf2:   1/2 F0 + 1/2 F1         sf4:       F1
*  Returns     : void
*
**************************************************************************
*/
void Int_lsf(
    Word16 lsf_old[], /* i : LSF vector at the 4th SF of past frame          */
    Word16 lsf_new[], /* i : LSF vector at the 4th SF of present frame       */
    Word16 i_subfr,   /* i : Pointer to current sf (equal to 0,40,80 or 120) */
    Word16 lsf_out[]  /* o : interpolated LSF parameters for current sf      */
)
{
    Word16 i;

    if ( i_subfr == 0 )
    {
                 
       for (i = 0; i < M; i++) 
	   {
          lsf_out[i] = add(sub(lsf_old[i], shr(lsf_old[i], 2)), shr(lsf_new[i], 2));
          
       }
    }
    else if (i_subfr == 40)
    {
        
       for (i = 0; i < M; i++) 
	   {
          lsf_out[i] = add(shr(lsf_old[i],1), shr(lsf_new[i], 1) );
          
       }
    }
    else if (i_subfr == 80)
    {
         
       for (i = 0; i < M; i++) 
	   {
          lsf_out[i] = add(shr(lsf_old[i], 2), sub(lsf_new[i], shr(lsf_new[i], 2)));
          
       }
    }
    else if (i_subfr == 120)
    {
                              
       for (i = 0; i < M; i++) 
	   {
          lsf_out[i] = lsf_new[i];                        
       }
    }

    return;
}
