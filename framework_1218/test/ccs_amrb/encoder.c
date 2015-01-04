/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*
*      File             : coder.c
*      Purpose          : Speech encoder main program.
*
*****************************************************************************
*
*    Usage : coder speech_file  bitstream_file
*
*    Format for speech_file:
*      Speech is read from a binary file of 16 bits data.
*
*    Format for bitstream_file:
*        1 word (2-byte) for the TX frame type
*          (see frame.h for possible values)
*      244 words (2-byte) containing 244 bits.
*          Bit 0 = 0x0000 and Bit 1 = 0x0001
*        1 word (2-byte) for the mode indication
*          (see mode.h for possible values)
*        4 words for future use, currently written as zero
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "typedef.h"
#include "cnst.h"
#include "mode.h"
#include "frame.h"
#include "pre_process.h"
#include "sid_sync.h"
#include "encoder_homing_frame_test.h"
#include "main_pc.h"

#include "amr_global_var.h"

int encoder_entry (Word16 new_speech[],Word16 serial[])
{
	AMR_Encoder_Channel *p_Encoder_Channel_Mem;

	/* get channel memory pointer */
	p_Encoder_Channel_Mem = &Encoder_Channel_Mem;

	/* encoding */
	Encoder_Channel_Proccess(new_speech,p_Encoder_Channel_Mem,serial);   
		
	return 0;
}
