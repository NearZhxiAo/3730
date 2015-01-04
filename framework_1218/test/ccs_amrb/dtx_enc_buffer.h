/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef dtx_enc_buffer_h
#define dtx_enc_buffer_h "$Id $"

#include "typedef.h"
#include "dtx_def.h"

/*
**************************************************************************
*
*  Function    : dtx_enc_buffer
*  Purpose     : handles the DTX buffer
*                
**************************************************************************
*/
int dtx_enc_buffer(dtx_encState *st,   /* i/o : State struct                    */
	       Word16 lsp_new[],   /* i   : LSP vector                      */
	       Word16 speech[]     /* i   : speech samples                  */
	       );
#endif
