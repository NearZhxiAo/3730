#include "system_includes.h"
#include "system_debug.h"
#include "parameter_offset.h"
#include "libvsshm.h"

int main(int argc, char **argv)
{
	int iLoopI = 0;
	int iLoopP = 0;
	unsigned int vsshm_seq = 0;
	int vsshm_sem = 0;
	char vsshm_data_i[VSSHM_UINT_LEN_I];
	char vsshm_data_p[VSSHM_UINT_LEN_P];
	LIBVSSHM_UINT_DATA_I struct_data_i;
	LIBVSSHM_UINT_DATA_P struct_data_p;
	memset(&vsshm_data_i, 0x00, sizeof(vsshm_data_i));
	memset(&vsshm_data_p, 0x00, sizeof(vsshm_data_p));
	LIBVSSHM_OBJ *p = VSSHM_INIT();
	if(NULL == p)
	{
		SYS_ERROR("VSSHM_INIT failed\r\n");
	}
	SYS_INFO("------------------------------I Frame \r\n");

	for(iLoopI = 0; iLoopI <= 20; iLoopI++)
	{
		VSSHM_GET_SEQ(p,1,&vsshm_seq,&vsshm_sem);
		SYS_INFO("vsshm_seq = 0x%x vsshm_sem = 0x%x\r\n\n",vsshm_seq,vsshm_sem);
		memset(&vsshm_data_i, iLoopI, sizeof(vsshm_data_i));
		VSSHM_PUT(p, vsshm_seq,1, (void*)&vsshm_data_i, sizeof(vsshm_data_i) - 1);
		VSSHM_GET_DATA(p, vsshm_seq, (void*)&struct_data_i);
		VSSHM_GET_COMPLETE(p, vsshm_seq);
		VSSHM_GET_COMPLETE(p, vsshm_seq);
		SYS_INFO("struct_data_i.vsshm_header.data_len=0x%x struct_data_i.vsshm_context[0]=0x%x\r\n"\
				,struct_data_i.vsshm_header.data_len,struct_data_i.vsshm_context[0]);
		SYS_INFO("struct_data_i.vsshm_context[struct_data_i.vsshm_header.data_len - 1]=0x%x\r\n",\
				struct_data_i.vsshm_context[struct_data_i.vsshm_header.data_len - 1]);
		SYS_INFO("struct_data_i.vsshm_context[struct_data_i.vsshm_header.data_len]=0x%x\r\n",\
				struct_data_i.vsshm_context[struct_data_i.vsshm_header.data_len]);
	}
	SYS_INFO("------------------------------P Frame \r\n");

	for(iLoopP = 0; iLoopP <= 60; iLoopP++)
	{
		VSSHM_GET_SEQ(p,0,&vsshm_seq,&vsshm_sem);
		//VSSHM_GET_SEQ(p,0,&vsshm_seq,NULL);
		printf("vsshm_seq = 0x%x vsshm_sem = 0x%x\r\n\n",vsshm_seq,vsshm_sem);
		memset(&vsshm_data_p, iLoopP, sizeof(vsshm_data_p));
		VSSHM_PUT(p, vsshm_seq,1, (void*)&vsshm_data_p, sizeof(vsshm_data_p) - 1);
		VSSHM_GET_DATA(p, vsshm_seq, (void*)&struct_data_p);
		VSSHM_GET_COMPLETE(p, vsshm_seq);
		VSSHM_GET_COMPLETE(p, vsshm_seq);
		SYS_INFO("struct_data_p.vsshm_header.data_len=0x%x struct_data_p.vsshm_context[0]=0x%x\r\n"\
				,struct_data_p.vsshm_header.data_len,struct_data_p.vsshm_context[0]);
		SYS_INFO("struct_data_p.vsshm_context[struct_data_p.vsshm_header.data_len - 1]=0x%x\r\n",\
				struct_data_p.vsshm_context[struct_data_p.vsshm_header.data_len - 1]);
		SYS_INFO("struct_data_p.vsshm_context[struct_data_p.vsshm_header.data_len]=0x%x\r\n",\
				struct_data_p.vsshm_context[struct_data_p.vsshm_header.data_len]);
	}

	VSSHM_DESTROY(p);
	return 0;
}
