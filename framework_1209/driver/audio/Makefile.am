EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = audio_test audio_test_enc audio_test_enc_bk

OTHERLIBRARIES= libaudio.a

DRIVERNAME=audio
include ../Makefile.driver

libaudio_a_SOURCES = libaudio.c   g711_sun_internal.c

audio_test_SOURCES = audio_test.c
audio_test_LDADD = -laudio

audio_test_enc_SOURCES = audio_test_enc.c
audio_test_enc_LDADD = -laudio


audio_test_enc_bk_SOURCES = audio_test_enc_bk.c
audio_test_enc_bk_LDADD = -laudio
