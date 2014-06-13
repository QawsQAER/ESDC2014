#ifndef _MACRO_H
#define _MACRO_H

#include "stdint.h"

#define MAX_COUNT	4			// max. no. of points being tracked
#define PixelWidth	0.0005		// in mm
#define MMtoP		1/PixelWidth	// no. of pixel per mm, each pixel is 5 um, so 1 mm has 200 pixel
#define GUESS		3000*MMtoP		// initial value of l[0], l[1], l[2], l[3]
#define ERROR		0.1			// lower bound of the errors' value
#define Focal		460		// focal length of webcam
#define MaxIter		50			// upper bound of number of iteration of the algorithm
#define A(i, j)		A.at<double>(i, j)
#define a(i, j)		a.at<double>(i, j)


#define WINNAME_LENGTH 32
#define IMG_SOURCE_CELLPHONE 0
#define IMG_SOURCE_WEBCAM 1

#define FILENAME_LENGTH 128

#define PATH_TO_FACE_CASCADE "lbpcascade_frontalface.xml"
#define PATH_TO_EYES_CASCADE "haarcascade_eye_tree_eyeglasses.xml"

#define IP "192.168.43.1"
#define PORT "8080"
#define IP_PORT "192.168.43.1:8080"

extern char *PATH_TEMP;
extern unsigned char continuity;

#define PATH_CAPTURE "../../Photos/Capture"

enum command_type{
	undefined,
	connect_request,
	start_movement,
	confirm_picture,
	pattern_1,
	pattern_2,
	pattern_3,
	pattern_4,
	car_forward,
	car_backward,
	car_left,
	car_right,
	camera_forward,
	camera_backward,
	camera_left,
	camera_right,
	lift_up,
	lift_down};

#define AUTOFOCUS

#define PIXEL_HEIGHT 127/480 //127mm for a 480 pixel height
#define PIXEL_WIDTH PIXEL_HEIGHT //consider PIXEL as a square
#define FOCAL_LENGTH 43 //43 mm for 


#define E970_F_NUMBER 2.4
#define E970_SENSOR_SIZE 1/3.2
#define E970_SENSOR_DIAGONAL 5.68
#define E970_SENSOR_WIDTH 4.54
#define E970_SENSOR_HEIGHT 3.42
 


#define F_NUMBER 2.4
#define SENSOR_DIAGONAL 5.68
#define SENSOR_WIDTH 4.54
#define SENSOR_HEIGHT 3.42



//IMAGE PROCESSING THRESHOLD
#define IMG_ORI_WIDTH 640
#define IMG_ORI_HEIGHT 480

#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define IMG_CENTER_X IMG_WIDTH/2
#define IMG_CENTER_Y IMG_HEIGHT/2
#define IMG_HORI_THRESHOLD 50
#define IMG_VERT_THRESHOLD 25


#define IMG_EXP_HEIGHT 420
#define IMG_EXP_WIDTH 70

#define IMG_EXP_DIS 4887.5

#define GOLDEN_RATIO 0.618

#define IMG_EXP_POS1_X ((uint16_t) IMG_WIDTH * GOLDEN_RATIO)
#define IMG_EXP_POS1_Y IMG_HEIGHT - ((uint16_t) (IMG_HEIGHT * (GOLDEN_RATIO + 0.15)))

#define IMG_EXP_POS2_X IMG_WIDTH - ((uint16_t) IMG_WIDTH * GOLDEN_RATIO)
#define IMG_EXP_POS2_Y IMG_HEIGHT - ((uint16_t) (IMG_HEIGHT * (GOLDEN_RATIO + 0.15)))

#define IMG_EXP_POS3_X IMG_CENTER_X
#define IMG_EXP_POS3_Y IMG_HEIGHT - ((uint16_t) (IMG_HEIGHT * (GOLDEN_RATIO )))

#define IMG_EXP_POS4_X IMG_WIDTH - 20
#define IMG_EXP_POS4_Y IMG_EXP_POS3_Y

#define DEFAULT_DIS 400
#define DEFAULT_DIS_LARGE 500
#define DEFAULT_DIS_SMALL 300



//
//
//
#define LIFTER_UP 0
#define LIFTER_DOWN 1
#define LIFTER_MIN 0 //the minimum height of the lifter in mm
#define LIFTER_MAX 700 //the maximun height of the lifter in mm
#define LIFTER_INIT_POS 250

#endif
