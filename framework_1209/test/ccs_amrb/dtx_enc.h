/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef dtx_enc_h
#define dtx_enc_h "$Id $"

#include "dtx_def.h"
#include "q_plsf.h"
#include "gc_pred.h"

/*
**************************************************************************
*
*  Function    : dtx_enc
*                
**************************************************************************
*/
int dtx_enc(
			dtx_encState *st,        /* i/o : State struct                    */
            Word16 computeSidFlag,   /* i   : compute SID                     */
            Q_plsfState *qSt,        /* i/o : Qunatizer state struct          */
            gc_predState* predState, /* i/o : State struct                    */
			Word16 **anap            /* o   : analysis parameters             */
			);
#endif

