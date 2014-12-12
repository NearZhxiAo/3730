#include "alert_private.h"
ALERT_PRIVATE_HANDLE global_alert_private_handle;
static int Di_checked_count = 0; //di事件检测计数
static int Key_checked_count = 0; //di事件检测计数

static void alert_poll_init(ALERT_PRIVATE_HANDLE *phandle)
{
	memset(phandle->polltimeout, 0x00, sizeof(struct pollfd) * ALERT_POLL_COUNT_MAX );
	phandle->polltimeout[0].fd = phandle->unix_fd;
	phandle->polltimeout[0].events = POLLIN;

	phandle->polltimeout[1].fd = phandle->pad_obj->ad_fd;
	phandle->polltimeout[1].events = POLLIN;

	phandle->polltimeout[2].fd = phandle->pgpio_obj->gpio_fd;
	phandle->polltimeout[2].events = POLLIN;

	//phandle->active_fdcount = 3;
	phandle->active_fdcount = 1;

}
static succeed_type alert_init(ALERT_PRIVATE_HANDLE *phandle)
{
	if ( NULL == phandle )
	{
		SYS_ERROR("in alert_init phandle is NULL.\r\n");
		return succeed_type_failed;
	}
	//1. Init
	phandle->pad_obj = AD_INIT();
	if ( NULL == phandle->pad_obj )
	{
		SYS_ERROR("AD_INIT failed.\r\n");
		return succeed_type_failed;
	}
	phandle->pgbshm_obj = GBSHM_INIT();
	if ( NULL == phandle->pgbshm_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}
	phandle->pgpio_obj = GPIO_INIT();
	if ( NULL == phandle->pgpio_obj )
	{
		SYS_ERROR("GBSHM_INIT failed.\r\n");
		return succeed_type_failed;
	}

	//2. 创建本进程绑定的UNIX文件，返回值为unix_fd
	if ( UNIX_CREATE(UNIXFILE_SOCKET_ALERT, &(phandle->unix_fd), 0 ) == succeed_type_failed )
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_ALERT);
		return succeed_type_failed;
	}

	alert_poll_init(phandle);
	return succeed_type_succeed;
}
#define TEST_IP "www.baidu.com"
void alert_3g_test_func(void* arg)
{
	ALERT_PRIVATE_HANDLE *phandle = (ALERT_PRIVATE_HANDLE*)arg;
	char cmd[128] = "";
	sprintf(cmd,"ping -c 1 %s >/dev/null",TEST_IP);
	int iping_ret = -1;
	while(1)
	{
		sleep(16);
		iping_ret = system(cmd);
		if( 0 == iping_ret) // 0x000-0x3FF
		{
			//GPIO_SETOUTPUT(phandle->pgpio_obj, GPIO_OUTPUT_3G_LED_GREEN);
			if (phandle->G_led != 1)
			{
				GPIO_3G_GREEN_ON(phandle->pgpio_obj);
			}
			phandle->G_led = 1;
			SYS_WARNING("3g normal!!\r\n");
		}
		else
		{
			//添加测试代码
		//	system("pppd call ppp0 > /dev/null &");
		//	sleep(30);
		//	GPIO_CLROUTPUT(phandle->pgpio_obj, GPIO_OUTPUT_3G_LED_RED);
			if (phandle->G_led != 0)
			{
				GPIO_3G_GREEN_OFF(phandle->pgpio_obj);
			}
			phandle->G_led = 0;
			SYS_WARNING("3g unnormal!!\r\n");
		}
	}
}

void alert_di_test_func(void* arg)
{
	ALERT_PRIVATE_HANDLE *phandle = (ALERT_PRIVATE_HANDLE*)arg;
	UNIX_CMD_STRUCT cmd;
	di_event event_value;
	int in_data = -1;
	int in_keydata = -1;

	cmd.cmd	= PARACMD_ALERT_DI_EVENT;
	cmd.cmdlen = sizeof(di_event);
	event_value.reserved= 0xFF;
	memcpy(cmd.cmdinfo, &event_value,sizeof(event_value));

	while(1)
	{
		usleep(100000);
		//GPIO_GETINPUT(phandle->pgpio_obj, GPIO_INPUT_DSP_TRIG,&in_data);
		GPIO_GET_DSP_TRIG(phandle->pgpio_obj, &in_data);
		GPIO_GET_DSP_KEY_DECT(phandle->pgpio_obj, &in_keydata);

		if(in_data == 0)
		{
			Di_checked_count++;
		}
		else
		{
			Di_checked_count = 0;
		}

		if(in_keydata == 1)
		{
			Key_checked_count++;
		}
		else
		{
			Key_checked_count = 0;
		}

		//对di进行检测
		if(Key_checked_count == 5)
		{
			SYS_WARNING("shutdown system\r\n");
			sleep(1);
			GPIO_CLROUTPUT(phandle->pgpio_obj,GPIO_OUTPUT_DSP_PWR_EN);
		}

		//对di进行检测
		if(Di_checked_count == 5)
		{
			//di点绿灯
			//GPIO_SETOUTPUT(phandle->pgpio_obj,GPIO_OUTPUT_STATE_LED_GREEN);
//			if (phandle->state_led != 1)
			{
				//GPIO_STATE_GREEN(phandle->pgpio_obj);
				GPIO_PWR_GREEN(phandle->pgpio_obj);
			}
			phandle->state_led = 1;
			UNIX_SEND(phandle->unix_fd,&cmd);
			SYS_WARNING("Send DI signo!!!\r\n");
			Di_checked_count = 0;
			sleep(5);
			//GPIO_STATE_GREEN_OFF(phandle->pgpio_obj);
			GPIO_PWR_GREEN_OFF(phandle->pgpio_obj);
		}
	}
}
void alert_pwr_test_func(void* arg)
{
	ALERT_PRIVATE_HANDLE *phandle = (ALERT_PRIVATE_HANDLE*)arg;
	int ad_value = -1;
	while(1)
	{
		sleep(2);
		AD_READ(phandle->pad_obj, &ad_value);
		if( 0x260 >= ad_value) // 0x000-0x3FF
		{
			//GPIO_SETOUTPUT(phandle->pgpio_obj, GPIO_OUTPUT_PWR_LED_RED);
	//		if (phandle->power_led != 2)
			{
				//GPIO_PWR_RED(phandle->pgpio_obj);
				GPIO_STATE_RED(phandle->pgpio_obj);
			}
			phandle->power_led = 2;

			SYS_WARNING("Power Low!! Please Charge!!\r\n");
		}
		else if(0x300 <= ad_value)
		{
	//		if (phandle->power_led != 1)
			{
				//GPIO_PWR_GREEN(phandle->pgpio_obj);
				GPIO_STATE_GREEN(phandle->pgpio_obj);
			}
			phandle->power_led = 1;

			SYS_WARNING("Fully Charging!! Please unplug the power to save energy!!\r\n");
		}
	}
}

int main(int argc, char **argv)
{
	pthread_t alert_eventsend_pthread_0;
	pthread_t alert_eventsend_pthread_1;
	pthread_t alert_eventsend_pthread_2;
	memset(&global_alert_private_handle, 0x00, sizeof(global_alert_private_handle));
	alert_init(&global_alert_private_handle);	
	SYS_INFO("Enter alert_manager func.\r\n");
#if 0
	if(pthread_create(&alert_eventsend_pthread_0, NULL, alert_3g_test_func, &global_alert_private_handle) != 0)
	{
		SYS_ERROR(" alert pthread_create0 failed!");
	}
#endif
	if(pthread_create(&alert_eventsend_pthread_1, NULL, alert_di_test_func, &global_alert_private_handle) != 0)
	{
		SYS_ERROR(" alert pthread_create1 failed!");
	}
#if 1
	if(pthread_create(&alert_eventsend_pthread_2, NULL, alert_pwr_test_func, &global_alert_private_handle) != 0)
	{
		SYS_ERROR(" alert pthread_create1 failed!");
	}
#endif
	while(1)
	{
		if ( poll( global_alert_private_handle.polltimeout, global_alert_private_handle.active_fdcount , -1) > 0 )
		{
			SYS_INFO("POLL event found.\r\n");
			if ( global_alert_private_handle.polltimeout[0].revents )
			{
				//do UNIX recv event.
				alert_unix(&global_alert_private_handle);
			}
			if ( global_alert_private_handle.polltimeout[1].revents )
			{
				alert_ad(&global_alert_private_handle);
			}
			if ( global_alert_private_handle.polltimeout[2].revents )
			{
				alert_gpio(&global_alert_private_handle);
			}
			alert_poll_init(&global_alert_private_handle);
		}
	}

	pthread_join(alert_eventsend_pthread_0, NULL);
	pthread_join(alert_eventsend_pthread_1, NULL);
	pthread_join(alert_eventsend_pthread_2, NULL);
	return 0;
}

