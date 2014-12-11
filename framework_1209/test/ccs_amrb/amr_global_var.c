#include "typedef.h"
#include "main_pd.h"
#include "main_pc.h"

Flag cgOverflow;
Flag cgCarry;

Word16 AmrCfgMode;
Word16 AmrCfgDtx;
Word16 AmrCfgRfci;
Word16 AmrCfgDtxRfci;
Word16 m_AmrDecoderState;

Word16 AmrTxFrameNo;
Word16 AmrRxFrameNo;

AMR_Decoder_Channel Decoder_Channel_Mem;

AMR_Encoder_Channel Encoder_Channel_Mem;

