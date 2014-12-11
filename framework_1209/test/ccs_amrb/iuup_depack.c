#include "mode.h"
#include "frame.h"
#include "amr_global_var.h"

int iuup_depack(char IuupPacket[],short serial[])
{
	int					result;
	int					BitValue;
	int					BitIndex;
	int					ByteIndex;
	int					ByteValue;
	int					Shift;
	short				RxRfci;

	RxRfci = (short)IuupPacket[1];
	
	if (RxRfci == AmrCfgRfci)
	{
		switch(AmrCfgMode)
		{
		case MR122:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 244)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;				
			break;
		case MR102:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 204)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR795:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 159)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR74:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 148)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR67:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 134)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR59:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 118)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR515:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 103)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		case MR475:
			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 95)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SPEECH_GOOD;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			result = 0;
			break;
		default:
			result = -1;
			break;
		}
		m_AmrDecoderState = 0;
	}
	else 
	{
		if (m_AmrDecoderState == 0)
		{
			m_AmrDecoderState = 1;

			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 39)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SID_FIRST;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			
			result = 0;
		}
		else if (m_AmrDecoderState == 1)
		{
			m_AmrDecoderState = 2;

			ByteIndex = 4;
			BitIndex = 1;
			ByteValue = IuupPacket[ByteIndex];
			Shift = 0;
			while(BitIndex <= 39)
			{
				BitValue =  ByteValue & 0x01;
				serial[BitIndex] = BitValue;
			
				ByteValue = ByteValue >> 1;

				BitIndex = BitIndex + 1;
				
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					ByteValue = IuupPacket[ByteIndex];
				}
			}
			serial[0] = TX_SID_UPDATE;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
			
			result = 0;
			
		}
		else if (m_AmrDecoderState == 2)
		{
			serial[0] = TX_NO_DATA;
			serial[1+MAX_SERIAL_SIZE] = AmrCfgMode;
		}

		result = -1;
	}
		
	return result;
}