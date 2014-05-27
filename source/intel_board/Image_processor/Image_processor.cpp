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
	if(this->img_source == IMG_SOURCE_CELLPHONE)
	{
		this->cam = new Camera();
		std::string ip("192.168.43.1:8080");
		this->cam->setip(ip);
	}
}

Image_processor::~Image_processor()
{
	printf("Destructing Image processor\n");
	delete this->cam;
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
 * Implementation of basic pedestrain detection algorithm
 */
 uint8_t Image_processor::basic_pedestrain_detection()
 {
 	this->analyzed_img = this->current_img;
	cv::HOGDescriptor hog;
	//use the default People Detector
	printf("Imaga_processor: Setting the default people detector\n");
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	if(!this->analyzed_img.data)
	{
		printf("Imaga_processor Error: Currently no valid data\n");
		return -1;
	}
	printf("Capture valid frame\n");
	std::vector<cv::Rect> found, found_filtered;

	//the main detection algorithm is run here
	hog.detectMultiScale(this->analyzed_img,found,0,cv::Size(8,8),cv::Size(32,32),1.05,2);

	size_t i,j;
	//first for loop
	printf("Entering first for loop\n");
	for(i = 0; i < found.size();i++)
	{
		cv::Rect r = found[i];
		for (j=0; j<found.size(); j++)
		{	
			if (j!=i && (r & found[j])==r)
				break;
		}

		if (j==found.size())
			found_filtered.push_back(r);
	}
	printf("Entering second for loop\n");
	//second for loop
	for(i = 0; i < found_filtered.size();i++)
	{
		cv::Rect r = found_filtered[i];
		r.x += cvRound(r.width * 0.1);
		r.width = cvRound(r.width * 0.8);
		r.y += cvRound(r.height * 0.06);
		r.height = cvRound(r.height * 0.9);
		cv::rectangle(this->analyzed_img,r.tl(),r.br(),cv::Scalar(0,255,0),2);
	}
 }
/*
 *
 */
uint8_t Image_processor::show_analyzed_img()
{
	cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	cv::imshow(this->winname,this->analyzed_img);
	cv::waitKey(0);
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
		this->save_current_image();
		this->basic_pedestrain_detection();
		this->show_analyzed_img();
		printf("PRESS ANY KEY TO CONTINUE\n");
		getchar();
	}
	return ;
}
