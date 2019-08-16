/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* srcname = "n004.jpg";
const char* fstref = "goodgentest3_withr_001.jpg";
const char* secref = "goodgentest3_withr_003.jpg";
const char* hsref = "test3_matching.jpg";
Mat src, ref1, ref2, hm;

float size_of_light_source = 0.5;
int size_of_section = 30;

//float size_of_light_source = 0.7;
//int size_of_section = 19;

//float size_of_light_source = 0.1;
//int size_of_section = 15;

int line_index = 6;

float fstmin = 0.5, fstmax = 3.0;
float sndmin = 0.2, sndmax = 0.8;

Mat getAlpha(Mat markers, vector<vector<Point>> contours, int index);

int main() {
	src = imread(srcname, 1);
	ref1 = imread(fstref, 1);
	ref2 = imread(secref, 1);
	hm = imread(hsref, 1);

	namedWindow(window_name, WINDOW_AUTOSIZE);

	Mat kernel = (Mat_<float>(3, 3) <<
			1, 1, 1,
			1, -8, 1,
			1, 1, 1);

	Mat imgLaplacian;
	Mat sharp = src;
	filter2D(sharp, imgLaplacian, CV_32F, kernel);
	src.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - imgLaplacian;

	imgResult.convertTo(imgResult, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
	
	Mat bw;
	cvtColor(imgResult, bw, CV_BGR2GRAY);
	threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	Mat dist;
	distanceTransform(bw, dist, CV_DIST_L2, 3);

	normalize(dist, dist, 0, 1., NORM_MINMAX);

	threshold(dist, dist, size_of_light_source, 1., CV_THRESH_BINARY);

	Mat kernel1 = Mat::ones(size_of_section, size_of_section, CV_8UC1);
	dilate(dist, dist, kernel1);

	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);

	vector<vector<Point>> contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat markers = Mat::zeros(dist.size(), CV_32SC1);

	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);

	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);

	watershed(src, markers);

	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);

	//so, this part is basically showing, not actually having any purpose!
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int b = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);

		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	Mat colored = Mat::zeros(markers.size(), CV_8UC3);
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				colored.at<Vec3b>(i, j) = colors[index - 1];
			}
			else
			{
				colored.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
		}
	}

	Mat dt1 = getAlpha(markers, contours, 0);
	Mat dt2 = getAlpha(markers, contours, 1);

	//combining everything
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			//int index = markers.at<int>(i, j);
			//if (index > 0 && index <= static_cast<int>(contours.size()))
			//{
			//	if (index > line_index)
			//		dst.at<Vec3b>(i, j) = ref1.at<Vec3b>(i, j);
			//	else
			//		dst.at<Vec3b>(i, j) = ref2.at<Vec3b>(i, j);
			//}
			//else
			//	dst.at<Vec3b>(i, j) = hm.at<Vec3b>(i, j);
			for (int c = 0; c < 3; c++)
			{ 
				float result = ref1.at<Vec3b>(i, j)[c] * dt1.at<float>(i, j) +
								ref2.at<Vec3b>(i, j)[c] * dt2.at<float>(i, j);
				if (result > 255) 
					dst.at<Vec3b>(i, j)[c] = 255;
				else 
					dst.at<Vec3b>(i, j)[c] = result;
			}
		}
	}
	
	imwrite("final7_normalize.jpg", dt1 * 200);
	imwrite("final8_normalize.jpg", dt2 * 200);
	
	//imshow(window_name, dt1); waitKey(0);
	//imshow(window_name, dt2); waitKey(0);
	imshow(window_name, dst); waitKey(0);
	//imwrite("combine_output_before.jpg", markers * 10000);
	imwrite("combine_output_6.jpg", dst);

	
	return(0);
}

Mat getAlpha(Mat markers, vector<vector<Point>> contours, int x)
{
	Mat end_dst = Mat::zeros(markers.size(), CV_8UC3);

	for (int i = 0; i < end_dst.rows; i++)
	{
		for (int j = 0; j < end_dst.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				if (x == 0)
				{ 
					if (index > line_index)
					{ 
						end_dst.at<Vec3b>(i, j)[0] = 255;
						end_dst.at<Vec3b>(i, j)[1] = 255;
						end_dst.at<Vec3b>(i, j)[2] = 255;
					}
					else
					{ 
						end_dst.at<Vec3b>(i, j)[0] = 0;
						end_dst.at<Vec3b>(i, j)[1] = 0;
						end_dst.at<Vec3b>(i, j)[2] = 0;
					}
				}
				else if (x == 1)
				{
					if (index > line_index)
					{
						end_dst.at<Vec3b>(i, j)[0] = 0;
						end_dst.at<Vec3b>(i, j)[1] = 0;
						end_dst.at<Vec3b>(i, j)[2] = 0;
					}
					else
					{
						end_dst.at<Vec3b>(i, j)[0] = 255;
						end_dst.at<Vec3b>(i, j)[1] = 255;
						end_dst.at<Vec3b>(i, j)[2] = 255;
					}
				}
			}
			else
			{
				end_dst.at<Vec3b>(i, j)[0] = 0;
				end_dst.at<Vec3b>(i, j)[1] = 0;
				end_dst.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}

	cvtColor(end_dst, end_dst, COLOR_BGR2GRAY);
	//if (x == 0)
	//	imwrite("final5_upper.jpg", end_dst);
	//else
	//	imwrite("final6_upper.jpg", end_dst);

	Mat dist_trans;
	distanceTransform(end_dst, dist_trans, DIST_L2, 3);
	if (x == 0)
	{ 
		
		normalize(dist_trans, dist_trans, fstmin, fstmax, NORM_MINMAX);
	}
	else
	{ 
		normalize(dist_trans, dist_trans, sndmin, sndmax, NORM_MINMAX);
	}

	return dist_trans;
}
*/