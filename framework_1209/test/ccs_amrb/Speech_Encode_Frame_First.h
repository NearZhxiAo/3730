/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Speech_Encode_Frame_First_h
#define Speech_Encode_Frame_First_h "$Id $"

#include "..\\common\\typedef.h"
#include "speech_encode_frame.h"

int Speech_Encode_Frame_First (
    Speech_Encode_FrameState *st,  /* i/o : post filter states       */
    Word16 *new_speech             /* i   : speech input             */
	);

#endif

