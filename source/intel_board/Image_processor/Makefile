#$(CC),$(MV) are inherited from the parent Makefile
all: Image_processor.o camshiftdemo.o fourPointAlgo.o

Image_processor.o:
	$(CC) -o Image_processor.o -c Image_processor.cpp $(CPPFLAGS)
	echo "Image_processor.o: moving to parent directory intel_board"
	$(MV) Image_processor.o ../

camshiftdemo.o:
	$(CC) -o  camshiftdemo.o -c camshiftdemo.cpp $(CPPFLAGS)
	$(MV) camshiftdemo.o ../

fourPointAlgo.o:
	$(CC) -o fourPointAlgo.o -c fourPointAlgo.cpp $(CPPFLAGS)
	$(MV) fourPointAlgo.o ../

clean:
	rm *.o

