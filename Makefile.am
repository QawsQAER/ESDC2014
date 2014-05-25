
ESDC_LDOBJECT = intel_board/intel_board.o intel_board/Image_processor.o #intel_board/camera.o
ESDC_LDLIB = -lopencv_highgui -lopencv_imgproc -lopencv_core
SUBDIRS = intel_board
bin_PROGRAMS = ESDC
ESDC_SOURCES = ESDC.cpp
ESDC_LDADD = $(ESDC_LDOBJECT) $(ESDC_LDLIB)
AM_CXXFLAGS = @ESDC_CFLAGS@
AM_LDFLAGS = @ESDC_LIBS@

CLEANFILES = *~
