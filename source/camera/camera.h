/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
**********************************************************/



#ifndef _CAMERA_H
#define _CAMERA_H

#include <string.h>
#include <stdio.h>

#define PATH_TEMP ../../Photos/Temp
#define PATH_CAPTURE ../../Photos/Capture

class Camera
{
public:
	Camera();
	~Camera();

	void setip(string ip_address);

	string photo();
	string photo_af();
	string photo_JPEG();

	string take_photo_af();

	void zoom(float scaler);// (scaler>1 in) (scaler< 1 out)

	void flash_open();
	void flash_close();

	void af_open();
	void af_close();
	

private:
	FILE *fp;

	string ip;

	string path_temp;
	static int count_temp_photo;

	string path_capture;
	static int count_capture_photo;
};

#endif
/*********************************************************/