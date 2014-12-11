#include "serial_manage_private.h"
#define BUFFER_SIZE 4096


int serial_init()
{

		return succeed_type_succeed;
}

succeed_type serial_write()
{


		return succeed_type_succeed;
}

succeed_type serial_read(char *buf)
{


		return succeed_type_succeed;
}

void data_parse(SERIAL_MANAGE_DATA_TYPE *data_type, char *buff)
{
		*data_type = DATA_GPS;
}

succeed_type data_infrared_deal(char *buff)
{
		UNIX_CMD_STRUCT cmd;
		serial_to_avenc_input_infrared data;

		//process buff ---> data

		cmd.cmd = PARACMD_SERIAL_MANAGE_SERIAL_TO_AVENC_INPUT_INFRARED;
		cmd.cmdlen = sizeof(data);
		memcpy(&cmd.cmdinfo, &data, sizeof(data));
		UNIX_SEND(phandle->unix_fd, &cmd);

		return succeed_type_succeed;
}


void serial_main_func(SERIAL_MANAGE_PRIVATE_HANDLE *phandle)
{

	char buff[BUFFER_SIZE];
	SERIAL_MANAGE_DATA_TYPE data_type;


//	while(1)
	{
			if(serial_read(buff) != succeed_type_succeed)
			{
					SYS_ERROR("serial_read failed!\r\n");
					break;
			}

			data_parse(&data_type, buff);

			switch (data_type)
			{

					case DATA_INFRARED:
							{
									data_infrared_deal(buff);
									break;
							}
					case DATA_MEASURE:
							{

									break;
							}

					case DATA_GPS:
							{

									break;
							}

					case DATA_MAGNET:
							{

									break;
							}

					case DATA_POWER_MANAGE:
							{

									break;
							}

					case DATA_OLED1_CTRL:
							{

									break;
							}

					case DATA_OLED2_CTRL:
							{

									break;
							}
					default:
							SYS_ERROR("data type error..\r\n");

			}


	}	

}
