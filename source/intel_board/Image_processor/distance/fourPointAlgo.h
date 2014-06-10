// Written by: Chan Ho Kwan
/*
This is the header file of 4-point algorithm
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

Mat run4PointAlgo(Mat a, Mat m, double focal, Mat guess, double error, int maxiter);

