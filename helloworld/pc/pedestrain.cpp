#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>

//for directory operation
#include <sys/types.h>
#include <dirent.h>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{

	DIR *dir;
	struct dirent *ent;
	if((dir = opendir(argv[1])) == NULL)
	{
		perror("");
		return EXIT_FAILURE;
	}


	while((ent = readdir(dir)) != NULL)
	{
		char filename[64];
		strcpy(filename,argv[1]);
		strcat(filename,"/");
		strcat(filename,ent->d_name);
		printf("Openning %s\n",filename);

		Mat img = imread(filename,CV_LOAD_IMAGE_COLOR);
		HOGDescriptor hog;
		//use the default People Detector
		cout<<"Setting the default people detector"<<endl;
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		cout<<"Setting done"<<endl;
		cout<<"Creatting a Window"<<endl;
		namedWindow("Pedestrian Detection",CV_WINDOW_AUTOSIZE);
		cout<<"Creation done"<<endl;
		if(!img.data)
		{
			cout<<"Currently no valid data"<<endl;
			continue;
		}
		cout<<"Capture valid frame"<<endl;
		vector<Rect> found, found_filtered;
		/*
		 *
		 *void HOGDescriptor::detectMultiScale(
		 *						const Mat& img, vector<Rect>& foundLocations,
		 *						double hitThreshold, Size winStride, Size padding,
		 *						double scale0, double finalThreshold, bool useMeanshiftGrouping) const
		 *
		 */
		hog.detectMultiScale(img,found,0,Size(8,8),Size(32,32),1.05,2);

		size_t i,j;
		//first for loop
		cout<<"Entering first for loop"<<endl;
		for(i = 0; i < found.size();i++)
		{
			Rect r = found[i];
			for (j=0; j<found.size(); j++)
				if (j!=i && (r & found[j])==r)
					break;
			if (j==found.size())
				found_filtered.push_back(r);
		}
		cout<<"Entering second for loop"<<endl;
		//second for loop
		for(i = 0; i < found_filtered.size();i++)
		{
			Rect r = found_filtered[i];
			r.x += cvRound(r.width * 0.1);
			r.width = cvRound(r.width * 0.8);
			r.y += cvRound(r.height * 0.06);
			r.height = cvRound(r.height * 0.9);
			rectangle(img,r.tl(),r.br(),cv::Scalar(0,255,0),2);
		}
		imshow("Pedestrian Detection",img);
		if(waitKey(5000) == 'n')
			continue;
		else
			break;
	}
	return 0;
}	
