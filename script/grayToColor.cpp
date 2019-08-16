/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* srcname = "nature.jpg";
const char* grayname = "011sdout.jpg";
const char* outname = "nagout011.jpg";
Mat src, gray;

int main() {
	src = imread(srcname, 1);
	gray = imread(grayname, 1);

	Mat new_image = Mat::zeros(src.size(), src.type());

	Mat grayhsv, srchsv;
	cvtColor(src, srchsv, CV_BGR2HSV);
	cvtColor(gray, grayhsv, CV_BGR2HSV);

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			new_image.at<Vec3b>(y, x)[0] = srchsv.at<Vec3b>(y, x)[0];
			new_image.at<Vec3b>(y, x)[1] = srchsv.at<Vec3b>(y, x)[1];
			new_image.at<Vec3b>(y, x)[2] = grayhsv.at<Vec3b>(y, x)[2];
		}
	}
	
	Mat output;
	cvtColor(new_image, output, CV_HSV2BGR);

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, output);
	imwrite(outname, output);
	waitKey(0);
	return(0);
}
*/