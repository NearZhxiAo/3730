#include <sys/time.h>
#include <time.h>


int main()
{
	struct timeval start,end;
	char *dis;
	char *cap;

	dis = malloc(10*1024*1024);
	cap = malloc(10*1024*1024);

	while(1)
	{
		gettimeofday(&start,NULL);
		memcpy(cap, dis, 10*1024*1024);
		gettimeofday(&end,NULL);
		printf("consume %dms\n", (end.tv_sec*1000 + end.tv_usec/1000) - (start.tv_sec*1000 + start.tv_usec/1000));
		//printf("start: sec%d, usec %d\n", start.tv_sec, start.tv_usec);
		//printf("end:   sec%d, usec %d\n", end.tv_sec, end.tv_usec);
	}


	return 0;
}
