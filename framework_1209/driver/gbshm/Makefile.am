EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = gbshm_test_write gbshm_test_read gbshm_test gbshm_producer gbshm_consumer gbshm_read

OTHERLIBRARIES= libgbshm.a

DRIVERNAME=gbshm
include ../Makefile.driver

libgbshm_a_SOURCES = libgbshm.c

gbshm_test_write_SOURCES = gbshm_test_write.c
gbshm_test_write_LDADD = -lgbshm

gbshm_test_read_SOURCES = gbshm_test_read.c
gbshm_test_read_LDADD = -lgbshm

gbshm_test_SOURCES = gbshm_test.c
gbshm_test_LDADD = -lgbshm

gbshm_producer_SOURCES = gbshm_producer.c
gbshm_producer_LDADD = -lgbshm

gbshm_consumer_SOURCES = gbshm_consumer.c
gbshm_consumer_LDADD = -lgbshm

gbshm_read_SOURCES = gbshm_read.c
gbshm_read_LDADD = -lgbshm

