/*
#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "image.png";
Mat src, dst;

int alpha = 10;
int const max_alpha = 30;

int blue = 0;
int const max_blue = 100;
int green = 0;
int const max_green = 100;
int red = 0;
int const max_red = 100;

void modifyColor(int, void*);

int main() {
	src = imread(filename, IMREAD_COLOR);
	dst = Mat::zeros(src.size(), src.type());

	namedWindow(window_name, WINDOW_AUTOSIZE);

	createTrackbar("Alpha:\n [1.0-3.0]", window_name,
	&alpha, max_alpha,
	modifyColor);
	createTrackbar("Blue: \n[0-100]", window_name,
	&blue, max_blue,
	modifyColor);
	createTrackbar("Green: \n[0-100]", window_name,
	&green, max_green,
	modifyColor);
	createTrackbar("Red: \n[0-100]", window_name,
	&red, max_red,
	modifyColor);

	imshow(window_name, dst);

	waitKey(0);
	return(0);
}

void modifyColor(int, void*)
	{
	float a = alpha / 10.f;
	for (int y = 0; y < src.rows; y++) {
	for (int x = 0; x < src.cols; x++) {
	for (int c = 0; c < src.channels(); c++) {
	if (c == 0)
	dst.at<Vec3b>(y, x)[c] =
	saturate_cast<uchar> (a*src.at<Vec3b>(y, x)[c] + blue);
	else if (c == 1)
	dst.at<Vec3b>(y, x)[c] =
	saturate_cast<uchar> (a*src.at<Vec3b>(y, x)[c] + green);
	else
	dst.at<Vec3b>(y, x)[c] =
	saturate_cast<uchar> (a*src.at<Vec3b>(y, x)[c] + red);
	}
	}
	}
	imshow(window_name, dst);
}
*/