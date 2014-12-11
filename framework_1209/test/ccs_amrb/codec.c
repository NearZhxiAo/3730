/*
 *****************************************************************************
 *
 *      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
 *                                R99   Version 3.2.0                
 *                                REL-4 Version 4.0.0                
 *
 *****************************************************************************
 *
 *      File             : coder.c
 *      Purpose          : Speech encoder main program.
 *
 *****************************************************************************
 *
 *    Usage : coder speech_file  bitstream_file
 *
 *    Format for speech_file:
 *      Speech is read from a binary file of 16 bits data.
 *
 *    Format for bitstream_file:
 *        1 word (2-byte) for the TX frame type
 *          (see frame.h for possible values)
 *      244 words (2-byte) containing 244 bits.
 *          Bit 0 = 0x0000 and Bit 1 = 0x0001
 *        1 word (2-byte) for the mode indication
 *          (see mode.h for possible values)
 *        4 words for future use, currently written as zero
 *
 *****************************************************************************
 */

/*
 *****************************************************************************
 *                         INCLUDE FILES
 *****************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "typedef.h"
#include "cnst.h"
#include "mode.h"
#include "frame.h"
#include "pre_process.h"
#include "sid_sync.h"
#include "encoder_homing_frame_test.h"
#include "main_pc.h"
#include "speech_decode_frame.h"
#include "main_pd.h"
#include "amr_global_var.h"

long long  get_ustime(void)
{
	static char firsttimehere=1;                                                                                             
	static struct timeval timeorigin;                                                                                        
	struct timeval now;                                                                                                      

	// the first time, set the origin.                                                                                       
	if (firsttimehere) {                                                                                                     
		gettimeofday(&timeorigin,NULL);                                                                                        
		firsttimehere=0;                                                                                                       
	}                                                                                                                        
	gettimeofday(&now,NULL);                                                                                                 
	                                                                                                                  
	return 	(long long)( 
                            ( (long long)now.tv_sec - (long long)timeorigin.tv_sec )*1000000LL +
                            ( (long long)now.tv_usec - (long long)timeorigin.tv_usec )
			   );                                                                                                          
} 

void intel2moto(Word16 inout[],Word16 Lenth)
{
	unsigned char low;
	unsigned char high;

	unsigned short temp;

	int i;

	for (i = 0; i < Lenth; i++)
	{
		low = inout[i] & 0xff;
		high =  (inout[i] & 0xff00) >> 8;

		temp = low << 8;
		temp = temp + high;

		inout[i] = (Word16)temp;
	}
	return;
}

/*
 *****************************************************************************
 *                             MAIN PROGRAM 
 *****************************************************************************
 */
int main ()
{
	FILE *file_speech;           /* File of speech data               */
	FILE *file_synth;			 /* File of synth  data               */
	FILE *file_encod;           /* File of config                    */

	char *modeStr = NULL;
	char *usedModeStr = NULL;

	char SpeechFileName[80];
	char SynthFileName[80];

	Word16 new_speech[L_FRAME];         /* Pointer to new speech data        */
	Word16 rserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 oserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 new_synth[L_FRAME];    /* Output bitstream buffer           */
	Word16 org_synth[L_FRAME];    /* Output bitstream buffer           */

	Word32 frame;
	Word16 dtx = 0;                     /* enable encoder DTX                */

	int i;

	enum Mode mode;

	AMR_Encoder_Channel *p_Encoder_Channel_Mem;
	AMR_Decoder_Channel *p_Decoder_Channel_Mem;
        long long timea = 0;
        long long timeb = 0;
        long long timec = 0;
        long long timed = 0;


	cgOverflow = 0;
	cgCarry = 0;
	if((file_speech=fopen("T01.INP","rb"))==NULL)
	{
		printf("speech file not found\n");
		exit(0);
	}

	if ((file_encod = fopen ("T01_122.COD","rb")) == NULL)
	{
		printf("encod file not found\n");
		exit(0);
	}

	if ((file_synth = fopen ("T01_122.OUT","rb")) == NULL)
	{
		printf("synth file not found\n");
		exit(0);
	}

	mode = MR122;
	dtx = 0;
	/*-----------------------------------------------------------------------*
	 * Initialisation of the coder.                                          *
	 *-----------------------------------------------------------------------*/
	p_Encoder_Channel_Mem = &Encoder_Channel_Mem;
	Encoder_Channel_Malloc(p_Encoder_Channel_Mem);
	p_Encoder_Channel_Mem->mode = mode;
	p_Encoder_Channel_Mem->dtx = dtx;
	Encoder_Channel_Init(p_Encoder_Channel_Mem);

	/*-----------------------------------------------------------------------*
	 * Initialization of decoder                                             *
	 *-----------------------------------------------------------------------*/
	p_Decoder_Channel_Mem=&Decoder_Channel_Mem;
	Decoder_Channel_Malloc(p_Decoder_Channel_Mem);
	p_Decoder_Channel_Mem->reset_flag_old=1;  
	Decoder_Channel_Init(p_Decoder_Channel_Mem);

	/*-----------------------------------------------------------------------*
	 * Process speech frame by frame                                         *
	 *-----------------------------------------------------------------------*/
	frame = 0;
	while (fread (new_speech, 2, L_FRAME, file_speech) == L_FRAME)
	{

		frame++;

		/* get channel memory pointer */
		p_Encoder_Channel_Mem = &Encoder_Channel_Mem;

		//intel2moto(new_speech,L_FRAME);

		/* encoding */
		timea = get_ustime();
		Encoder_Channel_Proccess(new_speech,p_Encoder_Channel_Mem,rserial);   
		timeb = get_ustime();
		printf("\n\r------------------Encoder_Channel_Proccess frame = %d takes time = %lld----------------\r\n", frame, timeb-timea);

		fread (oserial, 2, SERIAL_FRAMESIZE, file_encod);
		//intel2moto(oserial,SERIAL_FRAMESIZE);

		for (i = 0; i < SERIAL_FRAMESIZE; i++)
		{
			if(oserial[i] != rserial[i])
			{
				printf("encoder failed at %d\n",frame);
				exit(0);
			}
		}
		printf("encoder passed at frame %d\n",frame);

		/* get channel memory pointer */
		p_Decoder_Channel_Mem = &Decoder_Channel_Mem;

		/* decoding */
		timec = get_ustime();
		Decoder_Channel_Process(p_Decoder_Channel_Mem,oserial,new_synth);
		timed = get_ustime();
		printf("\n\r------------------Dncoder_Channel_Proccess frame = %d takes time = %lld----------------\r\n", frame, timed-timec);

		fread(org_synth, sizeof(Word16), L_FRAME,file_synth);

		//intel2moto(org_synth,L_FRAME);

		for (i = 0; i < L_FRAME; i++)
		{
			if(org_synth[i] != new_synth[i])
			{
				printf("decoder failed at %d\n",frame);
				exit(0);
			}
		}

		printf("decoder passed at frame %d\n",frame);
	}
	printf("all passed\n");

	fclose(file_speech);
	fclose(file_synth);
	return (0);
}
