#include "Image_processor.h"

Image_processor::Image_processor(uint8_t img_source)
{
	printf("Constructing Image processor\n");
	this->state = IMAGE_PROCESS_INIT;
	strcpy(this->winname,"hello, world");
	this->win_exist = 0;
	this->img_source = img_source;
	if(this->img_source == IMG_SOURCE_WEBCAM)
	{
		this->cap = new cv::VideoCapture(0);
		if(!cap->isOpened())
		{
			printf("Image_processor Error: Cannot open camera\n");
			exit(-1);
		}
	}
	else if(this->img_source == IMG_SOURCE_CELLPHONE)
	{
		this->cam = new Camera();
		std::string ip("192.168.43.1:8080");
		this->cam->setip(ip);
	}
	//check the existence of the directory for storing the capture image
}

Image_processor::~Image_processor()
{
	printf("Destructing Image processor\n");
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
	printf("Image_processor init(): setting the default face detector");
	if( !this->face_cascade.load(this->face_cascade_name) ){ printf("--(!)Error loading face cascade\n"); exit(-1); };
	if( !this->eyes_cascade.load(this->eyes_cascade_name) ){ printf("--(!)Error loading eyes cascade\n"); exit(-1); };

	return 1;
}
IMAGE_PROCESS_STATE Image_processor::get_state()
{
	return this->state;
}

uint8_t Image_processor::get_image_from_cellphone()
{
	char *filename = (char *) malloc(sizeof(char) * FILENAME_LENGTH);
	strcpy(filename,this->cam->photo_af().c_str());
	printf("Image_processor::get_image_from_cellphone: Reading from %s\n",filename);
	this->current_img = cv::imread(filename,CV_LOAD_IMAGE_COLOR);
	if(!this->current_img.data)
	{
		printf("Image_processor::get_image_from_cellphone: No data is loaded from the cellphone\n");
		getchar();
	}
	return 1;
}

uint8_t Image_processor::get_image_from_webcam()
{
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
 *	Implementation of stored_current_image()
 *
 */
uint8_t Image_processor::save_current_image()
{
	char *filename =(char *) malloc(sizeof(char) * FILENAME_LENGTH);
	time_t timestamp = time(NULL);
	struct tm *current_time = gmtime(&timestamp);
	//get the filename in format of month-day_hour:minute:second
	sprintf(filename,"%d-%d_%d:%d:%d.png",
			current_time->tm_mon,
			current_time->tm_mday,
			current_time->tm_hour,
			current_time->tm_min,
			current_time->tm_sec);
	//specify the compression parameters
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	//write the data into the file
	cv::imwrite(filename,this->current_img,compression_params);
	free(filename);
}
/*
 * Implementation of basic analyzation
 */
uint8_t Image_processor::analyze_image()
{

}
/*
 *
 * Implementation of basic pedestrain detection algorithm
 *
 */
 uint8_t Image_processor::run_body_detection()
 {
 	this->body_detect.clear();
 	std::vector<cv::Rect> found;
 	this->hog.detectMultiScale(this->current_img,found,0,cv::Size(8,8),cv::Size(32,32),1.05,2);
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
 			this->body_detect.push_back(r);
 	}
 	return 1;
 }
cv::Mat Image_processor::mark_detected_body(cv::Mat source_img)
{
	cv::Mat marked_img = source_img.clone();
	//for every detected face
	for(size_t count = 0;count < this->body_detect.size();count++)
	{
		cv::Rect r = this->body_detect[count];
		cv::rectangle(marked_img,r.tl(),r.br(),cv::Scalar(0,255,0),2);
		/*
		cv::Rect r = this->body_detect[count];
		r.x += cvRound(r.width * 0.1);
		r.width = cvRound(r.width * 0.8);
		r.y += cvRound(r.height * 0.06);
		r.height = cvRound(r.height * 0.9);
		//draw the rectangle on the analyzed_img
		cv::rectangle(marked_img,r.tl(),r.br(),cv::Scalar(0,255,0),2);
		*/
	}
	return marked_img;
}
 uint8_t Image_processor::basic_pedestrain_detection()
 {
 	this->run_body_detection();
 	printf("basic_pedestrain_detection: %d body detected\n",this->body_detect.size());
 	this->analyzed_img = this->mark_detected_body(this->current_img);
 	return 1;
 }
/*
 *
 * Implementation of basic face detection
 *
 */
uint8_t Image_processor::run_face_detection()
{
	this->face_detect.clear();
	this->eyes_detect.clear();

	//use gray image for face detection
	cv::Mat frame_gray;
	cv::cvtColor(this->current_img, frame_gray,cv::COLOR_BGR2GRAY );
	cv::equalizeHist(frame_gray, frame_gray);
	//doing face detection
	this->face_cascade.detectMultiScale(frame_gray, this->face_detect, 1.1, 2, 0, cv::Size(10, 10));
	printf("run_face_detection: detect %d faces\n",this->face_detect.size());
	return 1;
}
cv::Mat Image_processor::mark_detected_face(cv::Mat source_img)
{
	cv::Mat marked_img = source_img.clone();
	//for evert detected face
	for(size_t count = 0;count < this->face_detect.size();count++)
	{
		//create a point noting the center of the region where a face is detected
		cv::Point center(this->face_detect[count].x + this->face_detect[count].width/2,
						this->face_detect[count].y + this->face_detect[count].height/2);
		//draw a ellipse for the face
		cv::ellipse(marked_img,center,
				cv::Size(this->face_detect[count].width/2,this->face_detect[count].height/2),
				0,0,360, cv::Scalar(255,0,0),2,8,0);
	}
	return marked_img;
}
uint8_t Image_processor::basic_face_detection()
{
	printf("Entering basic_face_detection\n");
	if(!this->run_face_detection())
	{
		printf("Image_processor basic_face_detection ERROR\n");
		return -1;
	}
	this->analyzed_img = this->mark_detected_face(this->current_img);
	return 1;
}
/*
 *
 */
uint8_t Image_processor::show_analyzed_img()
{
	cv::destroyWindow(this->winname);
	cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	cv::imshow(this->winname,this->analyzed_img);
	char k;
	while( (k = cv::waitKey(0)) != 'n')
	{
		if(k == 'e')
			exit(0);
		printf("You have pressed %c %d\n",k,k);
	}

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
		//if image is capture, show it to the window
		//this->save_current_image();
		
		//this->show_analyzed_img();
		this->basic_pedestrain_detection();
		this->basic_face_detection();
		this->show_analyzed_img();
	}
	return ;
}

uint8_t Image_processor::read_image(const char* filename)
{
	this->current_img = cv::imread(filename,CV_LOAD_IMAGE_COLOR);
	return 1;
}

uint8_t Image_processor::load_current_img_to_analyzed_img()
{
	this->analyzed_img = this->current_img.clone();
	return 1;
}

uint8_t Image_processor::basic_filter()
{
	this->final_detect.clear();
	//for each body deteced, try to find a face detected in the body region
	size_t count_body = 0, count_face = 0;
	for(count_body = 0;count_body < this->body_detect.size();count_body++)
	{
		for(count_face = 0; count_face < this->face_detect.size();count_face++)
		{
			if(this->face_body_related(body_detect[count_body],face_detect[count_face]))
			{
				this->final_detect.push_back(body_detect[count_body]);
			}
		}
	}
}

uint8_t Image_processor::face_body_related(const cv::Rect &body,const cv::Rect &face)
{
	cv::Point body_center(body.x + body.width / 2,
							body.y +_body.height / 2);
	cv::Point face_center(face.x + face.width / 2,
							face.y + face.height / 2);
	return 1;
}
