INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = flash_manager

flash_manager_SOURCES = flash_main.c flash_unix.c	

flash_manager_LDADD = -lunix -lgbshm -lsignal
