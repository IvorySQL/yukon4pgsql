# geomodel extension makefile.in 
GEOMODEL_MAJRO_VERSION=1
GEOMODEL_MINOR_VERSION=0
POSTGIS_PGSQL_VERSION=92

EXTENSION = yukon_geomodel        # the extensions name
MODULE_big=yukon_geomodel-$(GEOMODEL_MAJRO_VERSION).$(GEOMODEL_MINOR_VERSION)
DATA = yukon_geomodel--1.0.sql  # script files to install

GEOMODEL_OBJS= \
    geomodel.o  \
    gmfuncs.o   \
    gmserialized_typmod.o \
    geomodel_util.o \

OBJS=$(GEOMODEL_OBJS)

uname_m = $(shell uname -m)
LD_FLAGS.x86_64 = ../libUGC/Builds/MakeProject/Bin64_Linux
LD_FLAGS.aarch64 = ../libUGC/Builds/MakeProject/Binaarch64_Linux
LD_FLAGS += $(LD_FLAGS.$(uname_m))

override CFLAGS :=-I../../liblwgeom -I../../include  -fpermissive $(IS_DEBUG) -D_UGUNICODE -D_UGQSTRING
PG_CPPFLAGS += -fPIC -I../libUGC/Include -I../../raster/rt_core/ -I../../raster/rt_pg $(CFLAGS)  -I/usr/local/include   -I/usr/include/libxml2  -I/usr/include/json-c   -DNDEBUG   -w 
#librtcore.a 的位置要放在 liblwgeom.a 的前面（librtcore.a 依赖于 liblwgeom.a），否则会造成有些函数无法链接到动态库
SHLIB_LINK = ../../raster/rt_core/librtcore.a ../../liblwgeom/.libs/liblwgeom.a  -L$(LD_FLAGS) -lYkGeometry3D  -L/usr/local/lib -lgeos_c -lproj -ljson-c  -lprotobuf-c  -lxml2 -lz -llzma -licui18n -licuuc -licudata -lm  -Wl,--exclude-libs,ALL  -lm   
# postgres build stuff
PG_CONFIG := /mnt/hdb1/pg12/bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)



distclean: clean
	rm -f Makefile
