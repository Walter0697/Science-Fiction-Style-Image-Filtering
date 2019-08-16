#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>      
/*
using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "image.png";
Mat src, dst;

int blurIndex = 2;
int const max_blur = 51;

void blurImage(int, void*);

int main() {
	src = imread(filename, IMREAD_COLOR);
	dst = Mat::zeros(src.size(), src.type());

	namedWindow(window_name, WINDOW_AUTOSIZE);

	createTrackbar("Blur:\n [1-51]", window_name,
					&blurIndex, max_blur,
					blurImage);

	imshow(window_name, dst);

	waitKey(0);
	return(0);
}

void blurImage(int, void*)
{
		for (int i = 1; i < blurIndex; i = i + 2)
		{
			GaussianBlur(src, dst, Size(i, i), 0, 0);
		}

		imshow(window_name, dst);
}

*/