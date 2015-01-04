#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <AMRCodec.h>


#define L_FRAME 160
#define SERIAL_FRAMESIZE 250

/*const Word16 MR122_order_table[] = 
{    
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,23,15,16,17,18,
	19,20,21,22,24,25,26,27,28,38,
	141,39,142,40,143,41,144,42,145,43,
	146,44,147,45,148,46,149,47,97,150,
	200,48,98,151,201,49,99,152,202,86,
	136,189,239,87,137,190,240,88,138,191,
	241,91,194,92,195,93,196,94,197,95,
	198,29,30,31,32,33,34,35,50,100,
	153,203,89,139,192,242,51,101,154,204,
	55,105,158,208,90,140,193,243,59,109,
	162,212,63,113,166,216,67,117,170,220,
	36,37,54,53,52,58,57,56,62,61,
	60,66,65,64,70,69,68,104,103,102,
	108,107,106,112,111,110,116,115,114,120,
	119,118,157,156,155,161,160,159,165,164,
	163,169,168,167,173,172,171,207,206,205,
	211,210,209,215,214,213,219,218,217,223,
	222,221,73,72,71,76,75,74,79,78,
	77,82,81,80,85,84,83,123,122,121,
	126,125,124,129,128,127,132,131,130,135,
	134,133,176,175,174,179,178,177,182,181,
	180,185,184,183,188,187,186,226,225,224,
	229,228,227,232,231,230,235,234,233,238,
	237,236,96,199
};*/



void Glin_Encode_Amr_Frame(char* amr_frame_bit, Word16* amr_frame_byte);
/*
*****************************************************************************
*                             MAIN PROGRAM 
*****************************************************************************
*/


void main(void)
{
 
	FILE *file_speech;           /* File of speech data               */
	FILE *file_synth;			 /* File of synth  data               */
	FILE *file_encod;           /* File of config  */
	FILE *file_amrResult;
	FILE *file_amr;
	FILE *file_dec;
	char   amr_frame[32] = {0};
	char   amr_filehead[6] = {0x23, 0x21, 0x41, 0x4D, 0x52, 0x0A};

	Word16 new_speech[L_FRAME];         /* Pointer to new speech data        */
	Word16 rserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 oserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 new_synth[L_FRAME];    /* Output bitstream buffer           */
	Word16 org_synth[L_FRAME];    /* Output bitstream buffer           */

	Word32 frame;

	int i;
  
	if ((file_speech = fopen ("e:\\ccs_amrl\\amrtest\\testinp4\\T04.INP","rb")) == NULL)
	{
		printf("speech file not found\n");
		exit(0);
	}

	if ((file_encod = fopen ("e:\\ccs_amrl\\amrtest\\testcod4\\122\\T04_122.COD","rb")) == NULL)
	{
		printf("encod file not found\n");
		exit(0);
	}
	if ((file_amr = fopen ("f:\\AMR.AMR","wb")) == NULL)
	{
		printf("AMR file not found\n");
		exit(0);
	}
	if ((file_dec = fopen ("e:\\dec.inp","wb")) == NULL)
	{
		printf("AMR file not found\n");
		exit(0);
	}


	if(fwrite(amr_filehead, sizeof(char), 6, file_amr) != 6)
    {
        printf("fwrite() return error 0\n");
        exit(0);
    }

	if ((file_synth = fopen ("e:\\ccs_amrl\\amrtest\\testcod4\\122\\T04_122.OUT","rb")) == NULL)
	{
		printf("synth file not found\n");
		exit(0);
	}

	//if ((file_amrResult = fopen("E:\\AMR_RESULT.OUT","rb+")) == NULL)
	//{
	//	printf("out file not found\n");
	//	exit(0);
	//}

	frame = 0;
	while (fread (new_speech, 2, L_FRAME, file_speech) == L_FRAME)
	{
		
		frame++;

		/* get channel memory pointer */
	
		if(1==frame)
		{
		    Encoder(new_speech,rserial,1);
		}
		else
		{
		    Encoder(new_speech,rserial,0);
		}

		/* amr成帧*/
        //Glin_Encode_Amr_Frame(amr_frame, rserial);
	
		/*if(fwrite(amr_frame, sizeof(char), 32, file_amr) != 32)
        {
            printf("fwrite() return error 1\n");
            exit(0);
        }*/
		


		/*与标准序列比较*/
		fread (oserial, 2, SERIAL_FRAMESIZE, file_encod);

		intel2moto(oserial,SERIAL_FRAMESIZE);

		
		for (i = 0; i < SERIAL_FRAMESIZE; i++)
		{
			if(oserial[i] != rserial[i])
			{
				printf("encoder failed at %d\n",frame);
				break;
			}
		}
		printf("encoder passed at frame %d\n",frame);

		if(1==frame)
		{
		    Decoder(oserial,new_synth,1);
		}
		else
		{
		    Decoder(oserial,new_synth,0);
		}

		fread(org_synth, sizeof(Word16), L_FRAME,file_synth);

		//intel2moto(org_synth,L_FRAME);
		intel2moto(new_synth,L_FRAME);

		for (i = 0; i < L_FRAME; i++)
		{
			if(org_synth[i] != new_synth[i])
			{
				printf("decoder failed at %d\n",frame);
				break;
			}
		}
		printf("decoder passed at frame %d\n",frame);
		
		fwrite(new_synth, sizeof(Word16), 160, file_dec);

		
	}
	printf("all passed\n");

	fclose(file_speech);
	fclose(file_encod);
	fclose(file_synth);
	//return (0);
}



#if 0
void main(void)
{
   int a=100;
   int b=200;
   int c;
   //c=add(a,b);

	Word16 new_speech[L_FRAME];         /* Pointer to new speech data        */
	Word16 rserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 oserial[SERIAL_FRAMESIZE];    /* Output bitstream buffer           */
	Word16 new_synth[L_FRAME];    /* Output bitstream buffer           */
	Word16 org_synth[L_FRAME];    /* Output bitstream buffer           */
    //int c;
    //c=add(a,b);
    //c=Encoder();
	c=Encoder(new_speech,rserial,1);
    printf("%d\n",c);
}
#endif



void Glin_Encode_Amr_Frame(char* amr_frame_bit, Word16* amr_frame_byte)
{
    int    i = 0;
    char   temp = 0;
    char   temp_byte = 0;
    
    amr_frame_bit[0] = 0x3C;

    for (i = 0; i < 244; i++)
    {
        //temp_byte = (unsigned char)amr_frame_byte[MR122_order_table[i] + 1];
        //temp_byte = (unsigned char)amr_frame_byte[i];
        temp |= (temp_byte << (7 - (i % 8)));
        
        if (0 == ((i + 1) % 8))
        {
            amr_frame_bit[(i + 1) / 8] = temp;
            temp = 0;
		
        } 
    }


    


    return;
}