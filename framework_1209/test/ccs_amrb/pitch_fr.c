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
#include "oper_32b.h"
#include "cnst.h"
#include "inv_sqrt.h"
#include "table.h"

#include "enc_lag3.h"
#include "enc_lag6.h"
#include "interpol_3or6.h"
#include "convolve.h"
#include "pitch_fr.h"
#include "getrange.h"
#include "norm_corr.h"
#include "searchfrac.h"
/*************************************************************************
 *
 *  FUNCTION:   Pitch_fr()
 *
 *  PURPOSE: Find the pitch period with 1/3 or 1/6 subsample resolution
 *           (closed loop).
 *
 *  DESCRIPTION:
 *        - find the normalized correlation between the target and filtered
 *          past excitation in the search range.
 *        - select the delay with maximum normalized correlation.
 *        - interpolate the normalized correlation at fractions -3/6 to 3/6
 *          with step 1/6 around the chosen delay.
 *        - The fraction which gives the maximum interpolated value is chosen.
 *
 *************************************************************************/
Word16 Pitch_fr (        /* o   : pitch period (integer)                    */
    Pitch_frState *st,   /* i/o : State struct                              */
    enum Mode mode,      /* i   : codec mode                                */
    Word16 T_op[],       /* i   : open loop pitch lags                      */
    Word16 exc[],        /* i   : excitation buffer                      Q0 */
    Word16 xn[],         /* i   : target vector                          Q0 */
    Word16 h[],          /* i   : impulse response of synthesis and
                                  weighting filters                     Q12 */
    Word16 L_subfr,      /* i   : Length of subframe                        */
    Word16 i_subfr,      /* i   : subframe offset                           */
    Word16 *pit_frac,    /* o   : pitch period (fractional)                 */
    Word16 *resu3,       /* o   : subsample resolution 1/3 (=1) or 1/6 (=0) */
    Word16 *ana_index    /* o   : index of encoding                         */
)
{
    Word16 i;
    Word16 t_min, t_max;
    Word16 t0_min, t0_max;
    Word16 max, lag, frac;
    Word16 tmp_lag;
    Word16 *corr;
    Word16 corr_v[40];    /* Total length = t0_max-t0_min+1+2*L_INTER_SRCH */

    Word16 max_frac_lag;
    Word16 flag3, flag4;
    Word16 last_frac;
    Word16 delta_int_low, delta_int_range;
    Word16 delta_frc_low, delta_frc_range;
    Word16 pit_min;
    Word16 frame_offset;
    Word16 delta_search;

    /*-----------------------------------------------------------------------*
     *                      set mode specific variables                      *
     *-----------------------------------------------------------------------*/

    max_frac_lag    = mode_dep_parm[mode].max_frac_lag;           
    flag3           = mode_dep_parm[mode].flag3;                  
    frac            = mode_dep_parm[mode].first_frac;             
    last_frac       = mode_dep_parm[mode].last_frac;              
    delta_int_low   = mode_dep_parm[mode].delta_int_low;          
    delta_int_range = mode_dep_parm[mode].delta_int_range;        
    
    delta_frc_low   = mode_dep_parm[mode].delta_frc_low;          
    delta_frc_range = mode_dep_parm[mode].delta_frc_range;        
    pit_min         = mode_dep_parm[mode].pit_min;                
    
    /*-----------------------------------------------------------------------*
     *                 decide upon full or differential search               *
     *-----------------------------------------------------------------------*/
    
    delta_search = 1;                                             
    
     
    if ((i_subfr == 0) || (i_subfr == L_FRAME_BY2)) 
	{
      
        /* Subframe 1 and 3 */
      
          
        if (((mode != MR475) && (mode != MR515)) || (i_subfr != L_FRAME_BY2)) 
		{
        
            /* set t0_min, t0_max for full search */
            /* this is *not* done for mode MR475, MR515 in subframe 3 */
        
            delta_search = 0; /* no differential search */         
            
            /* calculate index into T_op which contains the open-loop */
            /* pitch estimations for the 2 big subframes */
            
            frame_offset = 1;                                      
            
            if (i_subfr == 0)
                frame_offset = 0;                                  
            
            /* get T_op from the corresponding half frame and */
            /* set t0_min, t0_max */
            
            getRange (T_op[frame_offset], delta_int_low, delta_int_range,
                      pit_min, PIT_MAX, &t0_min, &t0_max);
        }
        else {
            
            /* mode MR475, MR515 and 3. Subframe: delta search as well */
            getRange (st->T0_prev_subframe, delta_frc_low, delta_frc_range,
                      pit_min, PIT_MAX, &t0_min, &t0_max);
        }
    }
    else {
        
        /* for Subframe 2 and 4 */
        /* get range around T0 of previous subframe for delta search */
        
        getRange (st->T0_prev_subframe, delta_frc_low, delta_frc_range,
                  pit_min, PIT_MAX, &t0_min, &t0_max);
    }

    /*-----------------------------------------------------------------------*
     *           Find interval to compute normalized correlation             *
     *-----------------------------------------------------------------------*/

    t_min = sub (t0_min, L_INTER_SRCH);
    t_max = add (t0_max, L_INTER_SRCH);

    corr = &corr_v[-t_min];                                        

    /*-----------------------------------------------------------------------*
     * Compute normalized correlation between target and filtered excitation *
     *-----------------------------------------------------------------------*/

    Norm_Corr (exc, xn, h, L_subfr, t_min, t_max, corr);

    /*-----------------------------------------------------------------------*
     *                           Find integer pitch                          *
     *-----------------------------------------------------------------------*/

    max = corr[t0_min];                                            
    lag = t0_min;                                                  

    for (i = t0_min + 1; i <= t0_max; i++) 
	{
        
        if (corr[i] >= max) 
		{
            max = corr[i];                                         
            lag = i;                                               
        }
    }

    /*-----------------------------------------------------------------------*
     *                        Find fractional pitch                          *
     *-----------------------------------------------------------------------*/
     
    if ((delta_search == 0) && (lag > max_frac_lag)) 
	{

        /* full search and integer pitch greater than max_frac_lag */
        /* fractional search is not needed, set fractional to zero */

        frac = 0;                                                  
    }
    else 
	{

        /* if differential search AND mode MR475 OR MR515 OR MR59 OR MR67   */
        /* then search fractional with 4 bits resolution           */
       
            
       if ((delta_search != 0) &&
           ((mode == MR475) ||
            (mode == MR515) ||
            (mode == MR59) ||
            (mode == MR67))) 
	   {

          /* modify frac or last_frac according to position of last */
          /* integer pitch: either search around integer pitch, */
          /* or only on left or right side */
          
          tmp_lag = st->T0_prev_subframe;                          
          
          if ((tmp_lag - t0_min) > 5)
             tmp_lag = t0_min + 5;
          
          if ( (t0_max - tmp_lag) > 4)
               tmp_lag = t0_max - 4;
          
           
          if ((lag == tmp_lag) ||
              (lag == (tmp_lag - 1))) 
		  {
             
             /* normal search in fractions around T0 */
             
             searchFrac (&lag, &frac, last_frac, corr, flag3);
             
          }
          else if (lag == (tmp_lag - 2)) 
		  {
             
             /* limit search around T0 to the right side */
             frac = 0;                                            
             searchFrac (&lag, &frac, last_frac, corr, flag3);
          }
          else if (lag == (tmp_lag + 1)) 
		  {
              
             /* limit search around T0 to the left side */
             last_frac = 0;                                       
             searchFrac (&lag, &frac, last_frac, corr, flag3);
          }
          else 
		  {
              
             /* no fractional search */
             frac = 0;                                            
            }
       }
       else
          /* test the fractions around T0 */
          searchFrac (&lag, &frac, last_frac, corr, flag3);
    }
    
    /*-----------------------------------------------------------------------*
     *                           encode pitch                                *
     *-----------------------------------------------------------------------*/
    
    
    if (flag3 != 0) {       
       /* flag4 indicates encoding with 4 bit resolution;         */
       /* this is needed for mode MR475, MR515 and MR59           */
       
       flag4 = 0;                                                 
           
       if ( (mode == MR475) ||
            (mode == MR515) ||
            (mode == MR59) ||
            (mode == MR67) ) 
	   {
          flag4 = 1;                                              
       }
       
       /* encode with 1/3 subsample resolution */
       
       *ana_index = Enc_lag3(lag, frac, st->T0_prev_subframe,
                             t0_min, t0_max, delta_search, flag4);  /* function result */

    }  
    else
    {
       /* encode with 1/6 subsample resolution */
       
       *ana_index = Enc_lag6(lag, frac, t0_min, delta_search);  /* function result */
    }
    
    /*-----------------------------------------------------------------------*
     *                          update state variables                       *
     *-----------------------------------------------------------------------*/
    
    st->T0_prev_subframe = lag;                                    
    
    /*-----------------------------------------------------------------------*
     *                      update output variables                          *
     *-----------------------------------------------------------------------*/
    
    *resu3    = flag3;                                             

    *pit_frac = frac;                                              

    return (lag);
}
