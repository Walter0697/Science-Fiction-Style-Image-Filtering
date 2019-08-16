/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <string>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
char track_name[] = "Trackbar";
Mat dst;
int hist_w, hist_h, histSize, bin_w;

int heightR = 10; int lightR = 10; int offsetR = 10; int curveR = 8;
int heightG = 10; int lightG = 10; int offsetG = 10; int curveG = 8;
int heightB = 10; int lightB = 10; int offsetB = 10; int curveB = 8;


int const max_height = 20;
int const max_light = 20;
int const max_offset = 20;
int const max_curve = 20;

float func(char* name, int index, float h, float l, float o, float c);
void drawImage(int, void*);
void drawimg();

int main() {
	hist_w = 512; hist_h = 400; histSize = 256;
	bin_w = cvRound((double)hist_w / histSize);

	dst = Mat::zeros(cv::Size(histSize, histSize), 16);
	//dst = Mat(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//namedWindow(track_name, WINDOW_AUTOSIZE);
	namedWindow(window_name, WINDOW_AUTOSIZE);

	while (true)
	{ 
		drawimg();
		heightR = rand() % 20; lightR = rand() % 20; offsetR = rand() % 20; curveR = rand() % 20;
		heightG = rand() % 20; lightG = rand() % 20; offsetG = rand() % 20; curveG = rand() % 20;
		heightB = rand() % 20; lightB = rand() % 20; offsetB = rand() % 20; curveB = rand() % 20;

		waitKey(0);
	}
	/*
	createTrackbar("HeightR: ", track_name,
	&heightR, max_height,
	drawImage);
	createTrackbar("LightR: ", track_name,
	&lightR, max_light,
	drawImage);
	createTrackbar("OffsetR: ", track_name,
	&offsetR, max_offset,
	drawImage);
	createTrackbar("CurveR: ", track_name,
	&curveR, max_curve,
	drawImage);

	createTrackbar("HeightG: ", track_name,
		&heightG, max_height,
		drawImage);
	createTrackbar("LightG: ", track_name,
		&lightG, max_light,
		drawImage);
	createTrackbar("OffsetG: ", track_name,
		&offsetG, max_offset,
		drawImage);
	createTrackbar("CurveG: ", track_name,
		&curveG, max_curve,
		drawImage);

	createTrackbar("HeightB: ", track_name,
		&heightB, max_height,
		drawImage);
	createTrackbar("LightB: ", track_name,
		&lightB, max_light,
		drawImage);
	createTrackbar("OffsetB: ", track_name,
		&offsetB, max_offset,
		drawImage);
	createTrackbar("CurveB: ", track_name,
		&curveB, max_curve,
		drawImage);
		

	imshow(window_name, dst);

	waitKey(0);
	return(0);
}

//index is from 0 to 255, to 0 to 1
float func(char* name, int index, float h, float l, float o, float c)
{
	float temp = index;
	if (name == "linear")
	{
		temp = index / 255.0f;
	}
	else if (name == "sigmoid")
	{
		temp = index;
		temp = h * (tanh(c * ((temp / 35.f) - 4 + l)) + 1 + o);
	}
	else if (name == "expo")
	{
		temp = index;
		temp = pow(2, ((temp / 65.f) - 2)) / 4.f;
	}
	float output = temp;
	if (output <= 0) return 0;
	if (output >= 1) return 1;
	return output;
}

void drawimg()
{
	float hr = heightR / 20.f;
	float lr = (lightR / 20.f) - 0.5f;
	float or = (offsetR / 20.f) - 0.5f;
	float cr = (curveR / 20.f);
	float hg = heightG / 20.f;
	float lg = (lightG / 20.f) - 0.5f;
	float og = (offsetG / 20.f) - 0.5f;
	float cg = (curveG / 20.f);
	float hb = heightB / 20.f;
	float lb = (lightB / 20.f) - 0.5f;
	float ob = (offsetB / 20.f) - 0.5f;
	float cb = (curveB / 20.f);
	cout << "hr: " << std::to_string(hr) << ", lr: " << std::to_string(lr) << ",or:" << std::to_string(or) << ",cr: " << std::to_string(cr) << endl;
	cout << "hg: " << std::to_string(hg) << ", lg: " << std::to_string(lg) << ",og:" << std::to_string(og) << ",cg: " << std::to_string(cg) << endl;
	cout << "hb: " << std::to_string(hb) << ", lb: " << std::to_string(lb) << ",ob:" << std::to_string(ob) << ",cb: " << std::to_string(cb) << endl;

	for (int y = 0; y < histSize; y++)
	{

		float ratio = func("sigmoid", y, hr, lr, or , cr);
		float ratio2 = func("sigmoid", y, hg, lg, og, cg);
		float ratio3 = func("sigmoid", y, hb, lb, ob, cb);
		for (int x = 0; x < histSize; x++)
		{
			dst.at<Vec3b>(y, x)[0] = (int)(ratio * 255);
			dst.at<Vec3b>(y, x)[1] = (int)(ratio2 * 255);
			dst.at<Vec3b>(y, x)[2] = (int)(ratio3 * 255);
		}
	}

	imshow(window_name, dst);
}

void drawImage(int, void*)
{
	float hr = heightR / 20.f;
	float lr = (lightR / 20.f) - 0.5f;
	float or = (offsetR / 20.f) - 0.5f;
	float cr = (curveR / 20.f);
	float hg = heightG / 20.f;
	float lg = (lightG / 20.f) - 0.5f;
	float og = (offsetG / 20.f) - 0.5f;
	float cg = (curveG / 20.f);
	float hb = heightB / 20.f;
	float lb = (lightB / 20.f) - 0.5f;
	float ob = (offsetB / 20.f) - 0.5f;
	float cb = (curveB / 20.f);
	/*
	dst.release();

	for (int i = 1; i < histSize; i++)
	{
		line(dst, Point(bin_w * (i - 1), hist_h - cvRound(func("sigmoid", i - 1, hr, lr, or , cr) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(func("sigmoid", i, hr, lr, or , cr) * hist_h)),
			Scalar(255, 0, 0), 2, 8, 0);
		line(dst, Point(bin_w * (i - 1), hist_h - cvRound(func("sigmoid", i - 1, hg, lg, og, cg) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(func("sigmoid", i, hg, lg, og, cg) * hist_h)),
			Scalar(0, 255, 0), 2, 8, 0);
		line(dst, Point(bin_w * (i - 1), hist_h - cvRound(func("sigmoid", i - 1, hb, lb, ob, cb) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(func("sigmoid", i, hb, lb, ob, cb) * hist_h)),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	imshow(window_name, dst);
	
	
	

	for (int y = 0; y < histSize; y++)
	{
		
		float ratio = func("sigmoid", y, hr, lr, or, cr);
		float ratio2 = func("sigmoid", y, hg, lg, og, cg);
		float ratio3 = func("sigmoid", y, hb, lb, ob, cb);
		for (int x = 0; x < histSize; x++)
		{
			dst.at<Vec3b>(y, x)[0] = (int)(ratio * 255);
			dst.at<Vec3b>(y, x)[1] = (int)(ratio2 * 255);
			dst.at<Vec3b>(y, x)[2] = (int)(ratio3 * 255);
		}
	}
	
	imshow(window_name, dst);
	
}
*/