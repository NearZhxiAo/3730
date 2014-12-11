/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Speech_Encode_Frame_init_h
#define Speech_Encode_Frame_init_h "$Id $"

#include "typedef.h"
#include "speech_encode_frame.h"
 
/*************************************************************************
*
*  Function:   Speech_Encode_Frame_init
*  Purpose:    Allocates memory for filter structure and initializes
*              state memory
*
**************************************************************************
*/
int Speech_Encode_Frame_init (
							  Speech_Encode_FrameState *state,
                              Flag dtx
							  );

#endif
