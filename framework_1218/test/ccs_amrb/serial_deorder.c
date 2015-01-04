#include "mode.h"
#include "frame.h"
#include "amr_global_var.h"
#include "table.h"

void serial_deorder(short order_serial[],short serial[])
{
	int					k;
	enum TXFrameType	FrameType;

	FrameType = (enum TXFrameType)order_serial[0];
	
	switch(FrameType)
	{
	case TX_SPEECH_GOOD:
		switch(AmrCfgMode)
		{
		case MR122:
			for (k = 1; k <= 244; k++)
			{
				serial[MR122_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR102:
			for (k = 1; k <= 204; k++)
			{
				serial[MR102_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR795:
			for (k = 1; k <= 159; k++)
			{
				serial[MR795_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR74:
			for (k = 1; k <= 148; k++)
			{
				serial[MR74_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR67:
			for (k = 1; k <= 134; k++)
			{
				serial[MR67_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR59:
			for (k = 1; k <= 118; k++)
			{
				serial[MR59_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR515:
			for (k = 1; k <= 103; k++)
			{
				serial[MR515_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		case MR475:
			for (k = 1; k <= 95; k++)
			{
				serial[MR475_order_table[k-1] + 1] = order_serial[k];
			}
			break;
		default:
			break;
		}
		break;
		
	case TX_SID_FIRST:
		for (k = 1; k <= 34; k++)
		{
			serial[k] = order_serial[k];
		}
		serial[35] = 0;
		serial[36] = AmrCfgMode & 0x01;
		serial[37] = (AmrCfgMode >> 1) & 0x01;
		serial[38] = (AmrCfgMode >> 2) & 0x01;

	case TX_SID_UPDATE:
		for (k = 1; k <= 34; k++)
		{
			serial[k] = order_serial[k];
		}
		serial[35] = 1;
		serial[36] = AmrCfgMode & 0x01;
		serial[37] = (AmrCfgMode >> 1) & 0x01;
		serial[38] = (AmrCfgMode >> 2) & 0x01;
		break;

	case TX_NO_DATA:

		break;

	default:
		break;
	}
	serial[0] = order_serial[0];
	serial[1+MAX_SERIAL_SIZE] = order_serial[1+MAX_SERIAL_SIZE];

	return;
}