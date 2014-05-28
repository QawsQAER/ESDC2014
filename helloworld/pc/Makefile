CC=gcc
LDFLAG=`pkg-config --libs --cflags opencv`  -lstdc++

TARGETS=hello hello_videocapture hello_objectdetect videocapture object_detect pedestrain_detect

all: $(TARGETS)

hello: helloworld.cpp
	$(CC) -o hello helloworld.cpp $(LDFLAG)

hello_videocapture: helloworld_videocapture.cpp Makefile
	$(CC) -o hello_videocapture helloworld_videocapture.cpp  $(LDFLAG)

hello_objectdetect: hello_objectdetect.cpp Makefile
	$(CC) -o hello_objectdetect hello_objectdetect.cpp $(LDFLAG)

videocapture: videocapture.cpp Makefile
	$(CC) -o videocapture videocapture.cpp $(LDFLAG)

object_detect: object_detect.cpp Makefile
	$(CC) -o object_detect object_detect.cpp $(LDFLAG)
pedestrain_detect: pedestrain.cpp Makefile
	$(CC) -o pedestrain_detect pedestrain.cpp $(LDFLAG)

highgui: highgui.cpp Makefile
	$(CC) -o highgui highgui.cpp $(LDFLAG)
