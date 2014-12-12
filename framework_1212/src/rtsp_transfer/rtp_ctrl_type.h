#ifndef _RTP_CTRL_TYPE
#define _RTP_CTRL_TYPE

#ifdef _cplusplus
extern "C" {
#endif
/*rtp包的控制命令类型：*/
#define RTP_CTRL_UNKNOWN					    (0x0000) /*未知命令*/
#define RTP_CTRL_CHANNEL_ACTIVE					(0x0001) /*激活通道和维持通道的活动状态*/
#define RTP_CTRL_CHANNEL_CLOSE					(0x0002) /*通知对方数据传输结束，关闭通道*/
#define RTP_CTRL_INCREASE_RATE					(0x0003) /*已废弃*/
#define RTP_CTRL_DECREASE_RATE				 	(0x0004) /*已废弃*/
#define RTP_CTRL_DATA_PAUSE						(0x0005) /*请求暂停该通道上的数据发送，无应答包*/
#define RTP_CTRL_DATA_CONTINUE					(0x0006) /*请求继续该通道上的数据发送，无应答包*/
#define RTP_CTRL_NET_TEST						(0x0007) /*测试网络是否可达*/
#define RTP_CTRL_NET_TEST_ACK					(0x8007) /*应答以告诉对对方网络可达*/
#define RTP_CTRL_PLAYBACK						(0x0008) /*回放控制命令*/

#define RTP_CTRL_RECV_REPORT					(0x0101) /*接收方向发送方报告接收丢包率*/
#define RTP_CTRL_REQ_RESEND						(0x0102) /*重发请求命令*/
#define RTP_CTRL_REQ_KEYFRAME					(0x0103) /*接收方向发送方请求I帧信令*/


#ifdef _cplusplus
}
#endif
#endif
