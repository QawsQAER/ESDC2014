#ifndef _MACRO_H
#define _MACRO_h

#define IMG_SOURCE_CELLPHONE 0
#define IMG_SOURCE_WEBCAM 1

#define FILENAME_LENGTH 128

#define PATH_TO_FACE_CASCADE "lbpcascade_frontalface.xml"
#define PATH_TO_EYES_CASCADE "haarcascade_eye_tree_eyeglasses.xml"

#define IP "192.168.43.1"
#define PORT "8080"
#define IP_PORT "192.168.43.1:8080"

#define PATH_TEMP "capture_image/"
#define PATH_CAPTURE "../../Photos/Capture"

enum command_type{undefined,connect_request,start_movement,confirm_picture,pattern_1,pattern_2,pattern_3,pattern_4,car_forward,car_backward,car_left,car_right,camera_forward,camera_backward,camera_left,camera_right,lift_up,lift_down};

#endif
