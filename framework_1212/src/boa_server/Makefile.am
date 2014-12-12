INCLUDES=-I$(top_srcdir)/include 

AM_LDFLAGS = -L$(top_srcdir)/lib

bin_PROGRAMS = boaserver

boaserver_SOURCES = alias.c config.c get.c pipe.c tree.c xparse.c appro_api.c buffer.c description.c hash.c ip.c psia_root.c response.c util.c auth.c cgi.c iface.c log.c mmap_cache.c queue.c select.c sublog.c sysinfo.c web_translate.c boa.c cgi_header.c file_list.c index.c net_config.c read.c signals.c timestamp.c xadd.c request.c escape.c y.tab.c lex.yy.c boa_unix.c boa_main.c 

boaserver_LDADD = -lunix -lgbshm -lsignal
