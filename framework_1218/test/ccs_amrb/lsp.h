/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef lsp_h
#define lsp_h "$Id $"
 
#include "typedef.h"
#include "q_plsf.h"
#include "mode.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/*
********************************************************************************
*                         DEFINITION OF DATA TYPES
********************************************************************************
*/
typedef struct {

   /* Past LSPs */
   Word16 lsp_old[M];
   Word16 lsp_old_q[M];

   /* Quantization state */
   Q_plsfState *qSt;

} lspState;

/*
**************************************************************************
*
*  Function    : lsp
*  Purpose     : Conversion from LP coefficients to LSPs.
*                Quantization of LSPs.
*  Description : Generates 2 sets of LSPs from 2 sets of
*                LP coefficients for mode 12.2. For the other
*                modes 1 set of LSPs is generated from 1 set of
*                LP coefficients. These LSPs are quantized with
*                Matrix/Vector quantization (depending on the mode)
*                and interpolated for the subframes not yet having
*                their own LSPs.
*                
**************************************************************************
*/
int lsp(lspState *st,        /* i/o : State struct                            */
        enum Mode req_mode,  /* i   : requested coder mode                    */
        enum Mode used_mode, /* i   : used coder mode                         */        
        Word16 az[],         /* i/o : interpolated LP parameters Q12          */
        Word16 azQ[],        /* o   : quantization interpol. LP parameters Q12*/
        Word16 lsp_new[],    /* o   : new lsp vector                          */ 
        Word16 **anap        /* o   : analysis parameters                     */
        );

#endif
