#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <system_includes.h>
#include <system_debug.h>


/* The number of channels of the audio codec */
#define OSA_SOK                0
#define OSA_EFAIL               -1

/* Number of samples to process at once */
#define NUMSAMPLES             1000

#define INPUTBUFSIZE           NUMSAMPLES

/* The max amount of bytes of speech data to process at once */
#define ENCODEDBUFSIZE         NUMSAMPLES / 2


int main(int argc, char **argv )
{
  int status, len, num;
  FILE *fp1 = NULL;
  FILE *fp2 = NULL;
  
  unsigned short i, j;
  short data;
  unsigned short isNegative;
  short nOut;
  short lowByte = 1;
  int outputSize = NUMSAMPLES / 2;

  unsigned short *inBuffer  = NULL;
  short *outBuffer          = NULL;

  outBuffer     = (unsigned short *)malloc(ENCODEDBUFSIZE);
  inBuffer 	= (short *)malloc(INPUTBUFSIZE);

  if (outBuffer == NULL || inBuffer == NULL) 
  {
      SYS_ERROR("Failed to allocate buffer\n");
      return OSA_EFAIL;
  }


  fp1 = fopen("in.wav", "rb+");
  fseek(fp1, 0, SEEK_END);
  len = ftell(fp1);
  fseek(fp1, 0, SEEK_SET);
  num = (int)(len/1000);

  fp2 = fopen("out.g711", "ab+");  

  for(j = 0; j<num; j++)
  {
	  fread(inBuffer, NUMSAMPLES, 1, fp1);   


	  for (i = 0; i < outputSize; i++) {
		  data = *((short *)inBuffer + i);
		  data >>= 2;
		  isNegative = (data < 0 ? 1 : 0);
		  if (isNegative)
			  data = -data;

		  if (data <= 1) {
			  nOut = (char) data;
		  } else if (data <= 31) {
			  nOut = ((data - 1) >> 1) + 1;
		  } else if (data <= 95) {
			  nOut = ((data - 31) >> 2) + 16;
		  } else if (data <= 223) {
			  nOut = ((data - 95) >> 3) + 32;
		  } else if (data <= 479) {
			  nOut = ((data - 223) >> 4) + 48;
		  } else if (data <= 991) {
			  nOut = ((data - 479) >> 5) + 64;
		  } else if (data <= 2015) {
			  nOut = ((data - 991) >> 6) + 80;
		  } else if (data <= 4063) {
			  nOut = ((data - 2015) >> 7) + 96;
		  } else if (data <= 7903) {
			  nOut = ((data - 4063) >> 8) + 112;
		  } else {
			  nOut = 127;
		  }

		  if (isNegative) {
			  nOut = 127 - nOut;
		  } else {
			  nOut = 255 - nOut;
		  }

		  // Pack the bytes in a word
		  if (lowByte)
			  *(outBuffer + (i >> 1)) = (nOut & 0x00FF);
		  else
			  *(outBuffer + (i >> 1)) |= ((nOut << 8) & 0xFF00);
		  lowByte ^= 0x1;
	  }


	  fwrite(outBuffer, outputSize,  1, fp2);

  }

  fclose(fp1);
  fclose(fp2);

  if( inBuffer )
  {
     free(inBuffer);
  }

  if( outBuffer )
  {
      free(outBuffer);
  }


  return OSA_SOK;
}
