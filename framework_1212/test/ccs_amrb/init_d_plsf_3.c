/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "cnst.h"
#include "copy.h"
#include "table.h"
#include "d_plsf.h"


void Init_D_plsf_3(
				   D_plsfState *st,  /* i/o: State struct                */
				   Word16 index      /* i  : past_rq_init[] index [0, 7] */
				   )
{
  Copy(&past_rq_init[index * M], st->past_r_q, M);
}
