EXTRA_DIST =
SUBDIRS =

INCLUDES = -I$(top_srcdir)/include 
AM_LDFLAGS = -L$(top_srcdir)/lib -L.

sbin_PROGRAMS = gpio_test 3g_reset usb_test gpio_gps usb_set  gpio_in_test gpio_start led_test gpio_test_5150rst WDQ_5150

OTHERLIBRARIES= libgpio.a

DRIVERNAME=gpio
include ../Makefile.driver

libgpio_a_SOURCES = libgpio.c

led_test_SOURCES = led_test.c
led_test_LDADD = -lgpio

gpio_start_SOURCES = gpio_start.c
gpio_start_LDADD = -lgpio

gpio_in_test_SOURCES = gpio_in_test.c
gpio_in_test_LDADD = -lgpio

usb_set_SOURCES = usb_set.c
usb_set_LDADD = -lgpio

gpio_test_SOURCES = gpio_test.c
gpio_test_LDADD = -lgpio


3g_reset_SOURCES = 3g_reset.c
3g_reset_LDADD = -lgpio

usb_test_SOURCES = usb_test.c
usb_test_LDADD = -lgpio

gpio_gps_SOURCES = gpio_gps.c
gpio_gps_LDADD = -lgpio

gpio_test_5150rst_SOURCES = gpio_test_5150rst.c
gpio_test_5150rst_LDADD = -lgpio

WDQ_5150_SOURCES = WDQ_5150.c
WDQ_5150_LDADD = -lgpio
