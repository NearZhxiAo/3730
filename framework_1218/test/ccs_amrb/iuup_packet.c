#include "mode.h"
#include "frame.h"
#include "amr_global_var.h"

int iuup_packet(short serial[],char IuupPacket[])
{
	int					Result;
	enum TXFrameType	FrameType;
	int					Bit;
	int					BitIndex;
	int					ByteIndex;
	int					Shift;

	AmrTxFrameNo = AmrTxFrameNo + 1;
	if (AmrTxFrameNo == 16)
	{
		AmrTxFrameNo = 0;
	}
	IuupPacket[0] = (char)AmrTxFrameNo;

	FrameType = (enum TXFrameType)serial[0];
	
	switch(FrameType)
	{
	case TX_SPEECH_GOOD:
		IuupPacket[1] = (char)AmrCfgRfci;
		switch(AmrCfgMode)
		{
		case MR122:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 244)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR102:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 204)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR795:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 159)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR74:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 148)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR67:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 134)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR59:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 118)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR515:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 103)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		case MR475:
			BitIndex = 1;
			ByteIndex = 4;
			Shift = 0;
			IuupPacket[ByteIndex] = 0;
			while(BitIndex <= 95)
			{
				Bit = serial[BitIndex];
				Bit = Bit << Shift;
				IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
				Shift = Shift + 1;
				if (Shift == 8)
				{
					Shift = 0;
					ByteIndex = ByteIndex + 1;
					IuupPacket[ByteIndex] = 0;
				}
				BitIndex = BitIndex + 1;
			}
			Result = 0;
			break;
		default:
			Result = -1;
			break;
		}
		break;
		
	case TX_SID_FIRST:
		IuupPacket[1] = (char)AmrCfgDtxRfci;
		BitIndex = 1;
		ByteIndex = 4;
		Shift = 0;
		IuupPacket[ByteIndex] = 0;
		while(BitIndex <= 39)
		{
			Bit = serial[BitIndex];
			Bit = Bit << Shift;
			IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
			Shift = Shift + 1;
			if (Shift == 8)
			{
				Shift = 0;
				ByteIndex = ByteIndex + 1;
				IuupPacket[ByteIndex] = 0;
			}
			BitIndex = BitIndex + 1;
		}
		Result = 0;
		break;
	case TX_SID_UPDATE:
		IuupPacket[1] = (char)AmrCfgDtxRfci;
		BitIndex = 1;
		ByteIndex = 4;
		Shift = 0;
		IuupPacket[ByteIndex] = 0;
		while(BitIndex <= 39)
		{
			Bit = serial[BitIndex];
			Bit = Bit << Shift;
			IuupPacket[ByteIndex] = IuupPacket[ByteIndex] + Bit;
			Shift = Shift + 1;
			if (Shift == 8)
			{
				Shift = 0;
				ByteIndex = ByteIndex + 1;
				IuupPacket[ByteIndex] = 0;
			}
			BitIndex = BitIndex + 1;
		}
		Result = 0;
		break;
	case TX_NO_DATA:
		Result = -1;
		break;
	default:
		Result = -1;
		break;
	}
	return Result;
}