# generated 2003/9/22 11:37:15 CEST by thoma@Tiger.(none)
# using glademm V2.0.0b
#
# newer (non customized) versions of this file go to Makefile.am_new

LIBS = -L../lib/ -lRPGS

bin_PROGRAMS = rps

rps_SOURCES = \
	rpgs.cc\
	main_window_RPS_glade.cc \
	main_window_RPS.cc 

noinst_HEADERS = \
	main_window_RPS_glade.hh \
	main_window_RPS.hh 

AM_CXXFLAGS = @GTKMM_CFLAGS@ \
              -I${ASD_DIR}asdlib/ \
              -I${ASD_DIR}common/

rps_LDADD = @LIBS@ @GTKMM_LIBS@ 
