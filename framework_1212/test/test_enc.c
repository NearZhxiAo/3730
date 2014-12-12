#include "system_includes.h"
#include "system_debug.h"
#include "libunix.h"
#include "libvsshm.h"
#include <sys/time.h>

static int error_num = 0;

long long  get_ustime(void)
{
        static char firsttimehere=1;
        static struct timeval timeorigin;
        struct timeval now;
                                                                                                                           
        // the first time, set the origin.
        if (firsttimehere) {
                gettimeofday(&timeorigin,NULL);
                firsttimehere=0;
        }
        gettimeofday(&now,NULL);                                                                                                 
        return  (long long)(                                                                                               
                            ( (long long)now.tv_sec - (long long)timeorigin.tv_sec )*1000000LL +                           
                            ( (long long)now.tv_usec - (long long)timeorigin.tv_usec )                                     
                           );                                                                                                            
}                                                                                                                          
                                                                                                                           
int main(int argc, char **argv)
{
	int sockfd = 0;
	int sem_value = -1;
	int iLoop0 = 0;
	int iLoop = 0;
	int timea , timeb;
	UNIX_CMD_STRUCT cmd;
	buffer_new_msg bufnewmsg;

	LIBVSSHM_OBJ  *pLibVsshmObj = NULL;
	LIBVSSHM_UINT_DATA_I stLibVsshmUint;
	LIBVSSHM_UINT_DATA_I *pstLibVsshmUint = &stLibVsshmUint;


	memset(&cmd, 0x00, sizeof(cmd));
	memset(&bufnewmsg, 0x00, sizeof(bufnewmsg));

	memset(pstLibVsshmUint, 0x00, sizeof(LIBVSSHM_UINT_DATA_I));

	pLibVsshmObj = VSSHM_INIT();
	/*创建Unix socket 发送消息到其他模块*/
	if(UNIX_CREATE(UNIXFILE_SOCKET_TEST, &sockfd, 0) == succeed_type_failed)
	{
		SYS_ERROR("UNIX_CREATE %s failed.\r\n", UNIXFILE_SOCKET_TEST);
		return succeed_type_failed;
	}

	while(1)
	{
		timea = get_ustime();
		for(iLoop0 =0; iLoop0 <1000; iLoop0 ++)
		{
			printf("iLoop = %d ...\r\n\r\n", iLoop0);
			iLoop %= 10;
			int seq_num;
#if 1
			if(VSSHM_GET_SEQ(pLibVsshmObj, (iLoop==0), &seq_num, &sem_value)==succeed_type_failed)
			{
				SYS_ERROR("the function VSSHM_GET() failed.\r\n");
				return succeed_type_failed;
			}
			/*获得信号量*/
			if(sem_value != 0)
			{
				error_num++;	
			}
			memset(&(pstLibVsshmUint->vsshm_context), iLoop, VSSHM_UINT_LEN_I);

			/*调用PUT函数*/
			if(VSSHM_PUT(pLibVsshmObj, seq_num, FRAME_TYPE_VIDEO_H264, \
						(void *)&(pstLibVsshmUint->vsshm_context),\
						(iLoop==0)?(VSSHM_UINT_LEN_I-1):(VSSHM_UINT_LEN_P-1))\
					== succeed_type_failed)
			{
				SYS_ERROR("the function VSSHM_PUT() failed.\r\n");
				return succeed_type_failed;
			}
			
#endif
			bufnewmsg.buffer_id = seq_num;
			cmd.cmd = PARACMD_AVENC_BUFFER_NEW_MSG;
			cmd.cmdlen = sizeof(bufnewmsg);
			memcpy(&cmd.cmdinfo,&bufnewmsg,sizeof(bufnewmsg));
			//SYS_INFO("\r\n-----------Send The bufnewmsg[%d] -----------\r\n",iLoop);
			UNIX_SEND(sockfd, &cmd);
			usleep(100000);
			//usleep(35000);
		}
		timeb = get_ustime();
		SYS_INFO("\n\r---------VSSHM_PUT 25f cost the time = %ld error_num = %d-----------\r\n",\
				timeb - timea,error_num);
		break;
		//sleep(1);
	}
	UNIX_DESTROY(UNIXFILE_SOCKET_TEST, sockfd);
	return 0;

}
