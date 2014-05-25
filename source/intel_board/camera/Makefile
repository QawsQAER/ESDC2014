MV=mv
all:
	$(CPP) -o camera.o -c camera.cpp
	$(CPP) -o test_camera.o -c main.cpp
	$(CPP) -o test_camera test_camera.o camera.o
	$(MV) camera.o ../
