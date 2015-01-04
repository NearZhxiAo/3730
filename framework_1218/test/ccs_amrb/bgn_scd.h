/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef bgnscd_h
#define bgnscd_h "$Id $"
 
#include "typedef.h"
#include "cnst.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define L_ENERGYHIST 60
#define INV_L_FRAME 102


/* 2*(160*x)^2 / 65536  where x is FLP values 150,5 and 50 */
#define FRAMEENERGYLIMIT  17578         /* 150 */
#define LOWERNOISELIMIT      20         /*   5 */
#define UPPERNOISELIMIT    1953         /*  50 */

/*
********************************************************************************
*                         DEFINITION OF DATA TYPES
********************************************************************************
*/
typedef struct{
   /* history vector of past synthesis speech energy */
   Word16 frameEnergyHist[L_ENERGYHIST];
   
   /* state flags */
   Word16 bgHangover;       /* counter; number of frames after last speech frame */

} Bgn_scdState;
 
 
/*
**************************************************************************
*
*  Function    : Bgn_scd
*  Purpose     : Charaterice synthesis speech and detect background noise
*  Returns     : background noise decision; 0 = bgn, 1 = no bgn
*
**************************************************************************
*/
Word16 Bgn_scd (Bgn_scdState *st,      /* i : State variables for bgn SCD         */
                Word16 ltpGainHist[],  /* i : LTP gain history                    */
                Word16 speech[],       /* o : synthesis speech frame              */
                Word16 *voicedHangover /* o : # of frames after last voiced frame */
);
 
#endif
