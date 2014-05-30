CC=gcc
LDFLAG=`pkg-config --libs --cflags opencv`  -lstdc++
DEBUG_FLAG=-g
TARGETS=hello hello_videocapture hello_objectdetect videocapture object_detect 
TARGETS:=pedestrain_detect facerec_eigenfaces face_detect
TARGETS:=sub_image


all: $(TARGETS)

hello: helloworld.cpp Makefile
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

facerec_eigenfaces: facerec_eigenfaces.cpp Makefile
	$(CC) -o facerec_eigenface facerec_eigenfaces.cpp $(LDFLAG)

face_detect: face_detect.cpp Makefile
	$(CC) -o face_detect face_detect.cpp $(LDFLAG)
sub_image: sub_image.cpp Makefile
	$(CC) -o sub_image sub_image.cpp $(LDFLAG) $(DEBUG_FLAG)
