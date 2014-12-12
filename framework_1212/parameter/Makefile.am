INCLUDES=-I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/lib

noinst_LIBRARIES   = libautogen.a
#sbin_PROGRAMS = para_test
#para_test_SOURCES = para_test.c
#BUILT_SOURCES = $(NOR_BUILT_CON)
#CLEANFILES = $(NOR_BUILT_CON)

libautogen_a_SOURCES = parameter.h parametertype.hin parameter_offset.h intercmd.h intercmdtype.hin
libautogen_a_DEPENDENCIES = parameter.h parametertype.hin parameter_offset.h parameter_offset.default intercmd.h intercmdtype.hin
CLEANFILES = parameter.h parametertype.hin parameter_offset.h parameter_internal.ddf parameter_offset.default intercmd.h intercmd_inter.ddf intercmdtype.hin

parameter_internal.ddf:parameter.def parameter_internal.tpl
	    autogen  -T parameter_internal.tpl -b parameter_internal -o tmp parameter.def
		autogen  -T parameter_internal.tpl -b parameter_internal -o ddf parameter.def
	    rm -f parameter_internal.tmp
parameter.h:parameter_internal.ddf parameter.tpl
	    autogen  -T parameter.tpl -b parameter -o h parameter_internal.ddf
parametertype.hin:parameter_internal.ddf parametertype.tpl
	    autogen  -T parametertype.tpl -b parametertype -o hin parameter_internal.ddf
parameter_offset.h:parameter_internal.ddf parameter_offset.tpl
	    autogen  -T parameter_offset.tpl -b parameter_offset -o h parameter_internal.ddf
parameter_offset.default:parameter_internal.ddf parameter_offset.tpl
	    autogen  -T parameter_offset.tpl -b parameter_offset -o default parameter_internal.ddf

intercmd_inter.ddf:intercmd.def intercmd_inter.tpl
	    autogen  -T intercmd_inter.tpl -b intercmd_inter -o tmp intercmd.def
	    autogen  -T intercmd_inter.tpl -b intercmd_inter -o ddf intercmd.def
	    rm intercmd_inter.tmp -f
intercmd.h:intercmd_inter.ddf intercmd.tpl
	    autogen  -T intercmd.tpl -b intercmd -o h intercmd_inter.ddf
 
intercmdtype.hin:intercmd_inter.ddf intercmdtype.tpl
	    autogen  -T intercmdtype.tpl -b intercmdtype -o hin intercmd_inter.ddf
   
