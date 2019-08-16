/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename	= "sorted_009.jpg";
const char* outputname	= "sorted_009_analysis.jpg";
Mat src;

int main() {
	src = imread(filename, 1);

	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	int size = 256;

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/ size );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(0 ,0, 0));

	for (int i = 1; i < size; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((src.at<Vec3b>(i - 1, 0)[0] / 256.f) * hist_h)),
						Point(bin_w * (i), hist_h - cvRound((src.at<Vec3b>(i, 0)[0] / 256.f) * hist_h)),
						Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((src.at<Vec3b>(i - 1, 0)[1] / 256.f) * hist_h)),
						Point(bin_w * (i), hist_h - cvRound((src.at<Vec3b>(i, 0)[1] / 256.f) * hist_h)),
						Scalar(0 ,255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((src.at<Vec3b>(i - 1, 0)[2] / 256.f) * hist_h)),
						Point(bin_w * (i), hist_h - cvRound((src.at<Vec3b>(i, 0)[2] / 256.f) * hist_h)),
						Scalar(0, 0, 255), 2, 8, 0);
	}

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, histImage);
	imwrite(outputname, histImage);
	waitKey(0);
	return(0);
}
*/