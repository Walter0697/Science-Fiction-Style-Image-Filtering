/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>      

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "sorted_009.jpg";
const char* outputname = "sorted_009_hist.jpg";
Mat src, temp, gray;


int main() {
	src = imread(filename, 1);

	//Mat temp;
	//cvtColor(src, temp, CV_BGR2GRAY);
	//imwrite("histgray.jpg", temp);

	//gray = imread("histgray.jpg", 1);

	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	int histSize = 256;

	float range[] = {0, 256};
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;


	Mat b_hist, g_hist, r_hist;

	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(0 ,0, 0));

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage,	Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
						Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i)) ),
						Scalar( 255, 0, 0), 2, 8, 0 );
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
						Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
						Scalar( 0 , 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
						Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
						Scalar( 0, 0, 255), 2, 8, 0);
	}
	
	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, histImage);
	imwrite(outputname, histImage);
	waitKey(0);
return(0);
}
*/