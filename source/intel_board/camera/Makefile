#$(CC) $(CPP) are inherited from the Elipse generated Makefile
#$(CPPFLAGS) are defined in intel_board/Makefile
#$(MV) are defined in intel_board/Makefile

MACHINE_CODE=test_camera
OBJECT_CODE=*.o

all:
	$(CC) -o camera.o -c camera.cpp $(CPPFLAGS)
	$(CC) -o test_camera.o -c main.cpp $(CPPFLAGS)
	$(CC) -o test_camera test_camera.o camera.o $(CPPFLAGS)
	$(MV) camera.o ../
clean:
	rm $(MACHINE_CODE)
	rm $(OBJECT_CODE)
	
