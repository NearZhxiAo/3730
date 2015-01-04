/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef update_gp_clipping_h
#define update_gp_clipping_h "$Id $"

#include "typedef.h"
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
);

#endif

