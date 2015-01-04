EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

#sbin_PROGRAMS = gbshm_test

OTHERLIBRARIES= libvidctl.a

DRIVERNAME=vidctl
include ../Makefile.driver

libvidctl_a_SOURCES = libvidctl.c

#gbshm_test_SOURCES = gbshm_test.c
#gbshm_test_LDADD = -lgbshm
