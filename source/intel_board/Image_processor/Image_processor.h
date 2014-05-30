/*

*/

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <stdint.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "../macro.h"
#include <vector>
#include <time.h>
#include "../camera/camera.h"
#include <string>

#define WINNAME_LENGTH 32

#ifndef IMG_SOURCE_CELLPHONE
#define IMG_SOURCE_CELLPHONE 0
#endif

#ifndef IMG_SOURCE_WEBCAM
#define IMG_SOURCE_WEBCAM 1
#endif

enum IMAGE_PROCESS_STATE
{
	IMAGE_PROCESS_INIT
};
class Image_processor
{
private:
	//the name of the displayed window
	char winname[WINNAME_LENGTH];
	//the state of the Image_processor
	IMAGE_PROCESS_STATE state;
	//the boolean variable which indicates whether there exist a window
	bool win_exist;

	//this variable indicates where the frame for analysis comes from
	//it's either equal to IMG_SOURCE_CELLPHONE, or IMG_SOURCE_WEBCAM
	uint8_t img_source;

	//cap is useful when img_source == IMG_SOURCE_WEBCAM
	cv::VideoCapture *cap;
	//cam is useful when img_source == IMG_SOURCE_CELLPHONE
	Camera *cam;




//-----------------body DETECTION RELATED VARIABLE--------------------------------------------
	//body_detect stores the rectangle of the body detected in the current_img
	std::vector<cv::Rect> body_detect;
	//the HOG for body detection
	cv::HOGDescriptor hog;

//------------------FACE DETECTION RELATED VARIABLE------------------------------------------
	//face_detect stores the rectangle of the face detected in the current_img
	std::vector<cv::Rect> face_detect;
	std::vector<cv::Rect> eyes_detect;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	//indicates the path to the Cascade 
	std::string face_cascade_name;
	std::string eyes_cascade_name;
//-------------------------------------------------------------------------------------------------------------------

	std::vector<cv::Rect> final_detect;

	//this function will get image from cell phone, and load the image into the current_img
	uint8_t get_image_from_cellphone();
	//this function will get image from camera, and load the image into the current_img
	uint8_t get_image_from_webcam();

	//this function will run analysis on the current_img
	

public:	
	Image_processor(uint8_t img_source);
	~Image_processor();

	//this variable stores the current image that is being processed
	cv::Mat current_img;
	cv::Mat analyzed_img;
	//load the Cascade Classifier for face detection, and the HOG SVM for body detection
	uint8_t init();

	//this function will get an image from either cam or cap, and store the image into the current_img
	uint8_t capture_image();
	uint8_t load_current_img_to_analyzed_img();
	//this function will stored the image into the hard disk,
	//naming it according to the time since Epoch
	uint8_t save_current_image();
	//this function simplily read the image from file to current_img
	uint8_t read_image(const char* filename);
	uint8_t analyze_image();
	
	/*this function will use current_img as image source and gives out analyzed result*/
	uint8_t basic_pedestrain_detection();
	uint8_t run_body_detection();
	cv::Mat mark_detected_body(cv::Mat source_img);
	/*this function will use current_img as image source and gives out analyzed result in analyzed img and face_detect*/
	uint8_t basic_face_detection();
	uint8_t run_face_detection();
	cv::Mat mark_detected_face(cv::Mat source_img);

	/**/
	//this function will do basic filtering, eliminate body detected result without face detection 
	uint8_t basic_filter();

	uint8_t face_body_related(const cv::Rect &body,const cv::Rect &face);
	/**/
	uint8_t show_analyzed_img();
	uint8_t get_image_and_show();
	IMAGE_PROCESS_STATE get_state();
	void test();
};

#endif
