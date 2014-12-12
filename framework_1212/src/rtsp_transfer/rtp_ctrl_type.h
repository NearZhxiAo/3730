#ifndef _RTP_CTRL_TYPE
#define _RTP_CTRL_TYPE

#ifdef _cplusplus
extern "C" {
#endif
/*rtp���Ŀ����������ͣ�*/
#define RTP_CTRL_UNKNOWN					    (0x0000) /*δ֪����*/
#define RTP_CTRL_CHANNEL_ACTIVE					(0x0001) /*����ͨ����ά��ͨ���Ļ״̬*/
#define RTP_CTRL_CHANNEL_CLOSE					(0x0002) /*֪ͨ�Է����ݴ���������ر�ͨ��*/
#define RTP_CTRL_INCREASE_RATE					(0x0003) /*�ѷ���*/
#define RTP_CTRL_DECREASE_RATE				 	(0x0004) /*�ѷ���*/
#define RTP_CTRL_DATA_PAUSE						(0x0005) /*������ͣ��ͨ���ϵ����ݷ��ͣ���Ӧ���*/
#define RTP_CTRL_DATA_CONTINUE					(0x0006) /*���������ͨ���ϵ����ݷ��ͣ���Ӧ���*/
#define RTP_CTRL_NET_TEST						(0x0007) /*���������Ƿ�ɴ�*/
#define RTP_CTRL_NET_TEST_ACK					(0x8007) /*Ӧ���Ը��߶ԶԷ�����ɴ�*/
#define RTP_CTRL_PLAYBACK						(0x0008) /*�طſ�������*/

#define RTP_CTRL_RECV_REPORT					(0x0101) /*���շ����ͷ�������ն�����*/
#define RTP_CTRL_REQ_RESEND						(0x0102) /*�ط���������*/
#define RTP_CTRL_REQ_KEYFRAME					(0x0103) /*���շ����ͷ�����I֡����*/


#ifdef _cplusplus
}
#endif
#endif
