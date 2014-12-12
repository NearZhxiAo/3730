#ifndef packeting_h
#define packeting_h "$ID packeting1_h  $"
#include "mode.h"

/*******************************************************/
void AMR_Framing3(
					Word16  		serial[],
					unsigned int	AMRPacketTx[]
				 );     
				 
enum Mode 
AMR_DeFraming3(
				unsigned int	AMRPacketRx[],
				Word16 			serial[]
			   );


/*******************************************************/
void AMR_Framing4(
					Word16  		serial[],
					unsigned char	AMRPacketTx[]
				 );     
				 
enum Mode 
AMR_DeFraming4(
				unsigned char	AMRPacketRx[],
				Word16 			serial[]
			   );

#endif


