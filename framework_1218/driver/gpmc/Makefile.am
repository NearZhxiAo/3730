EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = test_gpmc

test_gpmc_SOURCES = test_gpmc.c

test_gpmc_LDADD = -lgpmc

OTHERLIBRARIES= libgpmc.a

DRIVERNAME=gpmc
include ../Makefile.driver

libgpmc_a_SOURCES = libgpmc.c

