#include "typedef.h"
#include "mode.h"
#include "encoder_homing_frame_test.h"
#include "main_pc.h"
#include "speech_encode_frame_init.h"
#include "sid_sync_init.h"
#include "speech_encode_frame_reset.h"
#include "sid_sync_reset.h"


void Encoder_Channel_Malloc(AMR_Encoder_Channel *p_Encoder_Channel_Mem)
{
	Speech_Encode_FrameState *speech_encoder_state;
	cod_amrState			 *coder_amrState;
	
	speech_encoder_state = &p_Encoder_Channel_Mem->m_Speech_Encoder_FrameState;
    
	speech_encoder_state->cod_amr_state = &p_Encoder_Channel_Mem->m_cod_amrState;
	
	speech_encoder_state->pre_state = &p_Encoder_Channel_Mem->m_Pre_ProcessState;

	coder_amrState = speech_encoder_state->cod_amr_state;

	coder_amrState->lpcSt = &p_Encoder_Channel_Mem->m_lpcState;
	coder_amrState->lpcSt->levinsonSt = &p_Encoder_Channel_Mem->m_LevinsonState;

	coder_amrState->lspSt = &p_Encoder_Channel_Mem->m_lspState;

	coder_amrState->lspSt->qSt = &p_Encoder_Channel_Mem->m_Q_plsfState;

	coder_amrState->clLtpSt = &p_Encoder_Channel_Mem->m_clLtpState;

	coder_amrState->clLtpSt->pitchSt = &p_Encoder_Channel_Mem->m_Pitch_frState;
	coder_amrState->gainQuantSt = &p_Encoder_Channel_Mem->m_gainQuantState;

	coder_amrState->gainQuantSt->adaptSt = &p_Encoder_Channel_Mem->m_GainAdaptState;
	coder_amrState->gainQuantSt->gc_predSt = &p_Encoder_Channel_Mem->m_gc_predState;
	coder_amrState->gainQuantSt->gc_predUnqSt = &p_Encoder_Channel_Mem->m_gc_predUnqState;

	coder_amrState->pitchOLWghtSt = &p_Encoder_Channel_Mem->m_pitchOLWghtState;

	coder_amrState->tonStabSt = &p_Encoder_Channel_Mem->m_tonStabState;

	coder_amrState->vadSt = &p_Encoder_Channel_Mem->m_vadState;

	coder_amrState->dtx_encSt = &p_Encoder_Channel_Mem->m_dtx_encState;

	return;
}
void Encoder_Channel_Init(AMR_Encoder_Channel *p_Encoder_Channel_Mem)
{
	Speech_Encode_FrameState *speech_encoder_state;
	sid_syncState *sid_state ;
	Word16 dtx;

	speech_encoder_state = &p_Encoder_Channel_Mem->m_Speech_Encoder_FrameState;
	sid_state = &p_Encoder_Channel_Mem->m_sid_syncState;

	dtx = p_Encoder_Channel_Mem->dtx;

	Speech_Encode_Frame_init(speech_encoder_state, dtx);
	sid_sync_init (sid_state);
	return;
}

void Encoder_Channel_Reset(AMR_Encoder_Channel *p_Encoder_Channel_Mem)
{
	Speech_Encode_FrameState *speech_encoder_state;
	sid_syncState *sid_state ;

	speech_encoder_state = &p_Encoder_Channel_Mem->m_Speech_Encoder_FrameState;
	sid_state = &p_Encoder_Channel_Mem->m_sid_syncState;

	Speech_Encode_Frame_reset(speech_encoder_state);
	sid_sync_reset(sid_state);

	return;
}

void Encoder_Channel_Proccess(Word16 new_speech[],
							  AMR_Encoder_Channel *p_Encoder_Channel_Mem,
							  Word16 serial[])
{
	Speech_Encode_FrameState *speech_encoder_state;
	sid_syncState *sid_state ;

    enum Mode mode;
	/* changed eedodr */
	Word16 reset_flag;
	Word16 i;
	enum Mode used_mode;
	enum TXFrameType tx_type;

	mode = p_Encoder_Channel_Mem->mode;
	
 	speech_encoder_state = &p_Encoder_Channel_Mem->m_Speech_Encoder_FrameState;
	sid_state = &p_Encoder_Channel_Mem->m_sid_syncState;

	/* zero flags and parameter bits */
	for (i = 0; i < SERIAL_FRAMESIZE; i++)
	{
		serial[i] = 0;
	}
	
	/* check for homing frame */
	reset_flag = encoder_homing_frame_test(new_speech);
	
	/* encode speech */
	Speech_Encode_Frame(speech_encoder_state, mode, new_speech, &serial[1], &used_mode); 
	
	/* include frame type and mode information in serial bitstream */
	sid_sync (sid_state, used_mode, &tx_type);
	serial[0] = tx_type;
    if (tx_type != TX_NO_DATA) 
    {
       serial[1+MAX_SERIAL_SIZE] = mode;
    }
    else 
    {
       serial[1+MAX_SERIAL_SIZE] = -1;
    }

	
	/* perform homing if homing frame was detected at encoder input */
	if (reset_flag != 0)
	{
		Encoder_Channel_Reset(p_Encoder_Channel_Mem);
	}

	return;
}
