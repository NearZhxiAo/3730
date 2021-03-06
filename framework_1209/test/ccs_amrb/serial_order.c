#include "mode.h"
#include "frame.h"
#include "amr_global_var.h"
#include "table.h"
void serial_order(short serial[],short order_serial[])
{
	int					k;
	enum TXFrameType	FrameType;


	FrameType = (enum TXFrameType)serial[0];
	
	switch(FrameType)
	{
	case TX_SPEECH_GOOD:
		switch(AmrCfgMode)
		{
		case MR122:
			for (k = 1; k <= 244; k++)
			{
				order_serial[k] = serial[MR122_order_table[k-1] + 1];
			}
			break;
		case MR102:
			for (k = 1; k <= 204; k++)
			{
				order_serial[k] = serial[MR102_order_table[k-1] + 1];
			}
			break;
		case MR795:
			for (k = 1; k <= 159; k++)
			{
				order_serial[k] = serial[MR795_order_table[k-1] + 1];
			}
			break;
		case MR74:
			for (k = 1; k <= 148; k++)
			{
				order_serial[k] = serial[MR74_order_table[k-1] + 1];
			}
			break;
		case MR67:
			for (k = 1; k <= 134; k++)
			{
				order_serial[k] = serial[MR67_order_table[k-1] + 1];
			}
			break;
		case MR59:
			for (k = 1; k <= 118; k++)
			{
				order_serial[k] = serial[MR59_order_table[k-1] + 1];
			}
			break;
		case MR515:
			for (k = 1; k <= 103; k++)
			{
				order_serial[k] = serial[MR515_order_table[k-1] + 1];
			}
			break;
		case MR475:
			for (k = 1; k <= 95; k++)
			{
				order_serial[k] = serial[MR475_order_table[k-1] + 1];
			}
			break;
		default:
			break;
		}
		break;
		
	case TX_SID_FIRST:
		for (k = 1; k <= 34; k++)
		{
			order_serial[k] = serial[k];
		}
		order_serial[35] = 0;
		order_serial[36] = AmrCfgMode & 0x01;
		order_serial[37] = (AmrCfgMode >> 1) & 0x01;
		order_serial[38] = (AmrCfgMode >> 2) & 0x01;
		break;

	case TX_SID_UPDATE:
		for (k = 1; k <= 34; k++)
		{
			order_serial[k] = serial[k];
		}
		order_serial[35] = 1;
		order_serial[36] = AmrCfgMode & 0x01;
		order_serial[37] = (AmrCfgMode >> 1) & 0x01;
		order_serial[38] = (AmrCfgMode >> 2) & 0x01;
		break;

	case TX_NO_DATA:

		break;

	default:
		break;
	}
	order_serial[0] = serial[0];
	order_serial[1+MAX_SERIAL_SIZE] = serial[1+MAX_SERIAL_SIZE];

	return;
}