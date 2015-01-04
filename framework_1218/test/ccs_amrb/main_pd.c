#include "main_pd.h"
#include "speech_decode_frame_init.h"
#include "speech_decode_frame_reset.h"
#include "decoder_homing_frame_test_first.h"
#include "decoder_homing_frame_test.h"
/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
enum RXFrameType tx_to_rx (enum TXFrameType tx_type)
{
    if (tx_type == TX_SPEECH_GOOD)
       return RX_SPEECH_GOOD;
    else if (tx_type == TX_SPEECH_DEGRADED)
       return RX_SPEECH_DEGRADED;
    else if (tx_type == TX_SPEECH_BAD)
       return RX_SPEECH_BAD;
    else if (tx_type == TX_SID_FIRST)
       return RX_SID_FIRST;
    else if (tx_type == TX_SID_UPDATE)
       return RX_SID_UPDATE;
    else if (tx_type == TX_SID_BAD)
       return RX_SID_BAD;
    else if (tx_type == TX_ONSET)
       return RX_ONSET;
    else if (tx_type == TX_NO_DATA)
       return RX_NO_DATA;
    else 
       return RX_N_FRAMETYPES;
}

void Decoder_Channel_Malloc(AMR_Decoder_Channel *Decoder_One_Channel_Mem)
{
	Speech_Decode_FrameState *pFrameState;
	Decoder_amrState *pDecoder_amrState;

    pFrameState=&Decoder_One_Channel_Mem->m_Speech_Decode_FrameState;

	pFrameState->decoder_amrState=&Decoder_One_Channel_Mem->m_Decoder_amrstate;
    
	pFrameState->post_state=&Decoder_One_Channel_Mem->m_Post_FilterState;
    pFrameState->post_state->agc_state=&Decoder_One_Channel_Mem->m_agcState;
	pFrameState->post_state->preemph_state=&Decoder_One_Channel_Mem->m_preemphasisState;
	
    pFrameState->postHP_state=&Decoder_One_Channel_Mem->m_Post_ProcessState;

	pDecoder_amrState=pFrameState->decoder_amrState;

    pDecoder_amrState->background_state=&Decoder_One_Channel_Mem->m_Bgn_scdState;
    pDecoder_amrState->Cb_gain_averState=&Decoder_One_Channel_Mem->m_Cb_gain_averageState;
    pDecoder_amrState->dtxDecoderState=&Decoder_One_Channel_Mem->m_dtx_decState;
	pDecoder_amrState->ec_gain_c_st=&Decoder_One_Channel_Mem->m_ec_gain_codeState;
	pDecoder_amrState->ec_gain_p_st=&Decoder_One_Channel_Mem->m_ec_gain_pitchState;
	pDecoder_amrState->lsfState=&Decoder_One_Channel_Mem->m_D_plsfState;
	pDecoder_amrState->lsp_avg_st=&Decoder_One_Channel_Mem->m_lsp_avgState;
	pDecoder_amrState->ph_disp_st=&Decoder_One_Channel_Mem->m_ph_dispState;
	pDecoder_amrState->pred_state=&Decoder_One_Channel_Mem->m_gc_predState;

	return;   
}
void Decoder_Channel_Init(AMR_Decoder_Channel *Decoder_One_Channel_Mem)
{

	Speech_Decode_FrameState *speech_decoder_state;
	
	speech_decoder_state= &Decoder_One_Channel_Mem->m_Speech_Decode_FrameState;
	
	Speech_Decode_Frame_init(speech_decoder_state);
}

void Decoder_Channel_Reset(AMR_Decoder_Channel *Decoder_One_Channel_Mem)
{

	Speech_Decode_FrameState *speech_decoder_state;
	
	speech_decoder_state= &Decoder_One_Channel_Mem->m_Speech_Decode_FrameState;
	
	Speech_Decode_Frame_reset(speech_decoder_state);
}

void Decoder_Channel_Process(AMR_Decoder_Channel *Decoder_One_Channel_Mem,
							 Word16 serial[],
							 Word16 synth[])
{

  Speech_Decode_FrameState *speech_decoder_state;
  enum Mode mode;
  enum RXFrameType rx_type;
  enum TXFrameType tx_type;
  Word16 reset_flag=0;
  int i;
  speech_decoder_state= &Decoder_One_Channel_Mem->m_Speech_Decode_FrameState;
  /* get frame type and mode information from frame */
  tx_type = (enum TXFrameType)serial[0];
  rx_type = tx_to_rx (tx_type);
  
  /* frame type error */
  if(rx_type == RX_N_FRAMETYPES)
  {
	/* return immediately to keep the program running */
	/* output same spech as the last frame            */
	/* no update is needed                            */
	return;
  }  	
 
  mode = (enum Mode) serial[1+MAX_SERIAL_SIZE];
  if (rx_type == RX_NO_DATA) 
  {
     mode = speech_decoder_state->prev_mode;
  }
  else 
  {
     speech_decoder_state->prev_mode = mode;
  }


  /* if homed: check if this frame is another homing frame */
  if (Decoder_One_Channel_Mem->reset_flag_old == 1)
  {
	  /* only check until end of first subframe */
	  reset_flag = decoder_homing_frame_test_first(&serial[1], mode);
  }
  /* produce encoder homing frame if homed & input=decoder homing frame */
  if ((reset_flag != 0) && (Decoder_One_Channel_Mem->reset_flag_old != 0))
  {
	  for (i = 0; i < L_FRAME; i++)
	  {
		  synth[i] = EHF_MASK;
	  }
  }
  else
  {     
	  /* decode frame */
	  Speech_Decode_Frame(speech_decoder_state, mode, &serial[1],
		                  rx_type, synth);
  }
  
  /* if not homed: check whether current frame is a homing frame */
  if (Decoder_One_Channel_Mem->reset_flag_old == 0)
  {
	  /* check whole frame */
	  reset_flag = decoder_homing_frame_test(&serial[1], mode);
  }
		  
  /* reset decoder if current frame is a homing frame */
  if (reset_flag != 0)
  {
	  Decoder_Channel_Reset(Decoder_One_Channel_Mem);
  }
  Decoder_One_Channel_Mem->reset_flag_old = reset_flag;
  return;
}
