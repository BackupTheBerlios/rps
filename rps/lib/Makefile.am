ASD_DIR=/usr/src/asd4-0.2/

AM_CXXFLAGS = -I${ASD_DIR}asdlib/ \
              -I${ASD_DIR}common/ \
               @SIGC_CFLAGS@ \
              `glib-config --cflags glib gthread`

lib_LIBRARIES=libRPGS.a
libRPGS_a_SOURCES = soundfile.cc \
                    FileList.cc \
                    PlayList.cc \
                    rps.cc \
                    asd_sound.cc \
                    itos.cc

###########################
#AM_CXXFLAGS=" -O0 -g "
#LIBS = -L. -lRPGS 
LIBS = `glib-config --libs glib gthread` \
       ${ASD_DIR}asdlib/ASDLIB.o \
       ${ASD_DIR}common/COMMON.o \
       -L. -lRPGS

#bin_PROGRAMS = rpgs_test
noinst_PROGRAMS = rpgs_test

#AM_CXXFLAGS = @GTKMM_CFLAGS@
rpgs_test_LDADD = @LIBS@ @SIGC_LIBS@ 
#ende

rpgs_test_SOURCES = rpgs_test.cc



#no_include_HEADERS = rpgs.hh

CLEANFILES= *~
