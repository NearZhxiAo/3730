#INCLUDES=-I$(top_srcdir)/include -I$(top_srcdir)/../../opt/dvsdk/psp/linux-2.6.32-psp03.00.01.06.sdk/include -I$(top_srcdir)/../../opt/dvsdk/psp/linux-2.6.32-psp03.00.01.06.sdk/arch/arm/include
INCLUDES=-I$(top_srcdir)/include -I/opt/sdb/linux-2.6.32-CNG205/include -I/opt/sdb/linux-2.6.32-CNG205/arch/arm/include

AM_LDFLAGS = -L$(top_srcdir)/lib

noinst_LIBRARIES = libosd.a libdisplay.a libv4l2_cap.a libcapture.a libresize.a libunix.a libsignal.a libsk.a #libresize.a#  libresz.a libenc.a libi2c.a libcpuinfo.a  libptz.a libcapture.a 

libosd_SOURCES = libosd.c


libv4l2_cap_SOURCES = libv4l2_cap.c

libunix_SOURCES = libunix.c

libsk_SOURCES = libsk.c

libcpuinfo_SOURCES = libcpuinfo.c

libresz_SOURCES = libresz.c
libresz_LIBADD = libdrv.a

libi2c_SOURCES = libi2c.c
libi2c_LIBADD = libdrv.a

libenc_SOURCES = libenc.c
libenc_LIBADD = libalg.a

libptz_SOURCES = libptz.c
libptz_LIBADD = -lgps

libcapture_SOURCES = libcapture.c

libdisplay_SOURCES = libdisplay.c

libresize_SOURCES = libresize.c

libsignal_SOURCES = libsignal.c

#NOR_BUILT_CON = libad.a libgbshm.a libaudio.a libgpio.a liblog.a libvsshm.a libvidctl.a libasshm.a libgps.a liblcd.a
NOR_BUILT_CON = libgbshm.a libvsshm.a libgpmc.a
BUILT_SOURCES = $(NOR_BUILT_CON)
CLEANFILES = $(NOR_BUILT_CON)
libgbshm.a:
	./link.sh driver/gbshm $@
libvsshm.a:
	./link.sh driver/vsshm $@
libgpmc.a:
	./link.sh driver/gpmc $@

