#ifndef table_h
#define table_h "$ID table_h $"

#include "typedef.h"
#include "mode.h"
#include "pred_lt_3or6.h"

/*
********************************************************************************
*      structure for pitch_fr()
********************************************************************************
*/

typedef struct 
{
    Word16 max_frac_lag;     /* lag up to which fractional lags are used    */
    Word16 flag3;            /* enable 1/3 instead of 1/6 fract. resolution */
    Word16 first_frac;       /* first fractional to check                   */
    Word16 last_frac;        /* last fractional to check                    */
    Word16 delta_int_low;    /* integer lag below TO to start search from   */
    Word16 delta_int_range;  /* integer range around T0                     */
    Word16 delta_frc_low;    /* fractional below T0                         */
    Word16 delta_frc_range;  /* fractional range around T0                  */
    Word16 pit_min;          /* minimum pitch                               */
} mode_dep_parmSt;
/*
********************************************************************************
*      Purpose          : Table for routine sqrt_l_exp()
********************************************************************************
*/
/* table[i] = sqrt((i+16)*2^-6) * 2^15, i.e. sqrt(x) scaled Q15 */

extern const Word16 table_sqrt_l_exp[];

extern const Word16 window_200_40[];


/* window for EFR, first two subframes, no lookahead */

extern const Word16 window_160_80[];

/* window for EFR, last two subframes, no lookahead */

extern const Word16 window_232_8[];

#define VQ_SIZE_LOWRATES 64
#define VQ_SIZE_HIGHRATES 128

/* table used in 'high' rates: MR67 MR74 */
extern const Word16 table_gain_highrates[];


/* table used in 'low' rates: MR475, MR515, MR59 */
extern const Word16 table_gain_lowrates[];


#define MR475_VQ_SIZE 256

/* The table contains the following data:
 *
 *    g_pitch(0)        (Q14) // for sub-
 *    g_fac(0)          (Q12) // frame 0 and 2
 *    g_pitch(1)        (Q14) // for sub-
 *    g_fac(2)          (Q12) // frame 1 and 3
 *
 */
extern const Word16 table_gain_MR475[];

extern const Word16 mean_lsf_5[10];

#define DICO1_SIZE_5  128
#define DICO2_SIZE_5  256
#define DICO3_SIZE_5  256
#define DICO4_SIZE_5  256
#define DICO5_SIZE_5  64

extern const Word16 dico1_lsf_5[];

extern const Word16 dico2_lsf_5[];

extern const Word16 dico3_lsf_5[];

extern const Word16 dico4_lsf_5[];

extern const Word16 dico5_lsf_5[];


#define PAST_RQ_INIT_SIZE 8
#define DICO1_SIZE_3  256
#define DICO2_SIZE_3  512
#define DICO3_SIZE_3  512
#define MR515_3_SIZE  128
#define MR795_1_SIZE  512

/* initalization table for MA predictor in dtx mode */
extern const Word16 past_rq_init[];


extern const Word16 mean_lsf_3[];


extern const Word16 pred_fac[];

/* first codebook from IS641 */

extern const Word16 dico1_lsf_3[];

/* second codebook from IS641 */

extern const Word16 dico2_lsf_3[];


/* third codebook from IS641 */
extern const Word16 dico3_lsf_3[];

/* third codebook for MR475, MR515 */

extern const  Word16 mr515_3_lsf[];

/* first codebook for MR795 */

extern const Word16 mr795_1_lsf[];

extern const Word16 inter_6[];

extern const Word16 b_enc[];
extern const Word16 a_enc[];

extern const Word16 table_pow2[];

/* filter coefficients (fc = 60 Hz) */
extern const Word16 b_dec[];
extern const Word16 a_dec[];

extern const Word16 ph_imp_low_MR795[];
extern const Word16 ph_imp_mid_MR795[];

extern const Word16 ph_imp_low[];
extern const Word16 ph_imp_mid[];


extern const Word16 table_lsp_lsf[];

/* 0x8000 = -32768 (used to silence the compiler) */

/* slope used to compute y = acos(x) */

extern const Word16 slope_lsp_lsf[];

extern const Word16 lsp_init_data[];

extern const Word16 table_log2[];

extern const Word16 lag_h[];

extern const Word16 lag_l[];

extern const Word16 table_inv_sqrt[];

extern const Word16 inter_3or6[];

#define grid_points 60
extern const Word16 grid[];

extern const Word16 gray[];
extern const Word16 dgray[];

extern const Word16 gamma1[];

/* gamma1 differs for the 12k2 coder */
extern const Word16 gamma1_12k2[];

extern const Word16 gamma2[];
/* Spectral expansion factors */
extern const Word16 gamma3_MR122[];

extern const Word16 gamma3[];

extern const Word16 gamma4_MR122[];

extern const Word16 gamma4[];

#define NB_QUA_PITCH 16
#define NB_QUA_CODE 32

extern const Word16 qua_gain_pitch[];

extern const Word16 qua_gain_code[];

extern const Word16 corrweight[];


#define BIT_0      0
#define BIT_1      1

#define PRMNO_MR475 17
#define PRMNO_MR515 19
#define PRMNO_MR59  19
#define PRMNO_MR67  19
#define PRMNO_MR74  19
#define PRMNO_MR795 23
#define PRMNO_MR102 39
#define PRMNO_MR122 57
#define PRMNO_MRDTX 5
/* number of parameters to first subframe */
#define PRMNOFSF_MR475 7
#define PRMNOFSF_MR515 7
#define PRMNOFSF_MR59  7
#define PRMNOFSF_MR67  7
#define PRMNOFSF_MR74  7
#define PRMNOFSF_MR795 8
#define PRMNOFSF_MR102 12
#define PRMNOFSF_MR122 18

/* number of parameters per modes (values must be <= MAX_PRM_SIZE!) */
extern const Word16 prmno[];



/* number of parameters to first subframe per modes */
extern const Word16 prmnofsf[];



/* parameter sizes (# of bits), one table per mode */

extern const Word16 bitno_MR475[];

extern const Word16 bitno_MR515[];

extern const Word16 bitno_MR59[];

extern const Word16 bitno_MR67[];

extern const Word16 bitno_MR74[];

extern const Word16 bitno_MR795[];

extern const Word16 bitno_MR102[];

extern const Word16 bitno_MR122[];

extern const Word16 bitno_MRDTX[];

/* overall table with all parameter sizes for all modes */
extern const Word16 *bitno[];


extern const Word16 startPos1[];
extern const Word16 startPos2[];
extern const Word16 startPos[];
extern const Word16 trackTable[];


extern const Word16 dhf_MR475[];
   
extern const Word16 dhf_MR515[];

extern const Word16 dhf_MR59[];

extern const Word16 dhf_MR67[];

extern const Word16 dhf_MR74[];

extern const Word16 dhf_MR795[];

extern const Word16 dhf_MR102[];

extern const Word16 dhf_MR122[];
        


/* overall table with the parameters of the
   decoder homing frames for all modes */
extern const Word16 *dhf[];

extern const Word16 pred[];
extern const Word16 pred_MR122[];


extern const Word16 lsf_hist_mean_scale[];

/*************************************************
 * level adjustment for different modes Q11      *
 *************************************************/
extern const Word16 dtx_log_en_adjust[];

extern const Word16 cdown[];

extern const Word16 pdown[];

/***************************pitch_fr.tab*****************************************************/
extern const mode_dep_parmSt mode_dep_parm[];

extern Flag cgOverflow;
extern Flag cgCarry;

extern const Word16 MR475_order_table[];

extern const Word16 MR515_order_table[];

extern const Word16 MR59_order_table[];

extern const Word16 MR67_order_table[];

extern const Word16 MR74_order_table[];
                 
extern const Word16 MR795_order_table[];
                  
extern const Word16 MR102_order_table[];

extern const Word16 MR122_order_table[];

#endif

