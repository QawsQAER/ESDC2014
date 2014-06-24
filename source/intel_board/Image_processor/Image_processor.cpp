/*
 * Copyright (C) Your copyright notice.
 *
 * Author: Edward HUANG, CUHK huangxx_2155@live.com
 *         Tony Yi, CUHK     
 *         Terry Lai, CUHK
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PG_ORGANIZATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY	THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS-IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Image_processor.h"

Image_processor::Image_processor(uint8_t img_source)
{

	printf("Constructing Image processor\n");
	this->state = IMAGE_PROCESS_INIT;
	strcpy(this->winname,"main window");
	strcpy(this->skinwin,"skin window");
	strcpy(this->edgewin,"edge window");
	this->win_exist = 0;
	this->img_source = img_source;
	if(this->img_source == IMG_SOURCE_WEBCAM)
	{
		printf("Image_processor: Using WEBCAM\n");
		this->cap = new cv::VideoCapture(CV_CAP_ANY);
		if(!cap->isOpened())
		{
			printf("Image_processor Error: Cannot open camera\n");
			exit(-1);
		}
		cap->set(CV_CAP_PROP_POS_AVI_RATIO,1);
	}
	else if(this->img_source == IMG_SOURCE_CELLPHONE)
	{
		printf("Image_processor: Using CELL PHONE\n");
		this->cam = new Camera();
		std::string ip(IP_PORT);
		this->cam->setip(ip);
	}
	//check the existence of the directory for storing the capture image
	this->current_img_path = (char *) malloc(sizeof(char) * FILENAME_LENGTH);
	memset(this->current_img_path,0,sizeof(char) * FILENAME_LENGTH);
}

Image_processor::~Image_processor()
{
	printf("Destructing Image processor\n");
	free(current_img_path);
	delete this->cam;
}

uint8_t Image_processor::init()
{
	printf("Image_processor init(): Setting the default body detector\n");
	//setting the SVM for body detection
	this->hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	//setting the path to the cascade classifier
	this->face_cascade_name = PATH_TO_FACE_CASCADE;
	this->eyes_cascade_name = PATH_TO_EYES_CASCADE;

	//setting the cascade classifier for face detection
	printf("Image_processor init(): setting the default face detector\n");
	if( !this->face_cascade.load(this->face_cascade_name) ){ printf("--(!)Error loading face cascade\n");std::cout<<this->face_cascade_name<<endl; exit(-1); };
	return 1;
}

IMAGE_PROCESS_STATE Image_processor::get_state()
{
	return this->state;
}

uint8_t Image_processor::get_image_from_cellphone()
{
	//TODO: maybe check the length of the c_str() return value
#ifdef AUTOFOCUS
	strcpy(this->current_img_path,this->cam->photo_af().c_str());
#else
	strcpy(this->current_img_path,this->cam->photo_frame().c_str());
#endif

	printf("Image_processor::get_image_from_cellphone: Reading from %s\n",this->current_img_path);
	this->current_img = cv::imread(this->current_img_path,CV_LOAD_IMAGE_COLOR);
	printf("Image_processor::get_image_from_cellphone: original size (%d,%d)\n",this->current_img.cols,this->current_img.rows);
	cv::resize(this->current_img,this->current_img,cv::Size(IMG_WIDTH,IMG_HEIGHT));
	printf("Image_processor::get_image_from_cellphone: current size (%d,%d)\n",this->current_img.cols,this->current_img.rows);
	if(!this->current_img.data)
	{
		printf("Image_processor::get_image_from_cellphone: No data is loaded from the cellphone\n");
		exit(-1);
	}

	return 1;
}

uint8_t Image_processor::get_image_from_webcam()
{
	for(uint8_t count_frame = 0;count_frame < 30;count_frame++)
	{
		if(!this->cap->grab())
		{
			printf("Image_processor::get_image_from_webcam(): Error, Cannot grab()\n");
			return 0;
		}
	}
	
	if(!this->cap->retrieve(this->current_img))
	{
		printf("Image_processor::get_image_from_webcam(): Error, Cannot retrieve()\n");
		return 0;
	}

	if( (this->current_img.rows == this->current_img.cols) && (this->current_img.cols == 0))
	{
		printf("Image_processor::get_image_from_webcam(): Error, get cols = rows = 0\n");
		return 0;
	}

	return 1;
}

/*
 * Implementation of capture_image()
 */
uint8_t Image_processor::capture_image()
{
	if(this->img_source == IMG_SOURCE_CELLPHONE)
	{
		return this->get_image_from_cellphone();
	}
	else if(this->img_source == IMG_SOURCE_WEBCAM)
	{
		return this->get_image_from_webcam();
	}
	return 1;
}

/*
 * Implementation of get_image_and_show()
 */

uint8_t Image_processor::get_image_and_show()
{
	//get image by path
	cv::Mat image;
	if(this->win_exist)
		cv::destroyWindow(this->winname);
	else
		this->win_exist = 1;
	cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	cv::imshow(this->winname,image);
	cv::waitKey(0);
	return 1;
}

/*
 *	Implementation of save_current_image()
 *
 */
uint8_t Image_processor::save_current_image(uint16_t task_counter)
{
	char *filename =(char *) malloc(sizeof(char) * FILENAME_LENGTH);
	char *analyzed_filename = (char*) malloc(sizeof(char) * FILENAME_LENGTH);
	char *analyzed_filtered_filename = (char*) malloc(sizeof(char) * FILENAME_LENGTH);

	time_t timestamp = time(NULL);
	struct tm *current_time = gmtime(&timestamp);
	//get the filename in format of month-day_hour:minute:second
	sprintf(filename,"%u",task_counter);
	strcpy(analyzed_filename,filename);
	strcat(analyzed_filename,"_ana.jpg");

	strcpy(analyzed_filtered_filename,filename);
	strcat(analyzed_filtered_filename,"_ana_fil.jpg");
	strcat(filename,".jpg");

	//specify the compression parameters
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//write the data into the file
	cv::imwrite(filename,this->current_img,compression_params);
	cv::imwrite(analyzed_filename,this->analyzed_img,compression_params);
	cv::imwrite(analyzed_filtered_filename,this->analyzed_img_filtered,compression_params);

	free(filename);
	free(analyzed_filename);
	free(analyzed_filtered_filename);
}

/*
 *
 */
cv::Mat Image_processor::concat_image(const cv::Mat &img1, const cv::Mat &img2,uint8_t dir = 0)
{
	printf("Image_processor::concat_image working\n");
	cv::Mat result(img1.rows,img1.cols + img2.cols,CV_8UC3);
	if(dir == 0)
	{
		cv::Mat left(result,cv::Rect(0,0,img1.cols,img1.rows));
		img1.copyTo(left);

		cv::Mat right(result,cv::Rect(img1.cols,0,img2.cols,img2.rows));
		img2.copyTo(right);
	}
	else
	{

	}
	printf("Image_processor::concat_image exiting\n");
	return result;
}

/*
 * Implementation of basic analysis
 */
uint8_t Image_processor::analyze_image()
{
	return 1;
}

/*
 * Basic edge detection implementation
 */
cv::Mat Image_processor::edge_detection(const cv::Mat &img)
{
	cv::Mat gray_img,result;
	cv::cvtColor(img,gray_img,CV_BGR2GRAY);
	uint16_t threadhold1 = 50;
	uint16_t threadhold2 = 150;
	int32_t apertureSize = 3;
	cv::Canny(gray_img,result,threadhold1,threadhold2,apertureSize);

	return result;
}

/*
 *
 * Implementation of basic pedestrain detection algorithm
 *
 */

 //TODO: understand this function
 uint8_t Image_processor::run_body_detection(const cv::Mat &source_img,std::vector<cv::Rect> &body_detect)
 {
 	printf("Image_processor::run_body_detection() running\n");
 	body_detect.clear();
 	std::vector<cv::Rect> found;
 	this->hog.detectMultiScale(source_img,found,0,cv::Size(8,8),cv::Size(32,32),1.05,2);
 	size_t count1, count2;
 	for(count1 = 0;count1 < found.size();count1++)
 	{
 		cv::Rect r = found[count1];
 		for(count2 = 0;count2 < found.size();count2++)
 		{
 			if(count2 != count1 && ((r & found[count2]) == r))
 				break;
 		}
 		if(count2 == found.size())
 		{
 			//move the top left bottom right a bit
 			r.x += cvRound(r.width * 0.1);
 			//shrink the width
 			r.width = cvRound(r.width * 0.8);
 			//move the top left bottom lower a bit 
 			r.y += cvRound(r.height * 0.06);
 			//shrink the height
 			r.height = cvRound(r.height * 0.9);
 			//push it into the storage of body detection result
 			body_detect.push_back(r);
 		}
 	}
 	printf("Image_processor::run_body_detection() exiting\n");
 	return 1;
 }


cv::Mat Image_processor::mark_detected_body(const cv::Mat &source_img, const std::vector<cv::Rect> &body_detect)
{
	//note that the body_detect is the parameter of the function, not the body_detect of the instance
	cv::Mat marked_img = source_img.clone();
	printf("mark_detected_body(): source_img (%d,%d)\n",source_img.cols,source_img.rows);
	//for every detected face
	for(size_t count = 0;count < body_detect.size();count++)
	{
		printf("mark_detected_body() [%lu] body: (%u,%u,%u,%u)\n",
			count + 1,
			body_detect[count].x,
			body_detect[count].y,
			body_detect[count].width,
			body_detect[count].height);
		cv::Rect r = body_detect[count];
		cv::rectangle(marked_img,r.tl(),r.br(),cv::Scalar(0,255,0),2);
	}
	printf("mark_detected_body(): exiting\n");
	return marked_img;
}

/*
 *
 * Implementation of basic face detection
 *
 */

uint8_t Image_processor::run_face_detection(const cv::Mat &source_img,std::vector<cv::Rect> &face_detect)
{
	printf("Image_processor::run_face_detection() running\n");
	face_detect.clear();
	
	//this->eyes_detect.clear();

	//use gray image for face detection
	cv::Mat frame_gray;
	cv::cvtColor(source_img, frame_gray,cv::COLOR_BGR2GRAY );
	cv::equalizeHist(frame_gray, frame_gray);
	//doing face detection

	//the minimum face is (IMG_FACE_WIDTH_MIN,IMG_FACE_HEIGHT_MIN)
	//the maximum face is (IMG_FACE_WIDTH_MAX,IMG_FACE_HEIGHT_MAX)
	//the scaleFactor is 1.1 as default
	//the flag is 0 as default, possible value is CV_HAAR_DO_CANNY_PRUNING
	//the minNeighbour is 4 for higher accruacy
	//flags NA in new cascade classifier
	this->face_cascade.detectMultiScale(frame_gray,
		face_detect,
		1.1, 4, 0,
		cv::Size(IMG_FACE_WIDTH_MIN,IMG_FACE_HEIGHT_MIN),
		cv::Size(IMG_FACE_WIDTH_MAX,IMG_FACE_HEIGHT_MAX));
	printf("Image_processor::run_face_detection: detect %lu faces\n",face_detect.size());

	//TODO: use color detection to filtered out the non-human color face
	std::vector<cv::Rect> tmp(face_detect);
	face_detect.clear();
	for(size_t count = 0;count < tmp.size();count++)
	{
		cv::Mat subImage = source_img(tmp[count]);
		if(getSkin(subImage,subImage)[0] > 120)
		{
			face_detect.push_back(tmp[count]);
		}
	}
	return 1;
}

cv::Scalar Image_processor::getSkin(const cv::Mat &source_img,cv::Mat &dest_img)
{
	int32_t Y_MIN = IMG_Y_MIN;
	int32_t Y_MAX = IMG_Y_MAX;
	int32_t Cr_MIN = IMG_Cr_MIN;
	int32_t Cr_MAX = IMG_Cr_MAX;
	int32_t Cb_MIN = IMG_Cb_MIN;
	int32_t Cb_MAX = IMG_Cb_MAX;
	cv::Mat result = source_img.clone();
	cv::cvtColor(source_img,result,cv::COLOR_BGR2YCrCb);
	cv::inRange(result,cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN),cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX),result);
	cv::Scalar value = mean(result);
	printf("Image_processor::getSkin average %lf\n",value[0]);
	dest_img = result;
	return value;
}

cv::Mat Image_processor::mark_detected_face(const cv::Mat &source_img,const std::vector<cv::Rect> &face_detect)
{
	cv::Mat marked_img = source_img.clone();
	printf("Image_processor::mark_detected_face working\n");
	//for evert detected face
	for(size_t count = 0;count < face_detect.size();count++)
	{
		printf("Image_processor::mark_detected_face (%d,%d,%d,%d)\n",
			face_detect[count].x,
			face_detect[count].y,
			face_detect[count].width,
			face_detect[count].height);
		//create a point noting the center of the region where a face is detected
		cv::Point center(face_detect[count].x + face_detect[count].width/2,
						face_detect[count].y + face_detect[count].height/2);
		//draw an ellipse for the face
		cv::ellipse(marked_img,center,
				cv::Size(face_detect[count].width/2,face_detect[count].height/2),
				0,0,360, cv::Scalar(255,0,0),2,8,0);
	}
	printf("Image_processor::mark_detected_face exiting\n");
	return marked_img;
}

/*
 *
 */
uint8_t Image_processor::show_analyzed_img()
{
	printf("\nImage_processor::show_analyzed_img() working\n");
	//cv::destroyWindow(this->winname);
	//cv::destroyWindow(this->skinwin);
	//cv::destroyWindow(this->edgewin);

	//cv::namedWindow(this->edgewin,CV_WINDOW_AUTOSIZE);
	//cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	//cv::namedWindow(this->skinwin,CV_WINDOW_AUTOSIZE);

	cv::Mat concat_image = this->concat_image(this->analyzed_img,this->analyzed_img_filtered);
	cv::moveWindow(this->winname,0,0);
	printf("Image_processor::show_analyzed_img() showing analyzed_img\n");	
	cv::imshow(this->winname,concat_image);

	printf("Image_processor::show_analyzed_img() showing skin_img\n");
	cv::imshow(this->skinwin,this->skin_img);

	//cv::imshow(this->edgewin,this->edge_img);
	if(continuity == 0)
	{
		char k;
		while( (k = cv::waitKey(0)) != 'n')
		{
			if(k == 'e')
				exit(0);
			printf("You have pressed %c %d\n",k,k);
		}
	}
	else
	{
		//imshow does not block the main process any more
		//but instead it waits for 3 second
		cv::waitKey(3000);
	}
	printf("Image_processor::show_analyzed_img() exiting\n");
	return 1;
}

/*
 * Implementation of test()
 */
void Image_processor::test()
{
	//create a window named as this->winname
	while(true)
	{
		if(!this->capture_image())
		{
			printf("Image_processor Error: cannot capture valid image\n");
			continue;
		}
		//if image is capture, run basic analysis
		this->run_body_detection(this->current_img,this->body_detect);
		this->run_face_detection(this->current_img,this->face_detect);

		//mark the detected results
		this->analyzed_img = this->mark_detected_body(this->current_img,this->body_detect);
		this->analyzed_img = this->mark_detected_face(this->analyzed_img,this->face_detect);
		this->show_analyzed_img();

		//run basic filter;
		this->basic_filter();

		//mark the detected results
		this->analyzed_img = this->mark_detected_body(this->current_img,this->final_body_detect);
		this->analyzed_img = this->mark_detected_face(this->analyzed_img,this->final_face_detect);
		this->show_analyzed_img();
	}
	return ;
}

uint8_t Image_processor::read_image(const char* filename)
{
	this->current_img = cv::imread(filename,CV_LOAD_IMAGE_COLOR);
	return 1;
}

uint8_t Image_processor::basic_filter()
{
	printf("Image_processor::basic_filter() working\n");
	this->final_body_detect.clear();
	this->final_face_detect.clear();
	//for each body deteced, try to find a face detected in the body region
	size_t count_body = 0, count_face = 0;
	for(count_body = 0;count_body < this->body_detect.size();count_body++)
	{
		for(count_face = 0; count_face < this->face_detect.size();count_face++)
		{
			if(this->face_body_related(body_detect[count_body],face_detect[count_face]))
			{
				printf("Image_processor basic_filter():a person is detected\n");
				//adjust the body_detect to better match the face
				body_detect[count_body].height+=body_detect[count_body].y - face_detect[count_face].y;
				body_detect[count_body].y = face_detect[count_face].y;

				this->final_body_detect.push_back(body_detect[count_body]);
				this->final_face_detect.push_back(face_detect[count_face]);
			}
		}
	}

	//if no body is detected after the filtering and a face is detected only once
	if(this->final_body_detect.size() == 0 && this->face_detect.size() == 1)
	{
		uint8_t factor = 1;
		float height_factor = 7.5;
		cv::Rect rect = face_detect[0];

		//if the face is at the lower section of the image
		if(rect.y > IMG_CENTER_Y)
			return 0;
		
		this->final_face_detect.push_back(rect);

		rect.x = std::max(rect.x - factor * rect.width,0);
		if(rect.x + rect.width * (factor * 2 + 1) > this->current_img.cols)
			rect.width = this->current_img.cols - rect.x;
		else
			rect.width = rect.width * (factor * 2 + 1);

		if(rect.y + rect.height * height_factor > this->current_img.rows)
			rect.height = this->current_img.rows - rect.y;
		else
			rect.height = ceil(rect.height * height_factor);

		this->final_body_detect.push_back(rect);
	}
	printf("Image_processor::basic_filter() exiting\n");
}

uint8_t Image_processor::face_body_related(const cv::Rect &body,const cv::Rect &face)
{
	float face_body_height_ratio = 0.20;
	float face_hori_threshold = 0.5;
	//x, y are the coordinate of the top left corner.
	cv::Point body_center(body.x + body.width / 2,
					body.y + body.height / 2);
	cv::Point face_center(face.x + face.width / 2,
					face.y + face.height / 2);

	/*
		check whether the face_center is higher than the body_center
		because the person's head must be at a higher position than than body, right?
	*/
	if(face_center.y + face.height < body_center.y)
	{
		//check whether the face is within the width of the body horizontally
		if( (body_center.x - (body.width / 2)) < (face_center.x - face.width / 2)
			&& (face_center.x + face.width / 2) < (body_center.x + (body.width / 2)) )
		{	
			//body is related to this face
			//printf("Image_processor face_body_related(): the face_center (%d,%d) the body_center (%d,%d)\n",face_center.x,face_center.y,body_center.x,body_center.y);
			//printf("body_center.x - body.width / 2 is %d, body_center.x + body.width/2 is %d\n",body_center.x - body.width / 2,body_center.x + body.width/2);
		}
		else
			return 0;

		//check whether the body is much longer than the face
		//printf("Image_processor face_body_related(): face height %d body height %d\n",face.height,body.height);
		//printf("Image_processor face_body_related(): face height / body height %f\n",(float)face.height/(float)body.height);
		//printf("Image_processor face_body_related(): threshold %f\n",face_body_height_ratio);
		if(((float)face.height / (float)body.height) > face_body_height_ratio)
			return 0;

		return 1;

	}

	//body is not related to this face
	return 0;
}


uint8_t Image_processor::find_body_according_to_face(const cv::Mat &source_img,const std::vector<cv::Rect> &face_detect)
{
	uint8_t factor = 2;
	printf("find_body_according_to_face(): There are %lu faces\n",face_detect.size());
	std::vector<cv::Rect> body_detect;
	//for every detected face recorded in face_detect
	for(size_t count_face = 0;count_face < face_detect.size();count_face++)
	{
		cv::Mat resulted_img;
		cv::Rect rect = face_detect[count_face];
		//make rect a larger rectangle based on the face detection result

		//1. move the left top to the top
		rect.y = 1;
		//2. move the left top to lefter position
		rect.x = std::max(rect.x - factor * face_detect[count_face].width,0);
		//3. make the width larger
		rect.width*=(factor * 2 + 1);
		if(rect.x + rect.width > source_img.cols)
		{
			rect.width = source_img.cols - rect.x;
		}
		//4. make the height larger
		rect.height = source_img.rows - rect.y;

		this->find_body_in_roi(source_img,rect,body_detect);
		for(size_t count_body = 0;count_body < body_detect.size();count_body++)
		{
			body_detect[count_body].x+=rect.x; 
			body_detect[count_body].y+=rect.y;
		}

		resulted_img = this->mark_detected_body(source_img,body_detect);
		cv::rectangle(resulted_img,rect.tl(),rect.br(),cv::Scalar(0,0,255),2);
		resulted_img = this->mark_detected_face(resulted_img,face_detect);
		cv::destroyWindow(this->winname);
		cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
		cv::imshow(this->winname,resulted_img);
		char key;
		while((key = cv::waitKey(0)) != 'n')
		{
			if(key == 'e')
				exit(-1);
		}
	}
	return 1;
}

uint8_t Image_processor::find_body_in_roi(const cv::Mat &source_img,const cv::Rect roi,std::vector<cv::Rect> &body_detect)
{
	body_detect.clear();
	cv::Mat subImage = source_img(roi);
	cv::Mat resulted_img;
	this->run_body_detection(subImage,body_detect);
	printf("find_body_in_roi(): find %lu body\n",body_detect.size());
	resulted_img = this->mark_detected_body(subImage,body_detect);

	cv::destroyWindow(this->winname);
	cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	cv::imshow(this->winname,resulted_img);
	char key;
	while((key = cv::waitKey(0)) != 'n')
	{
		if(key == 'e')
			exit(-1);
	}
	
	return 1;
}
		
		
uint8_t Image_processor::one_target_in_scope(const uint8_t &flags)
{
	uint8_t enable_body_detect = ((flags & ENABLE_BODY_DETECT) == ENABLE_BODY_DETECT); 
	uint8_t enable_face_detect = ((flags & ENABLE_FACE_DETECT) == ENABLE_FACE_DETECT);
	
	this->body_detect.clear();
	this->face_detect.clear();

	printf("Image_processor::one_target_in_scope() running\n");
	if(!this->capture_image())
	{
		printf("Image_processor Error: cannot capture valid image\n");
		return 0;
	}
	
	//if an image is captured, run basic analysis
	//this->analyzed_img is the initial detection result image
	this->analyzed_img = this->current_img.clone();

	//get the skin color detection result, and store it into skin_img
	this->getSkin(this->current_img,this->skin_img);
	if(enable_body_detect)
	{
		this->run_body_detection(this->current_img,this->body_detect);
		this->analyzed_img = this->mark_detected_body(this->analyzed_img,this->body_detect);
	}
	
	if(enable_face_detect)
	{
		this->run_face_detection(this->current_img,this->face_detect);
		this->analyzed_img = this->mark_detected_face(this->analyzed_img,this->face_detect);
		this->skin_img = this->mark_detected_face(this->skin_img,this->face_detect);
	}
	

	
	//run basic filter;
	this->basic_filter();
	//mark the detected results
	//this->analyzed_img_filtered is the final detection result image
	this->analyzed_img_filtered = this->mark_detected_body(this->current_img,this->final_body_detect);
	this->analyzed_img_filtered = this->mark_detected_face(this->analyzed_img_filtered,this->final_face_detect);
	if(this->final_body_detect.size() >= 1)
	{	
		printf("Image_processor::one_target_in_scope() returning\n");
		return this->final_body_detect.size();			
	}
	else if(this->face_detect.size() == 0)
	{
		//delete the current image if no faces is found in the scope
		printf("Image_processor::one_target_in_scope() returning\n");
		remove(this->current_img_path);
		return 0;
	}
	
	return 0;
}


void Image_processor::mark_exp_region(const cv::Rect &rect)
{
	cv::rectangle(this->analyzed_img,rect.tl(),rect.br(),cv::Scalar(0,0,0x80),2);
	cv::rectangle(this->analyzed_img_filtered,rect.tl(),rect.br(),cv::Scalar(0,0,0x80),2);
	return ;
}
cv::Rect Image_processor::get_detection_result()
{
	return this->final_body_detect[0];
}

cv::Rect Image_processor::get_face_detection_result()
{
	return this->final_face_detect[0];
}

double Image_processor::get_distance(const cv::Rect &face)
{	
	return runCAMShift(face);
}
