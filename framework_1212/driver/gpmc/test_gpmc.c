#include "system_includes.h"
#include "system_debug.h"

#include "libgpmc.h"


int main(int argc, char **argv)
{
		LIBGPMC_OBJ *pgpmc_obj ;

		pgpmc_obj = GPMC_INIT();

		GPMC_DESTROY(pgpmc_obj);
		return 0;
}
