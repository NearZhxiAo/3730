INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = alert_manager

alert_manager_SOURCES = alert_main.c alert_unix.c alert_gpio.c alert_ad.c

alert_manager_LDADD = -lad -lgbshm -lunix -lgpio
