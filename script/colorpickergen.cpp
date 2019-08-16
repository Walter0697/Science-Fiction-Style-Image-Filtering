/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
const char* outname = "goodgen_ken.jpg";
const char* fucname = "goodgenhist_ken.jpg";
const char* noisename = "noise.png";

Mat noise, output, blurred;

int RED = 0;
int GREEN = 1;
int BLUE = 2;

float darkness = 0.003f;			//lower the darker
float lightness = 0.2f;			//lower the lighter
int redv = 245; int greenv = 203; int bluev = 66;
int rx[5];
int gx[5];
int bx[5];

float range = 0.1f;

void setValue(int name, int index, int x);
float func(int name, int index);
void drawRatio();

int image_size = 256;

int main() {
	noise = imread(noisename, 1);

	setValue(RED, 0, 0);
	setValue(RED, 1, (redv * darkness));
	setValue(RED, 2, redv);
	setValue(RED, 3, (redv * lightness) + 255 * (1 - lightness));
	setValue(RED, 4, 255);

	setValue(GREEN, 0, 0);
	setValue(GREEN, 1, (greenv * darkness));
	setValue(GREEN, 2, greenv);
	setValue(GREEN, 3, (greenv * lightness) + 255 * (1 - lightness));
	setValue(GREEN, 4, 255);

	setValue(BLUE, 0, 0); 
	setValue(BLUE, 1, (bluev * darkness));
	setValue(BLUE, 2, bluev);
	setValue(BLUE, 3, (bluev* lightness) + 255 * (1 - lightness));
	setValue(BLUE, 4, 255);

	drawRatio();
	output = Mat::zeros(cv::Size(image_size, image_size), 16);

	for (int y = 0; y < image_size; y++)
	{
		float ratio = func(RED, y);
		float ratio2 = func(GREEN, y);
		float ratio3 = func(BLUE, y);
		for (int x = 0; x < image_size; x++)
		{
			output.at<Vec3b>(y, x)[2] = (int)(ratio * 255);
			output.at<Vec3b>(y, x)[1] = (int)(ratio2 * 255);
			output.at<Vec3b>(y, x)[0] = (int)(ratio3 * 255);
		}
	}

	GaussianBlur(output, blurred, Size(15, 15), 0, 0);

	
	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, output);

	imwrite(outname, blurred);
	waitKey(0);
	return(0);
}

void setValue(int name, int index, int x)
{
	if (name == RED)
	{
		rx[index] = x;
	}
	else if (name == GREEN)
	{
		gx[index] = x;
	}
	else if (name == BLUE)
	{
		bx[index] = x;
	}
}

//index is from 0 to 255, to 0 to 1
float func(int name, int index)
{
	//float temp = index;
	//float curve = 0.4f; //from 1 - 5
	//temp = 0.5 * (tanh(curve * ((temp / 35.f) - 4)) + 1);

	//float output = temp - fmod(noise.at<Vec3b>(index, 0)[0] / 255.0f, range) + range;
	//float output = temp - fmod(rand(), range) + range;
	float t = index / 255.0f;
	float output = 0;

	//we need to convert index to from 0 to 1
	if (name == RED)
	{
		if (t > 0.5f)
		{ 
			t = (t - 0.5f) * 2;
			output = (1 - t) * ((1 - t) * rx[2] + t * rx[3]) + t * ((1 - t) * rx[3] + t * rx[4]);
		}
		else
		{
			t = t * 2;
			output = (1 - t) * ((1 - t) * rx[0] + t * rx[1]) + t * ((1 - t) * rx[1] + t * rx[2]);
		}
	}
	else if (name == GREEN)
	{
		if (t > 0.5f)
		{
			t = (t - 0.5f) * 2;
			output = (1 - t) * ((1 - t) * gx[2] + t * gx[3]) + t * ((1 - t) * gx[3] + t * gx[4]);
		}
		else
		{
			t = t * 2;
			output = (1 - t) * ((1 - t) * gx[0] + t * gx[1]) + t * ((1 - t) * gx[1] + t * gx[2]);
		}
	}
	else if (name == BLUE)
	{
		if (t > 0.5f)
		{
			t = (t - 0.5f) * 2;
			output = (1 - t) * ((1 - t) * bx[2] + t * bx[3]) + t * ((1 - t) * bx[3] + t * bx[4]);
		}
		else
		{
			t = t * 2;
			output = (1 - t) * ((1 - t) * bx[0] + t * bx[1]) + t * ((1 - t) * bx[1] + t * bx[2]);
		}
	}
	

	output /= 255.0f;

	if (output <= 0) return 0;
	if (output >= 1) return 1;
	return output;
}

void drawRatio()
{
	int hist_w = 512; int hist_h = 400; int histSize = 256;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(func(RED, i - 1) * hist_h)),
		Point(bin_w * (i), hist_h - cvRound(func(RED, i) * hist_h)),
		Scalar(0, 0, 255), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(func(GREEN, i - 1) * hist_h)),
		Point(bin_w * (i), hist_h - cvRound(func(GREEN, i) * hist_h)),
		Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(func(BLUE, i - 1) * hist_h)),
		Point(bin_w * (i), hist_h - cvRound(func(BLUE, i) * hist_h)),
		Scalar(255, 0, 0), 2, 8, 0);
	}

	imwrite(fucname, histImage);
}
*/