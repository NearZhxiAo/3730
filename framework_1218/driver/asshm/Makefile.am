EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = #asshm_test_read asshm_test_write

OTHERLIBRARIES= libasshm.a
DRIVERNAME = asshm

include ../Makefile.driver
libasshm_a_SOURCES = libasshm.c

asshm_test_read_SOURCES = asshm_test_read.c
asshm_test_read_LDADD = -lasshm


asshm_test_write_SOURCES = asshm_test_write.c
asshm_test_write_LDADD = -lasshm


