#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include "libosd.h"
#include "fonts.h"
#include "libv4l2_cap.h"

#define START_X 360
#define START_Y 240
int start_x, start_y;
void draw(char *word, char *data, DrawType drawtype, int colorValue) {
	FILE *fp	= NULL;
	int width 	= IMAGEWIDTH;
	int height 	= IMAGEHEIGHT;
	char *data_index;
	char *u_index;
	char *v_index;

	char *temp;
	int i, j, k, loc;
	int word_x, word_y, word_index;
	unsigned char curr_ch;


	word_y = 0;
	word_index = 0;

	if(start_y + 15 > height  || start_x + 15 > width)
	{
		printf("text location out range.\r\n");
		printf("start_x:%d.\r\nstart_y:%d", start_x, start_y);
		return;
	}

	for(i = start_y; i < height;)
	{
		data_index = data + (width*2*i);

	
		curr_ch = word[word_index++];

		j = 1;	
		for(k = 0; k < 8; k++)
		{
			loc = j + start_x*2;
			if((curr_ch >>(7-k)) & 0x1==1) 
			{
				//data_index[loc] = 0;
				data_index[loc] = (colorValue >> 8 ) & 0xff;
				data_index[loc+1] = colorValue & 0xff;
			}
			j = j + 2;
		}

		curr_ch = word[word_index++];
		for(k = 0; k < 8; k++)
		{
			loc = j + start_x*2;
			if((curr_ch >>(7-k)) & 0x1==1) 
			{
				data_index[loc] = (colorValue >> 8 ) & 0xff;
				data_index[loc+1] = colorValue & 0xff;
				//data_index[loc] = colorValue;
			//	data_index[loc] = 0;
			}

			j = j + 2;
		}

		i++;
		temp = data + (width*2*i);

		if(word_index >= 32)
		{
			break;
		}
	}

	if (drawtype == TIME || drawtype == CHAR)
	{
		start_x += 8;
	}
	else
	{
		start_x += 20;
	}
}


int findIndex(char data)
{
	int i = 0;
	
	for(i = 0; i < MAX_FONTS; i++)
	{
		if(font_index[i] == data)
		{
			return i;
		}
	}

	return -1;
}

char *get_datetime()
{
	static char datatime[128];
	struct tm  *ptm;
	long   ts;
	int    y, m, d, h, n, s;

	memset(datatime, 0x00, 128);

	ts = time(NULL);
	ptm = localtime(&ts);

	y = ptm->tm_year+1900; 
	m = ptm->tm_mon+1;     
	d = ptm->tm_mday;      
	h = ptm->tm_hour;      
	n = ptm->tm_min;       
	s = ptm->tm_sec;       

	sprintf(datatime, "%d-%d-%d %d:%d:%d", y, m, d, h, n, s);

	return datatime;
}
//画画


int set_osd(char *data, DrawType drawtype, char *text, int x, int y, int colorValue)
{
	unsigned char *datetime =  NULL;
	int i, index;
	char word[32];
	char filename[] = "16px_zpix.hzk";
	FILE *fp_font = NULL;
	unsigned char loc_q, loc_w, a1, a2;
	int location;
	

	start_x	= x;
	start_y	= y;
	
	return 0;
	switch(drawtype)
	{
		case TIME://叠加时间(数字)或字母
		case CHAR:
			{
				if (drawtype == TIME)
				{
					datetime = get_datetime();
					for(i = 0;  i < strlen(datetime); i++) 
					{
						index = findIndex(datetime[i]);
						if(index == -1)
						{
							printf("can not find %c value:%x fonts.\r\n", datetime[i], datetime[i]);	
							index = findIndex('?');
							draw(fonts[index], data, drawtype, colorValue);
						}
						else
						{
							draw(fonts[index], data, drawtype, colorValue);
						}
					}
				}
				else
				{
					for(i = 0;  i < strlen(text); i++) 
					{
						index = findIndex(text[i]);
						if(index == -1)
						{
							printf("can not find %c value:%x fonts.\r\n", text[i], text[i]);	
							index = findIndex('?');
							draw(fonts[index], data, drawtype, colorValue);
						}
						else
						{
							draw(fonts[index], data, drawtype, colorValue);
						}
					}

				}

				break;
			}
		case CHINESE: //写中文字体
			{
			//	unsigned char text1[]="郑少雄";
				fp_font = fopen(filename, "rb+");
				if (fp_font == NULL)
				{
					printf("open font file failed.\r\n");
					return -1;
				}
				for (i = 0; i < strlen(text);)
				{
					loc_q = text[i++] - 0xA0;
					loc_w = text[i++] - 0xA0;

					location = ((loc_q - 1)*94 + (loc_w - 1))*32;
					fseek(fp_font, location, SEEK_SET);
					fread(word, 32, 1, fp_font);
					draw(word, data, CHINESE, colorValue);
				}
				fclose(fp_font);
				break;
			}
		default:
			{
				printf("the drawtype is not known!\r\n");
				return -1;
			}
	}

	

	return 0;
}

