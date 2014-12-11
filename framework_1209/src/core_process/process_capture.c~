#include "core_private.h"

void process_capture(CORE_PROCESS_PRIVATE_HANDLE *phandle)
{
    int current_state;
    int jpeg_encode_length;
	UNIX_CMD_STRUCT cmd;
    CaptureBuffer capture_buffer;

    current_state = get_core_state(phandle);

    //if (succeed_type_failed == Capture_get(phandle->capture_handle, &capture_buffer)) 
    if(0)
    {
        SYS_ERROR("Capture_get error.\r\n");
        return;
    }

    switch (current_state) 
    {
        case IDLE_STATE:
            //call algorithm 
            //memcpy(phandle->jpeg_inbuffer, capture_buffer.virtaddr, YUV_BUFFER_SIZE);
            //Jpeg_encode(phandle->jpeg_inbuffer, phandle->jpeg_outbuffer, &jpeg_encode_length);

            cmd.cmd = PARACMD_CORE_PTZ_START_PRESET; //PARACMD_CORE_PTZ_START_SETPRESET;
			Unix_send(phandle->unix_fd, &cmd);

            set_core_state(phandle, ALERT_STATE);
            break;
        case ALERT_STATE:
            //continue capture
            break;
        case GRASP_STATE:
            //memcpy(phandle->jpeg_inbuffer, capture_buffer.virtaddr, YUV_BUFFER_SIZE);
            //Jpeg_encode(phandle->jpeg_inbuffer, phandle->jpeg_outbuffer, &jpeg_encode_length);

            cmd.cmd = PARACMD_CORE_PTZ_START_PRESET; //PARACMD_CORE_PTZ_START_SETPRESET;
			Unix_send(phandle->unix_fd, &cmd);

            set_core_state(phandle, WAIT_STATE);
            break;
        case WAIT_STATE:
            break;
        default:
            SYS_ERROR("current core is a error state.\r\n");
            SYS_ERROR("resume to idle state.\r\n");
            sleep(5);
            set_core_state(phandle, IDLE_STATE);
            break;
    }
}
