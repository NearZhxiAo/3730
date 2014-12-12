INCLUDES=-I$(top_srcdir)/include -I/opt/dvsdk/linux-devkit/arm-none-linux-gnueabi/usr/include
AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = client_pc

client_pc_SOURCES = client_pc_main.c client_pc_poll.c client_pc_deal.c client_pc_socket.c client_pc_unix.c
client_pc_LDADD = -lunix -lgbshm -lssl -lsk -lsignal
