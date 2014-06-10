// Written by: Chan Ho Kwan
/*
This program is used to demonstrate the 4-point algorithm using CAMShift method
*/
#include "fourPointAlgo.h"
#include "camshiftdemo.h"

#define MAX_COUNT	4			// max. no. of points being tracked
#define PixelWidth	0.0084		// in mm
#define MMtoP		1/PixelWidth	// no. of pixel per mm, each pixel is 5 um, so 1 mm has 200 pixel
#define GUESS		3000*MMtoP		// initial value of l[0], l[1], l[2], l[3]
#define ERROR		0.1			// lower bound of the errors' value
#define Focal		692.28		// focal length of webcam
#define MaxIter		50			// upper bound of number of iteration of the algorithm
#define A(i, j)		A.at<double>(i, j)
#define a(i, j)		a.at<double>(i, j)

using namespace cv;
using namespace std;



double runCAMShift( CvRect r ){
// double runCAMShift(){
double answer;
	// 2D coordinates of 4 model points on the image plane

	Mat a = (Mat_<double>(4, 2) << r.x, r.y, 
								      r.x, r.y+r.height,
								      r.x+r.width, r.y+r.height,
								  	  r.x+r.width, r.y);
	 // (267,133,148,314)
	// (326,128,34,34)
	
	// Mat a = (Mat_<double>(4, 2) << 326, 128, 
	// 							   326, 128+34,
	// 							   326+34, 128+34,
	// 							  326+34, 128);
	
	//expect 3m 3000mm

	/* POINT 1 POINT 4
	   POINT 2 POINT 3
	  */

	
	// 3D coodinates of 4 model points on the same plane (4 corners of square)
	Mat m = (Mat_<double>(4, 3) << 0.0, 0.0, 0.0,
								   0.0, 150.0*MMtoP, 0.0,
								   200.0*MMtoP, 150.0*MMtoP, 0.0,
								   200.0*MMtoP, 0.0, 0.0);
	// 3D coodinates of 4 model points on different plane
	/*Mat m = (Mat_<double>(4, 3) << 0.0, 0.0, 0.0,
								   0.0, 0.0, 60.0*MMtoP,
								   0.0, 70.0*MMtoP, 0.0,
								   60.0*MMtoP, 0.0, 0.0);*/

	// 3D coordinates computed by 4-point algo
	Mat A = Mat::zeros(4, 3, CV_64F);
	Mat guess = (Mat_<double>(4, 1) << GUESS, GUESS, GUESS, GUESS);
	Mat error = (Mat_<double>(4, 1) << ERROR, ERROR, ERROR, ERROR);

  
	Point3d pos3d = Point3d(0,0,0);

			
			// Here is the 4-point algo
			if (1){
				A = run4PointAlgo(a, m, Focal, guess, ERROR, MaxIter);		// John's 4-point algo
				
			for (int i = 0; i < MAX_COUNT; i++){
					pos3d.x += A(i,0);
					pos3d.y += A(i,1);
					pos3d.z += A(i,2);
				}
				pos3d.x /= 4;
				pos3d.y /= 4;
				pos3d.z /= 4;
				 answer=pos3d.z;
				// putText(image, intToStringCAMShift(pos3d.x*PixelWidth)
				// 		+", "+intToStringCAMShift(pos3d.y*PixelWidth)
				// 		+", "+intToStringCAMShift(pos3d.z*PixelWidth),
				// 		averageCenter, 2, 0.5, Scalar(0,0,0), 2);
				pos3d.x = pos3d.y = pos3d.z = 0.0;
				
			}

		return answer*PixelWidth;

  } 

