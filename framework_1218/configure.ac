#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.1])
AC_INIT(POLICE_work, 1.0, liangzhang@xidian.edu.cn)
AM_INIT_AUTOMAKE(POLICE_work, 1.0, liangzhang@xidian.edu.cn)
#AC_CONFIG_SRCDIR([src/testprog/main.cpp])
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([pthread], [main])

AC_HEADER_STDBOOL

AC_C_INLINE

AC_TYPE_SIZE_T

# Checks for header files.
AC_CHECK_HEADERS()

AC_PROG_RANLIB
# Checks for library functions.

AC_OUTPUT([Makefile
	parameter/Makefile
	include/Makefile
	include/parameters/Makefile
	include/intercmds/Makefile
	lib/Makefile
	test/Makefile
	driver/Makefile
	driver/gbshm/Makefile
	driver/asshm/Makefile
	driver/vsshm/Makefile
	driver/gpmc/Makefile
	src/Makefile
	src/av_encoder/Makefile
	src/core_process/Makefile
	src/storage/Makefile
	src/client_pc/Makefile
	src/misc_manager/Makefile
	src/flash_manager/Makefile
	src/serial_manage/Makefile
    ])
#src/Makefile
#	src/boa_server/Makefile
#	src/flash_manager/Makefile
#	src/ptz/Makefile

