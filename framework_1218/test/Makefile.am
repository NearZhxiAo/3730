#INCLUDES=-I$(top_srcdir)/include -I$(top_srcdir)/../../opt/dvsdk/psp/linux-2.6.32-psp03.00.01.06.sdk/include -I$(top_srcdir)/../../opt/../dvsdk/psp/linux-2.6.32-psp03.00.01.06.sdk/arch/arm/include
INCLUDES=-I$(top_srcdir)/include -I/opt/sdb/linux-2.6.32-CNG205/include -I/opt/sdb/linux-2.6.32-CNG205/arch/arm/include
AM_LDFLAGS = -L$(top_srcdir)/lib

sbin_PROGRAMS =  test_Loopback #test_h264#test_display    test_resize test_capture 

test_SOURCES= test.c
test_LDADD = -ltest

test_capture_SOURCES = test_capture.c 
test_capture_LDADD = -lcapture -lcmem -losd

test_display_SOURCES = test_display.c 
test_display_LDADD = -ldisplay  -lcmem

test_loopback_SOURCES = test_loopback.c 
test_loopback_LDADD = -ldisplay -lcmem -losd -lv4l2_cap

test_Loopback_SOURCES = test_Loopback.c 
test_Loopback_LDADD = -ldisplay -lcmem -losd -lcapture -lJpegEncode -lH264Encode -lresize -lpthread


test_h264_SOURCES = test_h264.c 
#test_h264_LDADD = -lH264Encode 

test_resize_SOURCES = test_resize.c 
test_resize_LDADD = -lcapture -ldisplay -lresize -lcmem

test_time_SOURCES = test_time.c

test_video_SOURCES = test_video.c

test_signal_SOURCES = test_signal.c 
test_signal_LDADD = -lsignal

test_sd_SOURCES = test_sd.c 

test_sd_space_SOURCES = test_sd_space.c 

test_SOURCES = test_t.c test_b.c
test_LDADD = 

test_cpu_SOURCES = test_cpu.c 
test_cpu_LDADD = -lcpuinfo

test_print_SOURCES = test_print.c

test_hello_SOURCES = test_hello.c
test_hello_LDADD = 

test_param_SOURCES = test_param.c
test_param_LDADD =

test_send_SOURCES = test_send.c
test_send_LDADD = -lunix

test_boa_SOURCES = test_boa.c
test_boa_LDADD = -lunix

test_avenc_SOURCES = test_avenc.c
test_avenc_LDADD = -lunix

test_rtsp_SOURCES = test_rtsp.c
test_rtsp_LDADD = -lunix

test_storage_SOURCES = test_storage.c
test_storage_LDADD = -lunix


test_hellow_SOURCES = test_helloworld.c
test_hellow_LDADD = 

test_clientpc_SOURCES = test_clientpc.c
test_clientpc_LDADD = -lunix -lgbshm

test_enc_SOURCES = test_enc.c
test_enc_LDADD = -lunix -lvsshm


test_audio_SOURCES = test_audio.c
test_audio_LDADD = -lunix


test_gbshm_SOURCES = test_gbshm.c
test_gbshm_LDADD = -lgbshm

test_sk_SOURCES = test_sk.c
test_sk_LDADD = -lsk

test_i2c_SOURCES = test_i2c.c
test_i2c_LDADD = -li2c -ldrv -losa

test_i2c_audio_SOURCES = test_i2c_audio.c
test_i2c_audio_LDADD = -li2c -ldrv -losa

test_ttyS_SOURCES = test_ttyS0.c

pppd_system_SOURCES = pppd_system.c
pppd_system_LDADD = -lgpio
