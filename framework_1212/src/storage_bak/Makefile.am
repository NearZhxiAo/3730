INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib -D_STORAGE

bin_PROGRAMS = storage

storage_SOURCES = storage_main.c storage_unix.c storage_index.c storage_file.c storage_index_mng.c storage_data_mng.c

storage_LDADD = -lunix -lgbshm -lvsshm -lsk -lasshm -lsignal


