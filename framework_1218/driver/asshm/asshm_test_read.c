#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libasshm.h"

int main(int argc, char **argv)
{
	LIBASSHM_OBJ *p = ASSHM_INIT();
	if(NULL == p)
	{
		SYS_INFO("ASSHM_INIT\r\n");
	}
	char buf[1024];
	int ptr_sem;
	int i;

	memset(buf, 0x00, 1024);

	if(succeed_type_failed == ASSHM_GET_SEM(p, 0, &ptr_sem))
	{
		SYS_ERROR("ASSHM_GET_SEM failed!\r\n");
		exit(0);
	}

	if(succeed_type_failed == ASSHM_GET_DATA(p, 0, buf))
	{
		SYS_ERROR("ASSHM_GET_DATAA failed!\r\n");
		exit(0);
	}

	for(i = 0; i < 1024; i++)
	{
		printf("0x%x ", buf[i]);
	}

	//ASSHM_MAGIC_DESTROY(p);
	ASSHM_DESTROY(p);
	return 0;
}
