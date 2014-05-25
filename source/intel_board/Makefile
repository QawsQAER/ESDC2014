INTEL_BOARD_LD = -l Image_processor/Image_processor.o
INTEL_BOARD_DEPEND = intel_board.cpp intel_board.h Image_processor.o camera.o Makefile
INTEL_BOARD_DEPEND_OBJECT = Image_processor.o camera.o

export RM=rm
export CP=cp
export MV=mv
export CPPFLAGS = -lstdc++
all: intel_board.o

intel_board.o: $(INTEL_BOARD_DEPEND)
	echo "intel_board.o: Compiling"
	$(CC) -o intel_board.o -c intel_board.cpp $(CPPFLAGS)
	echo "intel_board.o: removing dependent object code"
	
Image_processor.o: Image_processor/Image_processor.cpp Image_processor/Image_processor.h Image_processor/Makefile
	echo "Image_processor.o: Compiling"
	cd Image_processor;make all
	
camera.o: camera/camera.cpp camera/camera.h camera/Makefile
	echo "camera.o: Compiling"
	cd camera;make all

clean:
	rm *.o
