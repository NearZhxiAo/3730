
/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*
*      File             : decoder.c
*      Purpose          : Speech decoder main program.
*
********************************************************************************
*
*         Usage : decoder  bitstream_file  synth_file
*
*
*         Format for bitstream_file:
*             1 word (2-byte) for the frame type
*               (see frame.h for possible values)
*               Normally, the TX frame type is expected.
*               RX frame type can be forced with "-rxframetype"
*           244 words (2-byte) containing 244 bits.
*               Bit 0 = 0x0000 and Bit 1 = 0x0001
*             1 word (2-byte) for the mode indication
*               (see mode.h for possible values)
*             4 words for future use, currently unused
*
*         Format for synth_file:
*           Synthesis is written to a binary file of 16 bits data.
*
********************************************************************************
*/
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "typedef.h"
#include "cnst.h"
#include "mode.h"
#include "frame.h"
#include "speech_decode_frame.h"
#include "main_pd.h"
#include "amr_global_var.h"

/*
********************************************************************************
*                             MAIN PROGRAM 
********************************************************************************
*/

int decoder_entry (Word16 serial[],Word16 speech[])
{
	AMR_Decoder_Channel *p_Decoder_Channel_Mem;

	/* get channel memory pointer */
	p_Decoder_Channel_Mem = &Decoder_Channel_Mem;
					
	/* decoding */
	Decoder_Channel_Process(p_Decoder_Channel_Mem,serial,speech);
			
	return 0;
}
