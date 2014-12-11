/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "post_filter.h"
#include "agc_init.h"
#include "post_filter_reset.h"
#include "preemphasis_init.h"

/*************************************************************************
*
*  Function:   Post_Filter_init
*  Purpose:    Allocates memory for filter structure and initializes
*              state memory
*
**************************************************************************
*/
int Post_Filter_init (Post_FilterState *state)
{
  preemphasis_init(state->preemph_state);
  agc_init(state->agc_state);
      
  Post_Filter_reset(state);
  
  return 0;
}
