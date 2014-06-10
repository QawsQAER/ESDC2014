#ifndef _MACRO_H
#define _MACRO_H

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

enum command_type{undefined,connect_request,start_movement,confirm_picture,pattern_1,pattern_2,pattern_3,pattern_4,car_forward,car_backward,car_left,car_right,camera_forward,camera_backward,camera_left,camera_right,lift_up,lift_down};

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



#endif
