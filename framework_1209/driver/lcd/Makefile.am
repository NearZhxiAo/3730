EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS =

OTHERLIBRARIES= liblcd.a

DRIVERNAME=lcd
include ../Makefile.driver

liblcd_a_SOURCES = liblcd.c

#gbshm_test_SOURCES = gbshm_test.c
#gbshm_test_LDADD = -lgbshm
