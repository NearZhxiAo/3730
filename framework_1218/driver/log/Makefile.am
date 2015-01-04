EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = log_test

OTHERLIBRARIES= liblog.a
DRIVERNAME=log

include ../Makefile.driver
liblog_a_SOURCES = liblog.c

log_test_SOURCES = log_test.c
log_test_LDADD = -llog
