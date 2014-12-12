INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = serial_manage

serial_manage_SOURCES = serial_manage_main.c serial_manage_unix.c
serial_manage_LDADD = -lunix -lgbshm  -lsk -lsignal
