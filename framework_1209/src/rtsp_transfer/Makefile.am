INCLUDES=-I$(top_srcdir)/include -I./
AM_LDFLAGS = -L$(top_srcdir)/lib -DSOCKET_SEND

bin_PROGRAMS = rtsp_transfer

rtsp_transfer_SOURCES = rtsp_transfer_main.c rtsp_transfer_unix.c rtpinit.c
#rtsp_transfer_SOURCES = rtp.c rtpinit.c 
	
rtsp_transfer_LDADD = -lunix -lgbshm -lsk -lvsshm -lasshm -laudio -lsignal
