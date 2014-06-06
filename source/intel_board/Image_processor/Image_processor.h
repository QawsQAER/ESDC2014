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

extern char *dir_path;
extern char *PATH_TEMP;

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

	char* current_img_path;
	//this variable indicates where the frame for analysis comes from
	//it's either equal to IMG_SOURCE_CELLPHONE, or IMG_SOURCE_WEBCAM
	uint8_t img_source;

	//cap is useful when img_source == IMG_SOURCE_WEBCAM
	cv::VideoCapture *cap;


//-----------------body DETECTION RELATED VARIABLE--------------------------------------------
	//body_detect stores the rectangle of the body detected in the current_img

	//the HOG for body detection
	cv::HOGDescriptor hog;

//------------------FACE DETECTION RELATED VARIABLE------------------------------------------
	//face_detect stores the rectangle of the face detected in the current_img

	std::vector<cv::Rect> eyes_detect;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	//indicates the path to the Cascade 
	std::string face_cascade_name;
	std::string eyes_cascade_name;
//-------------------------------------------------------------------------------------------------------------------



	//this function will get image from cell phone, and load the image into the current_img
	uint8_t get_image_from_cellphone();
	//this function will get image from camera, and load the image into the current_img
	uint8_t get_image_from_webcam();

	//this function will run analysis on the current_img
	

public:	
	Image_processor(uint8_t img_source);
	~Image_processor();

	//cam is useful when img_source == IMG_SOURCE_CELLPHONE
	Camera *cam;
	
	//these variables stores the detection results
	std::vector<cv::Rect> body_detect;
	std::vector<cv::Rect> face_detect;
	std::vector<cv::Rect> final_body_detect;
	std::vector<cv::Rect> final_face_detect;
	//these variables stores the current image that is being processed
	cv::Mat current_img;
	cv::Mat analyzed_img;

	//these variables stores the expected image parameters
	
	float exp_w,exp_h; //expected width and height
	float exp_x,exp_y; //expected x and y
	cv::Point point;
	cv::Rect expected_region; //

	int32_t diff_x;
	int32_t diff_y;
	
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
	//this function will concat the input two images horizontally
	cv::Mat concat_image(const cv::Mat &img1, const cv::Mat &img2,uint8_t dir);
	uint8_t analyze_image();
	
	cv::Mat edge_detection(const cv::Mat &img);
	/*
	
		body detection related functions
	
	*/
	uint8_t basic_pedestrain_detection();
	uint8_t run_body_detection(const cv::Mat &source_img,std::vector<cv::Rect> &body_detect);
	cv::Mat mark_detected_body(const cv::Mat &source_img,const std::vector<cv::Rect> &body_detect);

	/*
	
		face detection related functions
	
	*/
	uint8_t basic_face_detection();
	uint8_t run_face_detection(const cv::Mat &source_img,std::vector<cv::Rect> &face_detect);
	cv::Mat mark_detected_face(const cv::Mat &source_img,const std::vector<cv::Rect> &face_detect);

	/*
	
		filtering related functions
	
	*/
	//this function will do basic filtering, eliminate body detected result without face detection
	uint8_t basic_filter();

	uint8_t find_body_according_to_face(const cv::Mat &source_img,
		const std::vector<cv::Rect> &face_detect);
	uint8_t find_body_in_roi(const cv::Mat &source_img,const cv::Rect roi,std::vector<cv::Rect> &body_detect);
	uint8_t face_body_related(const cv::Rect &body,const cv::Rect &face);
	/**/
	uint8_t show_analyzed_img();
	uint8_t get_image_and_show();
	IMAGE_PROCESS_STATE get_state();
	void test();

	/*
	
		Interface for intel board to use

	*/
	//this function will take a picture of the current scope,
	//and analyze whether there is a person/target in the picture.
	uint8_t target_in_scope();
	cv::Rect get_detection_result();
};

#endif
