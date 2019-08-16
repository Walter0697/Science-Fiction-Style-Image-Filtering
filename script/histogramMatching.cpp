/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <unordered_set>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "n004.jpg";
const char* refname  = "ro007.jpg";
const char* outputname = "test3_matching.jpg";
Mat srcimg, refimg, srcgray, refgray, temp, outputimg;
int histList[256];

int main() {
	srcimg = imread(filename, 1);
	refimg = imread(refname, 1);
	
	cvtColor(refimg, temp, CV_BGR2GRAY);
	imwrite("gray.jpg", temp);
	refgray = imread("gray.jpg", 1);
	//resize the reference image into the size of source one
	resize(refgray, refgray, srcimg.size());
	outputimg = refgray;
	cvtColor(srcimg, temp, CV_BGR2GRAY);
	imwrite("gray2.jpg", temp);
	srcgray = imread("gray2.jpg", 1);
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	//INITIALIZE EVERYTHING ABOVE

	vector<Mat> ref_planes;
	split(refgray, ref_planes);
	Mat ref_hist;
	calcHist( &ref_planes[0], 1, 0, Mat(), ref_hist, 1, &histSize, &histRange, uniform, accumulate );
	vector<Mat> src_planes;
	split(srcgray, src_planes);
	Mat src_hist;
	calcHist( &src_planes[0], 1, 0, Mat(), src_hist, 1, &histSize, &histRange, uniform, accumulate );

	for (int i = 0; i < histSize; i++)
	{
		histList[i] = i;
	}

	int cur_ref = 0;
	int off = 0;
	for (int i = 0; i < histSize; i++)
	{
		//src_pixel = src_hist.at<float>(i);
		if (off == 0)
		{
			histList[i] = cur_ref;
			off = src_hist.at<float>(i) - ref_hist.at<float>(cur_ref);
		}
		else if (off > 0)
		{
			while (off > 0)
			{
				off -= ref_hist.at<float>(cur_ref);
				cur_ref++;
			}
			histList[i] = cur_ref;
			off += src_hist.at<float>(i);
		}
		else
		{
			off += src_hist.at<float>(i);
			histList[i] = cur_ref;
		}
	}

	//int from 0 to final
	//and then again for the reference
	//another int for keep tracking if it is too dark

	Mat out_src = Mat::zeros(srcgray.size(), srcgray.type());
	//looping picture to track which pixel is which
	for (int y = 0; y < srcgray.rows; y++)
	{
		for (int x = 0; x < srcgray.cols; x++)
		{
			out_src.at<Vec3b>(y, x)[0] = histList[srcgray.at<Vec3b>(y, x)[0]];
			out_src.at<Vec3b>(y, x)[1] = histList[srcgray.at<Vec3b>(y, x)[1]];
			out_src.at<Vec3b>(y, x)[2] = histList[srcgray.at<Vec3b>(y, x)[2]];
		}
	}

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, out_src);
	imwrite(outputname, out_src);
	waitKey(0);
	return(0);
}
*/