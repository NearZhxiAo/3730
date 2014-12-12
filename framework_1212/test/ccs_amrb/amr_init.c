#include "typedef.h"
#include "amr_global_var.h"

int amr_init ()
{
	AMR_Encoder_Channel *p_Encoder_Channel_Mem;
	AMR_Decoder_Channel *p_Decoder_Channel_Mem;

	m_AmrDecoderState = 0;

	AmrTxFrameNo = 0;
	AmrRxFrameNo = 0;

	cgOverflow = 0;
	cgCarry = 0;

	/*-----------------------------------------------------------------------*
	* Initialisation of the coder.                                          *
	*-----------------------------------------------------------------------*/
	p_Encoder_Channel_Mem = &Encoder_Channel_Mem;
	Encoder_Channel_Malloc(p_Encoder_Channel_Mem);
	p_Encoder_Channel_Mem->mode = (enum Mode)AmrCfgMode;
	p_Encoder_Channel_Mem->dtx = AmrCfgDtx;
	Encoder_Channel_Init(p_Encoder_Channel_Mem);

	/*-----------------------------------------------------------------------*
	 * Initialization of decoder                                             *
	 *-----------------------------------------------------------------------*/
	p_Decoder_Channel_Mem=&Decoder_Channel_Mem;
	Decoder_Channel_Malloc(p_Decoder_Channel_Mem);
	p_Decoder_Channel_Mem->reset_flag_old=1;  
	Decoder_Channel_Init(p_Decoder_Channel_Mem);
	
	return 0;
}
