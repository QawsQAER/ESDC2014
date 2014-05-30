/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
**********************************************************/



#ifndef _CAMERA_H
#define _CAMERA_H

#include <string>
#include <iostream>
#include <cstdio>
<<<<<<< HEAD

#define PATH_TEMP "/home/terry/Desktop/esdc2014/source/intel_board/camera/Photos/"
#define PATH_CAPTURE "../../Photos/Capture"
=======
#include "../macro.h"
>>>>>>> 683093f1d90c7d619c5a28d85843ed3bf3ec8e36

#define IP "192.168.43.1"
#define PORT "8080"
#define IP_PORT "192.168.43.1:8080"

class Camera
{
public:
	Camera();
	~Camera();

	void setip(std::string ip_address);

	std::string photo();
	std::string photo_af();
	std::string photo_JPEG();

	std::string take_photo_af();

	void zoom(float scaler);// (scaler>1 in) (scaler< 1 out)

	void flash_open();
	void flash_close();

	void af_open();
	void af_close();

	int test_connection();


private:
	FILE *fp;

	std::string ip;

	std::string path_temp;
	

	std::string path_capture;

	int count_temp_photo;
	int count_capture_photo;

	
};

#endif
/*********************************************************/
