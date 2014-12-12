INCLUDES=-I$(top_srcdir)/include -I./
AM_LDFLAGS = -L$(top_srcdir)/lib 
bin_PROGRAMS = core_process

core_process_SOURCES = core_main.c  core_init.c process_capture.c process_unix.c  
core_process_LDADD = -lunix -lgbshm -lcmem -ljpeg_encode 
