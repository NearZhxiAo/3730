/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef tx_dtx_handler_h
#define tx_dtx_handler_h "$Id $"

#include "dtx_def.h"
#include "mode.h"
#include "typedef.h"

/*
**************************************************************************
*
*  Function    : tx_dtx_handler
*  Purpose     : adds extra speech hangover to analyze speech on the decoding side. 
*                
**************************************************************************
*/
Word16 tx_dtx_handler(dtx_encState *st,      /* i/o : State struct           */
                      Word16 vad_flag,       /* i   : vad decision           */
                      enum Mode *usedMode    /* i/o : mode changed or not    */
                      );
#endif

