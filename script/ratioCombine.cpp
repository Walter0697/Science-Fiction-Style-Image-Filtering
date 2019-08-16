/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* srcname = "n004.jpg";					//very original image
const char* refname = "test3_matching.jpg";				//image after histogram matching
const char* colname = "goodgen_73_227_227.jpg";			//color from sci-fi image
const char* outname = "goodgentest3_withr_005.jpg";		
const char* fucname = "goodtest_func_ken.jpg";	
Mat srcimg, refimg, colimg, temp;

float curve = 1.5f; //from 1 - 5
float light = 0.6f;	//from -2 to 2
float offset = 2.8f;
float height = 0.2f;	//default is 0.5, but for sci-fi-ish, 0.4

float getRatio(char* method, int index);
void drawRatio(char* method);

int main() {
	srcimg = imread(srcname, 1);
	refimg = imread(refname, 1);
	resize(refimg, refimg, srcimg.size());
	colimg = imread(colname, 1);

	//drawRatio("sigmoid");
	Mat new_image = Mat::zeros(srcimg.size(), srcimg.type());
	
	for (int y = 0; y < srcimg.rows; y++) 
	{
		for (int x = 0; x < srcimg.cols; x++)
		{
			int index = refimg.at<Vec3b>(y, x)[0];
			float ratio = getRatio("sigmoid", index);
			new_image.at<Vec3b>(y, x)[0] = srcimg.at<Vec3b>(y, x)[0] * (1 - ratio) + colimg.at<Vec3b>(index, 0)[0] * (ratio);
			new_image.at<Vec3b>(y, x)[1] = srcimg.at<Vec3b>(y, x)[1] * (1 - ratio) + colimg.at<Vec3b>(index, 0)[1] * (ratio);
			new_image.at<Vec3b>(y, x)[2] = srcimg.at<Vec3b>(y, x)[2] * (1 - ratio) + colimg.at<Vec3b>(index, 0)[2] * (ratio);
		}
	}

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, new_image);
	imwrite(outname, new_image);
	waitKey(0);
	return(0);
}

//from 0 to 255 return a number between 0 to 1
float getRatio(char* method, int index)
{
	//return 1;
	if (method == "linear")
	{
		float temp = index;
		return temp / 255.0f;
	}
	else if (method == "sigmoid")
	{
		float temp = index;
		return height * ( tanh( curve * ( ( temp / 35.f ) - 4 + light) ) + 1 + offset);
	}
	else if (method == "expo")
	{
		float temp = index;
		return pow(2, ((temp / 65.f) - 2)) / 4.f;
	}
	return index;
}

void drawRatio(char* method)
{
	int hist_w = 512; int hist_h = 400; int histSize = 256;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(getRatio(method, i - 1) * hist_h)), 
				Point(bin_w * (i), hist_h - cvRound(getRatio(method, i) * hist_h)),
				Scalar(255, 255, 255), 2, 8, 0);
	}

	imwrite(fucname, histImage);
}
*/