/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst_vad.h"
#include "vad1.h"
#include "first_filter_stage.h"
#include "filter5.h"
#include "filter3.h"
#include "level_calculation.h"
/****************************************************************************
 *
 *     Function     : filter_bank
 *     Purpose      : Divides input signal into 9-bands and calculas level of
 *                    the signal in each band 
 *
 ***************************************************************************/
void filter_bank(vadState1 *st,  /* i/o : State struct               */
                        Word16 in[],   /* i   : input frame                */
                        Word16 level[] /* 0   : signal levels at each band */
                        )
{
  Word16 i;
  Word16 tmp_buf[FRAME_LEN];

  /* calculate the filter bank */

  first_filter_stage(in, tmp_buf, st->a_data5[0]);
  
  for (i = 0; i < FRAME_LEN/4; i++)
  {
     filter5(&tmp_buf[4*i], &tmp_buf[4*i+2], st->a_data5[1]);
     filter5(&tmp_buf[4*i+1], &tmp_buf[4*i+3], st->a_data5[2]);
  }
  for (i = 0; i < FRAME_LEN/8; i++)
  {
     filter3(&tmp_buf[8*i+0], &tmp_buf[8*i+4], &st->a_data3[0]);
     filter3(&tmp_buf[8*i+2], &tmp_buf[8*i+6], &st->a_data3[1]);
     filter3(&tmp_buf[8*i+3], &tmp_buf[8*i+7], &st->a_data3[4]);
  }
  
  for (i = 0; i < FRAME_LEN/16; i++)
  {
     filter3(&tmp_buf[16*i+0], &tmp_buf[16*i+8], &st->a_data3[2]);
     filter3(&tmp_buf[16*i+4], &tmp_buf[16*i+12], &st->a_data3[3]);
  }
  
  /* calculate levels in each frequency band */
  
  /* 3000 - 4000 Hz*/
  level[8] = level_calculation(tmp_buf, &st->sub_level[8], FRAME_LEN/4-8,
                               FRAME_LEN/4, 4, 1, 15);
  
  /* 2500 - 3000 Hz*/  
  level[7] = level_calculation(tmp_buf, &st->sub_level[7], FRAME_LEN/8-4,
                               FRAME_LEN/8, 8, 7, 16);
  
  /* 2000 - 2500 Hz*/
  level[6] = level_calculation(tmp_buf, &st->sub_level[6], FRAME_LEN/8-4,
                               FRAME_LEN/8, 8, 3, 16);
  
  /* 1500 - 2000 Hz*/
  level[5] = level_calculation(tmp_buf, &st->sub_level[5], FRAME_LEN/8-4,
                               FRAME_LEN/8, 8, 2, 16);
  
  /* 1000 - 1500 Hz*/
  level[4] = level_calculation(tmp_buf, &st->sub_level[4], FRAME_LEN/8-4,
                               FRAME_LEN/8, 8, 6, 16);
  
  /* 750 - 1000 Hz*/
  level[3] = level_calculation(tmp_buf, &st->sub_level[3], FRAME_LEN/16-2,
                               FRAME_LEN/16, 16, 4, 16);
  
  /* 500 - 750 Hz*/
  level[2] = level_calculation(tmp_buf, &st->sub_level[2], FRAME_LEN/16-2,
                               FRAME_LEN/16, 16, 12, 16);
  
  /* 250 - 500 Hz*/
  level[1] = level_calculation(tmp_buf, &st->sub_level[1], FRAME_LEN/16-2,
                               FRAME_LEN/16, 16, 8, 16);
  
  /* 0 - 250 Hz*/
  level[0] = level_calculation(tmp_buf, &st->sub_level[0], FRAME_LEN/16-2,
                               FRAME_LEN/16, 16, 0, 16);
  
}
