INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = misc_manager

misc_manager_SOURCES = misc_main.c misc_unix.c misc_gps.c

misc_manager_LDADD = -lunix -lgbshm -lsignal -lgpio -lgps
