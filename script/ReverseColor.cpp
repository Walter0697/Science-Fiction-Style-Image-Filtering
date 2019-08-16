#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>      
/*
using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "image.png";
Mat src, dst;

void reverseColor();

int main() {
	src = imread(filename, IMREAD_COLOR);
	dst = Mat::zeros(src.size(), src.type());

	namedWindow(window_name, WINDOW_AUTOSIZE);

	reverseColor();

	imshow(window_name, dst);

	waitKey(0);
	return(0);
}

void reverseColor()
{
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			for (int c = 0; c < src.channels(); c++) {
				dst.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar> (255 - src.at<Vec3b>(y, x)[c]);
			}
		}
	}
	imshow(window_name, dst);
}
*/