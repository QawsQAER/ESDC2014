CC=gcc
CFLAGS=-lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_objdetect -lstdc++ -lm
ESDC_LDOBJECTS=intel_board/intel_board.o intel_board/Image_processor.o intel_board/camera.o intel_board/Motion_controller.o intel_board/Controller_Com.o intel_board/message.o intel_board/ui.o intel_board/camshiftdemo.o intel_board/fourPointAlgo.o

all: ESDC

ESDC: ESDC.o Makefile
	cd intel_board;make all
	$(CC) -o ESDC ESDC.o $(ESDC_LDOBJECTS) $(CFLAGS)

ESDC.o:
	$(CC) -o ESDC.o -c ESDC.cpp $(CFLAGS)

