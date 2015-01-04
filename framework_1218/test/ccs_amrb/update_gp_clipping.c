/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "copy.h"
#include "basic_op.h"
#include "ton_stab.h"
/***************************************************************************
 *
 *  Function:  Update_Gp_Clipping()                                          
 *  Purpose:   Update past pitch gain memory
 *                                                                         
 ***************************************************************************
 */
void update_gp_clipping(tonStabState *st, /* i/o : State struct            */
                        Word16 g_pitch    /* i   : pitch gain              */
)
{
   Copy(&st->gp[1], &st->gp[0], N_FRAME-1);
   st->gp[N_FRAME-1] = g_pitch >> 3;
}
