#ifndef main_pd_h
#define main_pd_h "$Id $"

#include "typedef.h"
#include "cnst.h"
#include "mode.h"
#include "frame.h"
#include "speech_decode_frame.h"

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/
typedef struct {
	Speech_Decode_FrameState     m_Speech_Decode_FrameState;  
	Decoder_amrState             m_Decoder_amrstate;
    Post_FilterState             m_Post_FilterState;
    Post_ProcessState            m_Post_ProcessState;
    Bgn_scdState                 m_Bgn_scdState;
	Cb_gain_averageState         m_Cb_gain_averageState;
    lsp_avgState				 m_lsp_avgState;
	D_plsfState					 m_D_plsfState;
	ec_gain_pitchState			 m_ec_gain_pitchState;
    ec_gain_codeState            m_ec_gain_codeState;
	gc_predState				 m_gc_predState;
	ph_dispState				 m_ph_dispState;
	dtx_decState				 m_dtx_decState;
	agcState					 m_agcState;
	preemphasisState             m_preemphasisState;
	Word16						 synth[L_FRAME];
	Word16						 serial[SERIAL_FRAMESIZE];
	Word16						 reset_flag_old;
}AMR_Decoder_Channel;

void Decoder_Channel_Malloc(AMR_Decoder_Channel *Decoder_One_Channel_Mem);

enum RXFrameType tx_to_rx (enum TXFrameType tx_type);

void Decoder_Channel_Init(AMR_Decoder_Channel *Decoder_One_Channel_Mem);

void Decoder_Channel_Reset(AMR_Decoder_Channel *Decoder_One_Channel_Mem);

void Decoder_Channel_Process(AMR_Decoder_Channel *Decoder_One_Channel_Mem, 
							 Word16 serial[], 
							 Word16 synth[]);
#endif
