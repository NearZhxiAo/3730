/*************************************************************************
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
#include "copy.h"
#include "set_zero.h"
#include "syn_filt.h"
#include "d_plsf.h"
#include "agc.h"
#include "dec_gain.h"
#include "dec_lag3.h"
#include "dec_lag6.h"
#include "d_gain_pitch.h"
#include "d_gain_code.h"
#include "dec_gain.h"
#include "ph_disp.h"
#include "int_lsf.h"
#include "lsp_lsf.h"
#include "lsf_lsp.h"
#include "lsp_avg.h"
#include "ex_ctrl.h"
#include "sqrt_l_exp.h"
#include "frame.h"
#include "table.h"
#include "decoder_amr.h"
#include "rx_dtx_handler.h"
#include "decoder_amr_reset.h"
#include "build_cn_param.h"
#include "d_plsf_3.h"
#include "d_plsf_5.h"
#include "int_lpc_1to3.h"
#include "int_lpc_1and3.h"
#include "decode_2i40_9bits.h"
#include "decode_2i40_11bits.h"
#include "decode_3i40_14bits.h"
#include "decode_4i40_17bits.h"
#include "dec_8i40_31bits.h"
#include "dec_10i40_35bits.h"
#include "ec_gain_pitch_update.h"
#include "ph_disp_release.h"
#include "ph_disp_lock.h"
#include "dtx_dec_activity_update.h"
#include "ec_gain_code_update.h"
#include "agc2.h"
/*
**************************************************************************
*
*  Function    : Decoder_amr
*  Purpose     : Speech decoder routine.
*
**************************************************************************
*/
int Decoder_amr (
    Decoder_amrState *st,      /* i/o : State variables                   */
    enum Mode mode,            /* i   : AMR mode                          */
    Word16 parm[],             /* i   : vector of synthesis parameters
                                        (PRM_SIZE)                        */
    enum RXFrameType frame_type, /* i   : received frame type             */
    Word16 synth[],            /* o   : synthesis speech (L_FRAME)        */
    Word16 A_t[]               /* o   : decoded LP filter in 4 subframes
                                        (AZ_SIZE)                         */
)
{
    /* LPC coefficients */
   
    Word16 *Az;                /* Pointer on A_t */
    
    /* LSPs */

    Word16 lsp_new[M];
    Word16 lsp_mid[M];

    /* LSFs */
    
    Word16 prev_lsf[M];
    Word16 lsf_i[M];    

    /* Algebraic codevector */

    Word16 code[L_SUBFR];

    /* excitation */

    Word16 excp[L_SUBFR];
    Word16 exc_enhanced[L_SUBFR];

    /* Scalars */

    Word16 i, i_subfr;
    Word16 T0, T0_frac, index, index_mr475 = 0;
    Word16 gain_pit, gain_code, gain_code_mix, pit_sharp, pit_flag, pitch_fac;
    Word16 t0_min, t0_max;
    Word16 delta_frc_low, delta_frc_range;
    Word16 tmp_shift;
    Word16 temp;
    Word32 L_temp;
    Word16 flag4;
    Word16 carefulFlag;
    Word16 excEnergy;
    Word16 subfrNr; 
    Word16 evenSubfr = 0;

    Word16 bfi = 0;   /* bad frame indication flag                          */
    Word16 pdfi = 0;  /* potential degraded bad frame flag                  */

    enum DTXStateType newDTXState;  /* SPEECH , DTX, DTX_MUTE */

    /* find the new  DTX state  SPEECH OR DTX */
    newDTXState = rx_dtx_handler(st->dtxDecoderState, frame_type);
       /* function result */
    
    /* DTX actions */
    
    if (sub(newDTXState, SPEECH) != 0 )
    {
       Decoder_amr_reset (st, MRDTX);

       dtx_dec(st->dtxDecoderState, 
               st->mem_syn, 
               st->lsfState, 
               st->pred_state,
               st->Cb_gain_averState,
               newDTXState,
               mode, 
               parm, synth, A_t);
       /* update average lsp */
       
       Lsf_lsp(st->lsfState->past_lsf_q, st->lsp_old, M);
       lsp_avg(st->lsp_avg_st, st->lsfState->past_lsf_q);
       goto the_end;
    }

    /* SPEECH action state machine  */
       
    if ((sub(frame_type, RX_SPEECH_BAD) == 0) || 
        (sub(frame_type, RX_NO_DATA) == 0) ||
        (sub(frame_type, RX_ONSET) == 0)) 
    {
       bfi = 1;                                          
        
       if ((sub(frame_type, RX_NO_DATA) == 0) ||
           (sub(frame_type, RX_ONSET) == 0))
       {
	  build_CN_param(&st->nodataSeed,
			 prmno[mode],
			 bitno[mode],
			 parm);
       }       
    }
    else if (sub(frame_type, RX_SPEECH_DEGRADED) == 0)
    {
       pdfi = 1;                                         
    }
   
    
    
    if (bfi != 0)
    {
        st->state = add (st->state, 1);
    }
    else if (sub (st->state, 6) == 0)

    {
        st->state = 5;                                   
    }
    else
    {
        st->state = 0;                                   
    }
    
     
    if (sub (st->state, 6) > 0)
    {
        st->state = 6;                                   
    }

    /* If this frame is the first speech frame after CNI period,     */
    /* set the BFH state machine to an appropriate state depending   */
    /* on whether there was DTX muting before start of speech or not */
    /* If there was DTX muting, the first speech frame is muted.     */
    /* If there was no DTX muting, the first speech frame is not     */
    /* muted. The BFH state machine starts from state 5, however, to */
    /* keep the audible noise resulting from a SID frame which is    */
    /* erroneously interpreted as a good speech frame as small as    */
    /* possible (the decoder output in this case is quickly muted)   */

     
    if (sub(st->dtxDecoderState->dtxGlobalState, DTX) == 0)
    {
       st->state = 5;                                    
       st->prev_bf = 0;                                  
    }
    else if ( sub(st->dtxDecoderState->dtxGlobalState, DTX_MUTE) == 0)
    {
       st->state = 5;                                    
       st->prev_bf = 1;                                  
    }
    
    /* save old LSFs for CB gain smoothing */
    Copy (st->lsfState->past_lsf_q, prev_lsf, M);
    
    /* decode LSF parameters and generate interpolated lpc coefficients
       for the 4 subframes */
    
    if (sub (mode, MR122) != 0)
    {
       D_plsf_3(st->lsfState, mode, bfi, parm, lsp_new);

                            /* function worst case */

       /* Advance synthesis parameters pointer */
       parm += 3;                  
       
       Int_lpc_1to3(st->lsp_old, lsp_new, A_t);
    }
    else
    {
       D_plsf_5 (st->lsfState, bfi, parm, lsp_mid, lsp_new);

                            /* function worst case */

       /* Advance synthesis parameters pointer */
       parm += 5;                  
       
       Int_lpc_1and3 (st->lsp_old, lsp_mid, lsp_new, A_t);
    }
       
    /* update the LSPs for the next frame */
    for (i = 0; i < M; i++)
    {
       st->lsp_old[i] = lsp_new[i];         
    }

                         /* function worst case */

   /*------------------------------------------------------------------------*
    *          Loop for every subframe in the analysis frame                 *
    *------------------------------------------------------------------------*
    * The subframe size is L_SUBFR and the loop is repeated L_FRAME/L_SUBFR  *
    *  times                                                                 *
    *     - decode the pitch delay                                           *
    *     - decode algebraic code                                            *
    *     - decode pitch and codebook gains                                  *
    *     - find the excitation and compute synthesis speech                 *
    *------------------------------------------------------------------------*/
    
    /* pointer to interpolated LPC parameters */
    Az = A_t;                                                        
    
    evenSubfr = 0;                                                  
    subfrNr = -1;                                                   
    for (i_subfr = 0; i_subfr < L_FRAME; i_subfr += L_SUBFR)
    {
       subfrNr = add(subfrNr, 1);
       evenSubfr = sub(1, evenSubfr);

       /* flag for first and 3th subframe */
       pit_flag = i_subfr;             

       
       if (sub (i_subfr, L_FRAME_BY2) == 0)
       {
           
          if (sub(mode, MR475) != 0 && sub(mode, MR515) != 0) 
          {
             pit_flag = 0;             
          }
       }
       
       /* pitch index */
       index = *parm++;                

       /*-------------------------------------------------------*
        * - decode pitch lag and find adaptive codebook vector. *
        *-------------------------------------------------------*/
       
       
       if (sub(mode, MR122) != 0)
       {
          /* flag4 indicates encoding with 4 bit resolution;     */
          /* this is needed for mode MR475, MR515, MR59 and MR67 */
          
          flag4 = 0;                                 
                     
          if ((sub (mode, MR475) == 0) ||
              (sub (mode, MR515) == 0) ||
              (sub (mode, MR59) == 0) ||
              (sub (mode, MR67) == 0) ) {
             flag4 = 1;                              
          }
          
          /*-------------------------------------------------------*
           * - get ranges for the t0_min and t0_max                *
           * - only needed in delta decoding                       *
           *-------------------------------------------------------*/

          delta_frc_low = 5;                      
          delta_frc_range = 9;                    

          
          if ( sub(mode, MR795) == 0 )
          {
             delta_frc_low = 10;                  
             delta_frc_range = 19;                
          }
             
          t0_min = sub(st->old_T0, delta_frc_low);
          
          if (sub(t0_min, PIT_MIN) < 0)
          {
             t0_min = PIT_MIN;                    
          }
          t0_max = add(t0_min, delta_frc_range);
          
          if (sub(t0_max, PIT_MAX) > 0)
          {
             t0_max = PIT_MAX;                    
             t0_min = sub(t0_max, delta_frc_range);
          }
             
          Dec_lag3 (index, t0_min, t0_max, pit_flag, st->old_T0,
                    &T0, &T0_frac, flag4);

          st->T0_lagBuff = T0;                        

          
          if (bfi != 0)
          {
             
             if (sub (st->old_T0, PIT_MAX) < 0)     
             {                                      /* Graceful pitch */
                st->old_T0 = add(st->old_T0, 1);    /* degradation    */
             }
             T0 = st->old_T0;                      
             T0_frac = 0;                          

                 
             if ( st->inBackgroundNoise != 0 && 
                  sub(st->voicedHangover, 4) > 0 &&
                  ((sub(mode, MR475) == 0 ) ||
                   (sub(mode, MR515) == 0 ) ||
                   (sub(mode, MR59) == 0) )
                  )
             {
                T0 = st->T0_lagBuff;                  
             }
          }

                       /* function worst case */

          Pred_lt_3or6 (st->exc, T0, T0_frac, L_SUBFR, 1);
       }
       else
       {
          Dec_lag6 (index, PIT_MIN_MR122,
                    PIT_MAX, pit_flag, &T0, &T0_frac);

            
          if ( bfi == 0 && (pit_flag == 0 || sub (index, 61) < 0))
          {
          }                
          else
          {
             st->T0_lagBuff = T0;                 
             T0 = st->old_T0;                      
             T0_frac = 0;                          
          }

                       /* function worst case */

          Pred_lt_3or6 (st->exc, T0, T0_frac, L_SUBFR, 0);
       }
       
                    /* function worst case */
       
       /*-------------------------------------------------------*
        * - (MR122 only: Decode pitch gain.)                    *
        * - Decode innovative codebook.                         *
        * - set pitch sharpening factor                         *
        *-------------------------------------------------------*/

          
        if (sub (mode, MR475) == 0 || sub (mode, MR515) == 0)
        {   /* MR475, MR515 */
           index = *parm++;        /* index of position */ 
           i = *parm++;            /* signs             */ 
           
                            /* function worst case */

           decode_2i40_9bits (subfrNr, i, index, code);
           
                            /* function worst case */

           pit_sharp = shl (st->sharp, 1);
        }
        else if (sub (mode, MR59) == 0)
        {   /* MR59 */
            
           index = *parm++;        /* index of position */ 
           i = *parm++;            /* signs             */ 
           
                            /* function worst case */

           decode_2i40_11bits (i, index, code);
           
                            /* function worst case */

           pit_sharp = shl (st->sharp, 1);
        }
        else if (sub (mode, MR67) == 0)
        {   /* MR67 */
            
           index = *parm++;        /* index of position */ 
           i = *parm++;            /* signs             */ 
           
                            /* function worst case */

           decode_3i40_14bits (i, index, code);
            
                            /* function worst case */

           pit_sharp = shl (st->sharp, 1);
        }
        else if (sub (mode, MR795) <= 0)
        {   /* MR74, MR795 */
             
           index = *parm++;        /* index of position */ 
           i = *parm++;            /* signs             */ 
           
                            /* function worst case */

           decode_4i40_17bits (i, index, code);
           
                            /* function worst case */

           pit_sharp = shl (st->sharp, 1);
        }
        else if (sub (mode, MR102) == 0)
        {  /* MR102 */
             

                            /* function worst case */

           dec_8i40_31bits (parm, code);
           parm += 7;                                        

                            /* function worst case */

           pit_sharp = shl (st->sharp, 1);
        }
        else
        {  /* MR122 */
             
           index = *parm++;                                
           
           if (bfi != 0)
           {
              ec_gain_pitch (st->ec_gain_p_st, st->state, &gain_pit);
           }
           else
           {
              gain_pit = d_gain_pitch (mode, index);        
           }
           ec_gain_pitch_update (st->ec_gain_p_st, bfi, st->prev_bf,
                                 &gain_pit);
           

                            /* function worst case */


           dec_10i40_35bits (parm, code);
           parm += 10;                                      

                            /* function worst case */

           /* pit_sharp = gain_pit;                   */
           /* if (pit_sharp > 1.0) pit_sharp = 1.0;   */
           
           pit_sharp = shl (gain_pit, 1);
        }
        
        /*-------------------------------------------------------*
         * - Add the pitch contribution to code[].               *
         *-------------------------------------------------------*/
        for (i = T0; i < L_SUBFR; i++)
        {
           temp = mult (code[i - T0], pit_sharp);
           code[i] = add (code[i], temp);
            
        }
        
                         /* function worst case */
        
        /*------------------------------------------------------------*
         * - Decode codebook gain (MR122) or both pitch               *
         *   gain and codebook gain (all others)                      *
         * - Update pitch sharpening "sharp" with quantized gain_pit  *
         *------------------------------------------------------------*/

        if ( sub (mode, MR475) == 0)
        {
           /* read and decode pitch and code gain */
                 
           if (evenSubfr != 0) 
           {
              index_mr475 = *parm++;         /* index of gain(s) */
           }

           
           if (bfi == 0)
           {
              Dec_gain(st->pred_state, mode, index_mr475, code,
                       evenSubfr, &gain_pit, &gain_code); 
           }
           else
           {
              ec_gain_pitch (st->ec_gain_p_st, st->state, &gain_pit);
              ec_gain_code (st->ec_gain_c_st, st->pred_state, st->state,
                            &gain_code);
           }
           ec_gain_pitch_update (st->ec_gain_p_st, bfi, st->prev_bf,
                                 &gain_pit);
           ec_gain_code_update (st->ec_gain_c_st, bfi, st->prev_bf,
                                &gain_code);
           
                            /* function worst case */

           pit_sharp = gain_pit;                                
           
           if (sub (pit_sharp, SHARPMAX) > 0) 
           {
               pit_sharp = SHARPMAX;                            
           }

        }
        else if (  (sub (mode, MR74) <= 0) || 
                 (sub (mode, MR102) == 0))
        {
            /* read and decode pitch and code gain */
            index = *parm++;                 /* index of gain(s) */
           
            
            if (bfi == 0)
            {
               Dec_gain(st->pred_state, mode, index, code,
                        evenSubfr, &gain_pit, &gain_code);
            }
            else
            {
               ec_gain_pitch (st->ec_gain_p_st, st->state, &gain_pit);
               ec_gain_code (st->ec_gain_c_st, st->pred_state, st->state,
                             &gain_code);
            }
            ec_gain_pitch_update (st->ec_gain_p_st, bfi, st->prev_bf,
                                  &gain_pit);
            ec_gain_code_update (st->ec_gain_c_st, bfi, st->prev_bf,
                                 &gain_code);

                             /* function worst case */
            
            pit_sharp = gain_pit;                               
            
            if (sub (pit_sharp, SHARPMAX) > 0) 
            {
               pit_sharp = SHARPMAX;                           
            }

            if (sub (mode, MR102) == 0)
            {
               if (sub (st->old_T0, add(L_SUBFR, 5)) > 0)
               {
                  pit_sharp = shr(pit_sharp, 2);
               }
            }
        }
        else
        {
           /* read and decode pitch gain */
           index = *parm++;                 /* index of gain(s) */
           
            
           if (sub (mode, MR795) == 0)
           {
              /* decode pitch gain */
              
              if (bfi != 0)
              {
                 ec_gain_pitch (st->ec_gain_p_st, st->state, &gain_pit);
              }
              else
              {
                 gain_pit = d_gain_pitch (mode, index);       
              }
              ec_gain_pitch_update (st->ec_gain_p_st, bfi, st->prev_bf,
                                    &gain_pit);
              
              /* read and decode code gain */
              index = *parm++;                                
              
              if (bfi == 0)
              {
                 d_gain_code (st->pred_state, mode, index, code, &gain_code);
              }
              else
              {
                 ec_gain_code (st->ec_gain_c_st, st->pred_state, st->state,
                               &gain_code);
              }
              ec_gain_code_update (st->ec_gain_c_st, bfi, st->prev_bf,
                                   &gain_code);
              
                               /* function worst case */
              
              pit_sharp = gain_pit;                               
              
              if (sub (pit_sharp, SHARPMAX) > 0)
              {
                 pit_sharp = SHARPMAX;                           
              }
           }
           else 
           { /* MR122 */
              
              if (bfi == 0)
              {
                 d_gain_code (st->pred_state, mode, index, code, &gain_code);
              }
              else
              {
                 ec_gain_code (st->ec_gain_c_st, st->pred_state, st->state,
                               &gain_code);
              }
              ec_gain_code_update (st->ec_gain_c_st, bfi, st->prev_bf,
                                   &gain_code);
                            
                               /* function worst case */
                
              pit_sharp = gain_pit;                                
           }
        }
        
        /* store pitch sharpening for next subframe          */
        /* (for modes which use the previous pitch gain for 
           pitch sharpening in the search phase)             */
        /* do not update sharpening in even subframes for MR475 */
         
        if (sub(mode, MR475) != 0 || evenSubfr == 0)
        {
            st->sharp = gain_pit;                                    
            
            if (sub (st->sharp, SHARPMAX) > 0)
            {
                st->sharp = SHARPMAX;                                
            }
        }

        pit_sharp = shl (pit_sharp, 1);
        
        if (sub (pit_sharp, 16384) > 0)
        {
           for (i = 0; i < L_SUBFR; i++)
            {
               temp = mult (st->exc[i], pit_sharp);
               L_temp = L_mult (temp, gain_pit);
               
               if (sub(mode, MR122)==0)
               {
                  L_temp = L_shr (L_temp, 1);
               }
               excp[i] = round (L_temp);                         
            }
        }
        
        /*-------------------------------------------------------*
         * - Store list of LTP gains needed in the source        *
         *   characteristic detector (SCD)                       *
         *-------------------------------------------------------*/
        
        if ( bfi == 0 )
        {
           for (i = 0; i < 8; i++)
           {
              st->ltpGainHistory[i] = st->ltpGainHistory[i+1];     
           }
           st->ltpGainHistory[8] = gain_pit;                       
        }

        /*-------------------------------------------------------*
         * - Limit gain_pit if in background noise and BFI       *
         *   for MR475, MR515, MR59                              *
         *-------------------------------------------------------*/

             
        if ( (st->prev_bf != 0 || bfi != 0) && st->inBackgroundNoise != 0 &&
             ((sub(mode, MR475) == 0) ||
              (sub(mode, MR515) == 0) ||
              (sub(mode, MR59) == 0))
             )
        {
           
           if ( sub (gain_pit, 12288) > 0)    /* if (gain_pit > 0.75) in Q14*/
              gain_pit = add( shr( sub(gain_pit, 12288), 1 ), 12288 );
              /* gain_pit = (gain_pit-0.75)/2.0 + 0.75; */

           
           if ( sub (gain_pit, 14745) > 0)    /* if (gain_pit > 0.90) in Q14*/
           {
              gain_pit = 14745;                                 
           }
        }

        /*-------------------------------------------------------*
         *  Calculate CB mixed gain                              *
         *-------------------------------------------------------*/
        Int_lsf(prev_lsf, st->lsfState->past_lsf_q, i_subfr, lsf_i); 
        gain_code_mix = Cb_gain_average(
            st->Cb_gain_averState, mode, gain_code, 
            lsf_i, st->lsp_avg_st->lsp_meanSave, bfi, 
            st->prev_bf, pdfi, st->prev_pdf,  
            st->inBackgroundNoise, st->voicedHangover);         
        
        /* make sure that MR74, MR795, MR122 have original code_gain*/
        
        if ((sub(mode, MR67) > 0) && (sub(mode, MR102) != 0) ) 
           /* MR74, MR795, MR122 */
        {
           gain_code_mix = gain_code;                 
        }
        
        /*-------------------------------------------------------*
         * - Find the total excitation.                          *
         * - Find synthesis speech corresponding to st->exc[].   *
         *-------------------------------------------------------*/
        
        if (sub(mode, MR102) <= 0) /* MR475, MR515, MR59, MR67, MR74, MR795, MR102*/
        {
           pitch_fac = gain_pit;                                
           tmp_shift = 1;                                       
        }
        else       /* MR122 */
        {
           pitch_fac = shr (gain_pit, 1);                       
           tmp_shift = 2;                                       
        }

        /* copy unscaled LTP excitation to exc_enhanced (used in phase
         * dispersion below) and compute total excitation for LTP feedback
         */
        for (i = 0; i < L_SUBFR; i++)
        {
           exc_enhanced[i] = st->exc[i];                        

           /* st->exc[i] = gain_pit*st->exc[i] + gain_code*code[i]; */
           L_temp = L_mult (st->exc[i], pitch_fac);
                                                      /* 12.2: Q0 * Q13 */
                                                      /*  7.4: Q0 * Q14 */
           L_temp = L_mac (L_temp, code[i], gain_code);
                                                      /* 12.2: Q12 * Q1 */
                                                      /*  7.4: Q13 * Q1 */
           L_temp = L_shl (L_temp, tmp_shift);                   /* Q16 */
           st->exc[i] = round (L_temp);                          
        }
        
        /*-------------------------------------------------------*
         * - Adaptive phase dispersion                           *
         *-------------------------------------------------------*/
        ph_disp_release(st->ph_disp_st); /* free phase dispersion adaption */

             
        if ( ((sub(mode, MR475) == 0) ||
              (sub(mode, MR515) == 0) ||
              (sub(mode, MR59) == 0))   &&
             sub(st->voicedHangover, 3) > 0 &&
             st->inBackgroundNoise != 0 &&
             bfi != 0 )
        {
           ph_disp_lock(st->ph_disp_st); /* Always Use full Phase Disp. */
        }                                /* if error in bg noise       */

        /* apply phase dispersion to innovation (if enabled) and
           compute total excitation for synthesis part           */
        ph_disp(st->ph_disp_st, mode,
                exc_enhanced, gain_code_mix, gain_pit, code,
                pitch_fac, tmp_shift);       
        
        /*-------------------------------------------------------*
         * - The Excitation control module are active during BFI.*
         * - Conceal drops in signal energy if in bg noise.      *
         *-------------------------------------------------------*/

        L_temp = 0;                                   
        for (i = 0; i < L_SUBFR; i++)
        {
            L_temp = L_mac (L_temp, exc_enhanced[i], exc_enhanced[i] );
        }

        L_temp = L_shr (L_temp, 1);     /* excEnergy = sqrt(L_temp) in Q0 */
        L_temp = sqrt_l_exp(L_temp, &temp);  /* function result */
        L_temp = L_shr(L_temp, add( shr(temp, 1), 15));
        L_temp = L_shr(L_temp, 2);       /* To cope with 16-bit and  */
        excEnergy = extract_l(L_temp);   /* scaling in ex_ctrl()     */

             
            
        if ( ((sub (mode, MR475) == 0) ||
              (sub (mode, MR515) == 0) ||
              (sub (mode, MR59) == 0))  &&
             sub(st->voicedHangover, 5) > 0 &&
             st->inBackgroundNoise != 0 &&
             sub(st->state, 4) < 0 &&
             ( (pdfi != 0 && st->prev_pdf != 0) ||
                bfi != 0 ||
                st->prev_bf != 0) )
        {
           carefulFlag = 0;                          
                       
           if ( pdfi != 0 && bfi == 0 )       
           {
              carefulFlag = 1;                       
           }

           Ex_ctrl(exc_enhanced,     
                   excEnergy,
                   st->excEnergyHist,
                   st->voicedHangover,
                   st->prev_bf,
                   carefulFlag);
        }

           
        if ( st->inBackgroundNoise != 0 &&
             ( bfi != 0 || st->prev_bf != 0 ) &&
             sub(st->state, 4) < 0 )
        { 
           ; /* do nothing! */
        }
        else
        {
           /* Update energy history for all modes */
           for (i = 0; i < 8; i++)
           {
              st->excEnergyHist[i] = st->excEnergyHist[i+1]; 
           }
           st->excEnergyHist[8] = excEnergy;   
        }
        /*-------------------------------------------------------*
         * Excitation control module end.                        *
         *-------------------------------------------------------*/
        
                         /* function worst case */

         
        if (sub (pit_sharp, 16384) > 0)
        {
           for (i = 0; i < L_SUBFR; i++)
           {
              excp[i] = add (excp[i], exc_enhanced[i]);              
               
           }
           agc2 (exc_enhanced, excp, L_SUBFR);
           cgOverflow = 0;                 
           Syn_filt (Az, excp, &synth[i_subfr], L_SUBFR,
                     st->mem_syn, 0);
        }
        else
        {
           cgOverflow = 0;                 
           Syn_filt (Az, exc_enhanced, &synth[i_subfr], L_SUBFR,
                     st->mem_syn, 0);
        }

        
        if (cgOverflow != 0)    /* Test for cgOverflow */
        {
           for (i = 0; i < PIT_MAX + L_INTERPOL + L_SUBFR; i++)
           {
              st->old_exc[i] = shr(st->old_exc[i], 2);       
           }
           for (i = 0; i < L_SUBFR; i++)
           {
              exc_enhanced[i] = shr(exc_enhanced[i], 2);     
           }
           Syn_filt(Az, exc_enhanced, &synth[i_subfr], L_SUBFR, st->mem_syn, 1);
        }
        else
        {
           Copy(&synth[i_subfr+L_SUBFR-M], st->mem_syn, M);
        }
        
        /*--------------------------------------------------*
         * Update signal for next frame.                    *
         * -> shift to the left by L_SUBFR  st->exc[]       *
         *--------------------------------------------------*/
        
        Copy (&st->old_exc[L_SUBFR], &st->old_exc[0], PIT_MAX + L_INTERPOL);

                         /* function worst case */

        /* interpolated LPC parameters for next subframe */
        Az += MP1;                      
        
        /* store T0 for next subframe */ 
        st->old_T0 = T0;                
    }
    
    /*-------------------------------------------------------*
     * Call the Source Characteristic Detector which updates *
     * st->inBackgroundNoise and st->voicedHangover.         *
     *-------------------------------------------------------*/

                             /* function result */
    st->inBackgroundNoise = Bgn_scd(st->background_state,
                                    &(st->ltpGainHistory[0]),
                                    &(synth[0]),
                                    &(st->voicedHangover) );

    dtx_dec_activity_update(st->dtxDecoderState, 
                            st->lsfState->past_lsf_q, 
                            synth);
    
                         /* function worst case */

    /* store bfi for next subframe */
    st->prev_bf = bfi;                   
    st->prev_pdf = pdfi;                 
    
    /*--------------------------------------------------*
     * Calculate the LSF averages on the eight          *
     * previous frames                                  *
     *--------------------------------------------------*/
    
    lsp_avg(st->lsp_avg_st, st->lsfState->past_lsf_q);
                     /* function worst case */

the_end:
    st->dtxDecoderState->dtxGlobalState = newDTXState;  
    
    return 0;
}
