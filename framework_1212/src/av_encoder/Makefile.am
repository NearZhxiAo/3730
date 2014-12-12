INCLUDES=-I$(top_srcdir)/include -I/opt/sdb/linux-2.6.32-CNG205/include -I/opt/sdb/linux-2.6.32-CNG205/arch/arm/include

AM_LDFLAGS = -L$(top_srcdir)/lib 
bin_PROGRAMS = av_encoder

av_encoder_SOURCES = av_encoder_init.c av_encoder_unix.c av_encoder_vid_enc_run.c av_encoder_main.c av_encoder_video.c

LIBS = -lpthread 

av_encoder_LDADD = -lunix -lgbshm -lvsshm -lasshm  -lsignal  -lcapture -lcmem -ldisplay -lresize -lH264Encode -lJpegEncode
