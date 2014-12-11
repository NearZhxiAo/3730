EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = asshm_test

OTHERLIBRARIES= libasshm.a
DRIVERNAME = asshm

include ../Makefile.driver
libasshm_a_SOURCES = libasshm.c

asshm_test_SOURCES = asshm_test.c
asshm_test_LDADD = -lasshm
