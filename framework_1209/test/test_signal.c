#include "system_includes.h"
#include "system_debug.h"
#include "signal.h"

#define SIG_TIME 10

void sig_handle(int sig)
{
	printf("sig = %d\r\n",sig);
}

int main( int argc, char** argv)
{
	succeed_type iret = succeed_type_failed;
	iret = SIGNAL_INIT(SIGINT,sig_handle);
	if(succeed_type_succeed != iret)
	{
		printf("SIGNAL_INIT failed!!\r\n");
	}
	while(1)
	{
		printf("wait for SIGINT for %ds\r\n",SIG_TIME);
		sleep(SIG_TIME);
	}
	return 0;
}
