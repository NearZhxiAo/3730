#ifndef amr_global_var_h
#define amr_global_var_h "$ID $"

#include "typedef.h"
#include "main_pd.h"
#include "main_pc.h"

extern Flag cgOverflow;
extern Flag cgCarry;

extern Word16 AmrCfgMode;
extern Word16 AmrCfgDtx;
extern Word16 AmrCfgRfci;
extern Word16 AmrCfgDtxRfci;
extern Word16 m_AmrDecoderState;

extern Word16 AmrTxFrameNo;
extern Word16 AmrRxFrameNo;

extern AMR_Decoder_Channel Decoder_Channel_Mem;

extern AMR_Encoder_Channel Encoder_Channel_Mem;

#endif