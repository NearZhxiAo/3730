/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Speech_Encode_Frame_h
#define Speech_Encode_Frame_h "$Id $"
 
#include "typedef.h"
#include "pre_process.h"
#include "cod_amr.h"

typedef struct{
    Pre_ProcessState *pre_state;
    cod_amrState   *cod_amr_state;
    Flag dtx;
} Speech_Encode_FrameState;
 
int Speech_Encode_Frame (
    Speech_Encode_FrameState *st, /* i/o : encoder states         */
    enum Mode mode,               /* i   : speech coder mode      */
    Word16 *new_speech,           /* i   : input speech           */
    Word16 *serial,               /* o   : serial bit stream      */
    enum Mode *usedMode           /* o   : used speech coder mode */
);
/*    return 0 on success
 */
 
#endif
