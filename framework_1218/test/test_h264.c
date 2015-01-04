#include <stdio.h>
#include "system_debug.h"
#include "system_includes.h"



int main()
{
	char *in, *out;


	H264_INIT(in, out, 720, 480, 25,2000000 ,30 );
	return 0;
}
