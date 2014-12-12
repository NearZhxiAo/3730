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


	//ASSHM_MAGIC_DESTROY(p);
	ASSHM_DESTROY(p);
	return 0;
}
