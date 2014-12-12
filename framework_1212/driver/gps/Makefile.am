EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = gps_test

OTHERLIBRARIES= libgps.a

DRIVERNAME=gps
include ../Makefile.driver

libgps_a_SOURCES = libgps.c

gps_test_SOURCES = gps_test.c
gps_test_LDADD = -lgps
