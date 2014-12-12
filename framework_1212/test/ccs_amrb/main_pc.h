
#ifndef main_pc_h
#define main_pc_h "$Id $"

#include "cod_amr.h"
#include "sid_sync.h"
#include "cnst.h"
#include "speech_encode_frame.h"

typedef struct {
	Speech_Encode_FrameState	m_Speech_Encoder_FrameState;
	Pre_ProcessState			m_Pre_ProcessState;
	cod_amrState				m_cod_amrState;
	lpcState					m_lpcState;
	LevinsonState				m_LevinsonState;
	lspState					m_lspState;
	Q_plsfState					m_Q_plsfState;
	clLtpState					m_clLtpState;
	Pitch_frState				m_Pitch_frState;
	gainQuantState				m_gainQuantState;
	gc_predState				m_gc_predState;
	gc_predState				m_gc_predUnqState;
	GainAdaptState				m_GainAdaptState;
	pitchOLWghtState			m_pitchOLWghtState;
	tonStabState				m_tonStabState;
	vadState1					m_vadState;
	dtx_encState				m_dtx_encState;
	sid_syncState				m_sid_syncState;

	Word16						new_speech[L_FRAME];
	Word16						serial[SERIAL_FRAMESIZE];

	Word16						dtx;                     
	enum Mode					mode;
  
} AMR_Encoder_Channel;

void Encoder_Channel_Malloc(AMR_Encoder_Channel *p_Encoder_Channel_Mem);

void Encoder_Channel_Init(AMR_Encoder_Channel *p_Encoder_Channel_Mem);

void Encoder_Channel_Reset(AMR_Encoder_Channel *p_Encoder_Channel_Mem);

void Encoder_Channel_Proccess(Word16 new_speech[],
							  AMR_Encoder_Channel *p_Encoder_Channel_Mem,
							  Word16 serial[]);
#endif
