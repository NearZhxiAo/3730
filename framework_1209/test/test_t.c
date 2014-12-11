#include "system_includes.h"
#include "system_debug.h"

succeed_type sub_p(void *p)
{
    if (NULL == p)
    {
	SYS_ERROR("p is NULL.\r\n");
	return succeed_type_failed;
    }
    return succeed_type_succeed;
}

int main(int argc, char **argv)
{
    sub_p(NULL);
    SYS_INFO("test_t program.\r\n");
    return 0;
}
