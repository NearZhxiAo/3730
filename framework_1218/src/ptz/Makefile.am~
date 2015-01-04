INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = ptz

ptz_SOURCES = ptz_main.c ptz_unix.c
ptz_LDADD = -lunix -lgbshm  -lsk -lsignal
