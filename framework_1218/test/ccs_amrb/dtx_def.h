/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*
*      File             : dtx_enc.h
*      Purpose          : DTX mode computation of SID parameters
*
********************************************************************************
*/
#ifndef dtx_def_h
#define dtx_def_h "$Id $" 
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "cnst.h"
#include "q_plsf.h"
#include "gc_pred.h"
#include "mode.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define DTX_HIST_SIZE 8
#define DTX_ELAPSED_FRAMES_THRESH (24 + 7 -1)
#define DTX_HANG_CONST 7             /* yields eight frames of SP HANGOVER  */

/*
********************************************************************************
*                         DEFINITION OF DATA TYPES
********************************************************************************
*/
typedef struct {
   Word16 lsp_hist[M * DTX_HIST_SIZE];
   Word16 log_en_hist[DTX_HIST_SIZE];
   Word16 hist_ptr;
   Word16 log_en_index;
   Word16 init_lsf_vq_index;
   Word16 lsp_index[3];

   /* DTX handler stuff */
   Word16 dtxHangoverCount;
   Word16 decAnaElapsedCount;

} dtx_encState;
/*
********************************************************************************
*                         DECLARATION OF PROTOTYPES
********************************************************************************
*/
#endif
