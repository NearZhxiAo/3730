EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = vsshm_test vsshm_destroy

OTHERLIBRARIES= libvsshm.a
DRIVERNAME = vsshm

include ../Makefile.driver
libvsshm_a_SOURCES = libvsshm.c

vsshm_test_SOURCES = vsshm_test.c
vsshm_test_LDADD = -lvsshm

vsshm_destroy_SOURCES = vsshm_destroy.c
vsshm_destroy_LDADD = -lvsshm
