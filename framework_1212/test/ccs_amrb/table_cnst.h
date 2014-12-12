
#include "typedef.h"    
#include "pitch_fr.h"

#ifndef table_cnst_h
#define table_cnst_h "$ID $"

/***********************bitno.tab*********************************************************/
/* number of parameters per modes (values must be <= MAX_PRM_SIZE!) */
extern const Word16 prmno[];
/* number of parameters to first subframe per modes */
extern const Word16 prmnofsf[]; 
/* parameter sizes (# of bits), one table per mode */
extern Word16 bitno_MR475[];
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



/***************************window.tab*****************************************************/
extern const Word16 window_200_40[];
extern const Word16 window_160_80[];  
extern const Word16 window_232_8[];  

/***************************sqrt_l.tab*****************************************************/
extern const Word16 table_sqrt_l[];


/***************************qua_gain.tab*****************************************************/
extern const Word16 table_gain_highrates[];
extern const Word16 table_gain_lowrates[];


/***************************qgain475.tab*****************************************************/
extern const Word16 table_gain_MR475[];




/***************************q_plsf_5.tab*****************************************************/
extern Word16 mean_lsf_5[];
extern const Word16 dico1_lsf_5[];
extern const Word16 dico2_lsf_5[];
extern const Word16 dico3_lsf_5[];
extern const Word16 dico4_lsf[]; 
extern const Word16 dico5_lsf[];


/***************************q_plsf_3.tab*****************************************************/
extern const Word16 past_rq_init[];
extern const Word16 mean_lsf_3[];
extern const Word16 pred_fac[];
extern const Word16 dico1_lsf_3[];
extern const Word16 dico2_lsf_3[];
extern const Word16 dico3_lsf_3[];
extern const Word16 mr515_3_lsf[];
extern const Word16 mr795_1_lsf[];


/**************************pre_process.tab**************************************************/
extern const Word16 b_encoder[];
extern const Word16 a_encoder[]; 

/* filter coefficients (fc = 60 Hz) */
extern const Word16 b_decoder[];
extern const Word16 a_decoder[];

/***************************pitch_fr.tab*****************************************************/
//extern const mode_dep_parmSt mode_dep_parm[];

/***************************pow2.tab*****************************************************/
extern const Word16 table_pow2[];


/****************************lsp.tab****************************************************/
extern const Word16 lsp_init_data[];

/****************************lsp_lsf.tab************************************************/
extern const Word16 table_lsp_lsf[];
extern const Word16 slope[];

/****************************ph_disp.tab****************************************************/
extern  const Word16 ph_imp_low_MR795[];
extern  const Word16 ph_imp_mid_MR795[];
extern  const Word16 ph_imp_low[]; 
extern  const Word16 ph_imp_mid[];

/****************************gc_pred.tab****************************************************/
extern const Word16 pred[];
extern const Word16 pred_MR122[];

/**************************gray.tab******************************************************/
extern const Word16 gray[];
extern const Word16 dgray[];


/***************************grid.tab*****************************************************/
extern const Word16 grid[];

/***************************inter_36.tab*****************************************************/
extern const Word16 inter_6_corr[];
extern const Word16 inter_6_exc[];


/****************************inv_sqrt.tab****************************************************/
extern const Word16 table_inv_sqrt[];


/***************************lag_wind.tab*****************************************************/
extern const Word16 lag_h[];
extern const Word16 lag_l[];


/***************************log2.tab*****************************************************/
extern const Word16 table_log2[];



/***************************c2_11pf.tab*****************************************************/
extern const Word16 startPos1[];
extern const Word16 startPos2[];

/****************************c2_9pf.tab****************************************************/
extern const Word16 startPos[];
extern const Word16 trackTable[];

 
/***************************corrwght.tab*****************************************************/
extern const Word16 corrweight[];


/***************************d_homing.tab*****************************************************/
extern const Word16 dhf_MR475[];
extern const Word16 dhf_MR515[]; 
extern const Word16 dhf_MR59[];
extern const Word16 dhf_MR67[];
extern const Word16 dhf_MR74[];
extern const Word16 dhf_MR795[];
extern const Word16 dhf_MR102[];
extern const Word16 dhf_MR122[];
extern const Word16 *dhf[];

/***************************gains.tab*****************************************************/
extern const Word16 qua_gain_pitch[];
extern const Word16 qua_gain_code[];


/***************************gamma.tab*****************************************************/
extern const Word16 gamma1[];
extern const Word16 gamma1_12k2[]; 
extern const Word16 gamma2[];
extern const Word16 gamma3_MR122[];
extern const Word16 gamma3[];
extern const Word16 gamma4_MR122[];
extern const Word16 gamma4[];

extern Flag Overflow ;
extern Flag Carry ;

/******************dtx_dec.tab***************************************************/  

extern const Word16 lsf_hist_mean_scale[];
extern const Word16 dtx_log_en_adjust[];

/********************ec_gains.tab************************************************/

/* constant for ec_gains */
extern const Word16 pdown[];
extern const Word16 cdown[];

/********************************************************************************/

/**************************scramble order table**********************************/

extern const Word16 MR475_order_table[];

extern const Word16 MR515_order_table[];

extern const Word16 MR59_order_table[];

extern const Word16 MR67_order_table[];

extern const Word16 MR74_order_table[];
                 
extern const Word16 MR795_order_table[];
                  
extern const Word16 MR102_order_table[];

extern const Word16 MR122_order_table[];

/************************** constant table for G.711 function ********************/
extern const Word16 seg_end[];

/************************** homing table for all mode *****************************/
/* little edian mode homing frame and header length is 24 bites*/
extern const unsigned int Le3_homing_MR475[];
extern const unsigned int Le3_homing_MR515[];
extern const unsigned int Le3_homing_MR59[];
extern const unsigned int Le3_homing_MR67[];
extern const unsigned int Le3_homing_MR74[];
extern const unsigned int Le3_homing_MR795[];
extern const unsigned int Le3_homing_MR102[];
extern const unsigned int Le3_homing_MR122[];

/* big edian mode homing frame and header length is 24 bites*/
extern const unsigned int Be4_homing_MR475[];
extern const unsigned int Be4_homing_MR515[];
extern const unsigned int Be4_homing_MR59[];
extern const unsigned int Be4_homing_MR67[];
extern const unsigned int Be4_homing_MR74[];
extern const unsigned int Be4_homing_MR795[];
extern const unsigned int Be4_homing_MR102[];
extern const unsigned int Be4_homing_MR122[];

#endif



