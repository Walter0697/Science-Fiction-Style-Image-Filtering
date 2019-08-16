
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <unordered_set>

using namespace std;
using namespace cv;

char window_name[] = "OpenCV";
//this should be the only source image people need for now
const char* filename = "testing_image/test001.jpg";
const char* refname = "reference_image/ref003.jpg";
const char* noisename = "noise/perlin2.png";

const char* funcname = "output/ratio.jpg";
const char* outputname = "output/result001.jpg";
Mat srcimg, refimg, noiseimg, dst;

void image_histogram(Mat, Mat&, bool, bool = true, bool = false);
void histogram_matching(Mat, Mat, Mat&);

float getRatio(int, float = 1.5f, float = 0.6f, float = 2.8f, float = 0.2f);
void drawRatio(float = 1.5f, float = 0.6f, float = 2.8f, float = 0.2f, char* = "ratio.jpg");
void fillColor(Mat, Mat, Mat, Mat&, float = 1.5f, float = 0.6f, float = 2.8f, float = 0.2f);

float getRatioByColor(int, int, float = 0.003f, float = 0.2f);
void colorPickByMidPoint(Mat&, int, int, int, float = 0.003f, float = 0.2f);
void drawRatio(int, int, int, float = 0.003f, float = 0.2f, char* = "color_ratio.jpg");

Mat getAlpha(Mat, vector<vector<Point>>, int, int, float, float);
void getMarkers(Mat, Mat&, vector<vector<Point>>&, float, int);
Mat fillSectionColor(Mat, Mat, vector<vector<Point>>);
void finalCombine(Mat, Mat, Mat, Mat, Mat, Mat&, bool = false);

int main() {

	srcimg = imread(filename, 1);
	refimg = imread(refname, 1);
	noiseimg = imread(noisename, 1);
	namedWindow(window_name, WINDOW_AUTOSIZE);
	
	Mat hmimg;
	histogram_matching(srcimg, refimg, hmimg);
	imshow(window_name, hmimg);
	waitKey(0);

	//drawRatio();

	Mat color1, color2, combineColor1, combineColor2;
	colorPickByMidPoint(color1, 240, 71, 255);
	fillColor(srcimg, hmimg, color1, combineColor1);	

	//imshow(window_name, color1);
	//waitKey(0);
	//imshow(window_name, combineColor1);
	//waitKey(0);

	//173, 252, 255
	colorPickByMidPoint(color2, 255, 139, 66);
	fillColor(srcimg, hmimg, color2, combineColor2);
	//imshow(window_name, color2);
	//waitKey(0);
	//imshow(window_name, combineColor2);
	//waitKey(0);

	//drawRatio( 1.5f, 0.6f, 2.8f, 0.2f, "ratio.jpg");

	Mat markers;
	vector<vector<Point>> contours;
	getMarkers(srcimg, markers, contours, 0.035f, 10);
	//imshow(window_name, markers * 1000000);
	//waitKey(0);

	Mat alpha1 = getAlpha(markers, contours, 0, contours.size() / 2 - 1, 0.1f, 0.4f);
	Mat alpha2 = getAlpha(markers, contours, contours.size() / 2, contours.size(), 0.8f, 1.9f);

	//imshow(window_name, alpha1);
	//waitKey(0);
	//imshow(window_name, alpha2);
	//waitKey(0);

	finalCombine(combineColor1, combineColor2, alpha1, alpha2, markers, dst);
	
	imshow(window_name, dst);
	imwrite(outputname, dst);
	
	waitKey(0);
	return(0);
}

//gettingthe image histogram with the source image
void image_histogram(Mat input, Mat& histImage, bool color, bool uniform, bool accumulate)
{
	Mat process = input;
	if (!color)			//if color is false, then we should turn the image into grayscale
	{ 
		Mat gray;
		cvtColor(process, gray, CV_BGR2GRAY);
		imwrite("gray.jpg", gray);

		process = imread("gray.jpg", 1);
	}

	//initializing values for histogram
	vector<Mat> bgr_planes;
	split(process, bgr_planes);

	int histSize = 256;

	float range[] = { 0, 256 };
	const float* histRange = { range };

	Mat b_hist, g_hist, r_hist;

	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//drawing the pixel number to the image
	for (int i = 1; i < histSize; i++)
	{
		if (!color)
		{ 
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
				Scalar(255, 255, 255), 2, 8, 0);
		}
		else
		{ 
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
				Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}
	}
}

//use a source image and a reference image to generate an image with histogram matching
void histogram_matching(Mat input, Mat ref, Mat& output)
{
	//turning all pictures into grayscale
	Mat inputgray, refgray, temp;
	cvtColor(input, temp, CV_BGR2GRAY);
	imwrite("gray.jpg", temp);
	inputgray = imread("gray.jpg", 1);

	cvtColor(ref, temp, CV_BGR2GRAY);
	imwrite("gray.jpg", temp);
	refgray = imread("gray.jpg", 1);
	resize(refgray, refgray, input.size());	//resize it so that the number of pixel is exactly the same

	int histSize = 256;
	float range[] = { 0 , 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	//getting the pixel data
	vector<Mat> ref_planes;
	split(refgray, ref_planes);
	Mat ref_hist;
	calcHist( &ref_planes[0], 1, 0, Mat(), ref_hist, 1, &histSize, &histRange, uniform, accumulate );
	vector<Mat> src_planes;
	split(inputgray, src_planes);
	Mat src_hist;
	calcHist( &src_planes[0], 1, 0, Mat(), src_hist, 1, &histSize, &histRange, uniform, accumulate );

	//initialize the pixel reference list
	int histList[256];
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

	output = Mat::zeros(inputgray.size(), inputgray.type());

	for (int y = 0; y < inputgray.rows; y++)
	{
		for (int x = 0; x < inputgray.cols; x++)
		{
			output.at<Vec3b>(y, x)[0] = histList[inputgray.at<Vec3b>(y, x)[0]];
			output.at<Vec3b>(y, x)[1] = histList[inputgray.at<Vec3b>(y, x)[1]];
			output.at<Vec3b>(y, x)[2] = histList[inputgray.at<Vec3b>(y, x)[2]];
		}
	}
}

//getting the ratio with the corresponding index
float getRatio(int index, float curve, float light, float offset, float height)
{
	float temp = index;
	return height * (tanh( curve * ( ( temp / 35.f ) - 4 + light ) ) + 1 + offset);
}

//drawing the curve in a histogram
void drawRatio(float curve, float light, float offset, float height, char* filename)
{
	//initialize everything
	int hist_w = 512; int hist_h = 400; int histSize = 256;
	int bin_w = cvRound((double)hist_w / histSize);

	//drawing into the image
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(getRatio(i - 1, curve, light, offset, height) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(getRatio(i, curve, light, offset, height) * hist_h)),
			Scalar(255, 255, 255), 2, 8, 0);
	}

	//output
	imwrite(filename, histImage);
}

//fill color with special ratio
void fillColor(Mat src, Mat ref, Mat col, Mat& dst, float curve, float light, float offset, float height)
{
	dst = Mat::zeros(src.size(), src.type());

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			int index = ref.at<Vec3b>(y, x)[0];
			float ratio = getRatio(index, curve, light, offset, height);
			dst.at<Vec3b>(y, x)[0] = src.at<Vec3b>(y, x)[0] * (1 - ratio) + col.at<Vec3b>(index, 0)[0] * ratio;
			dst.at<Vec3b>(y, x)[1] = src.at<Vec3b>(y, x)[1] * (1 - ratio) + col.at<Vec3b>(index, 0)[1] * ratio;
			dst.at<Vec3b>(y, x)[2] = src.at<Vec3b>(y, x)[2] * (1 - ratio) + col.at<Vec3b>(index, 0)[2] * ratio;
		}
	}
}

float getRatioByColor(int index, int midvalue, float darkness, float lightness)
{
	float p1 = 0;
	float p2 = midvalue * darkness;
	float p3 = midvalue;
	float p4 = (midvalue * lightness) + 255 * (1 - lightness);
	float p5 = 255;

	float t = index / 255.0f;
	float output = 0;

	if (t > 0.5f)
	{
		t = (t - 0.5f) * 2;
		output = (1 - t) * ((1 - t) * p3 + t * p4) + t * ((1 - t) * p4 + t * p5);
	}
	else
	{
		t = t * 2;
		output = (1 - t) * ((1 - t) * p1 + t * p2) + t * ((1 - t) * p2 + t * p3);
	}
	
	output /= 255.0f;
	if (output <= 0) return 0;
	if (output >= 1) return 1;
	return output;
}

void colorPickByMidPoint(Mat& dst, int r, int g, int b, float darkness, float lightness)
{
	int image_size = 256;
	dst = Mat::zeros(cv::Size(image_size, image_size), 16);

	for (int y = 0; y < image_size; y++)
	{
		float ratio1 = getRatioByColor(y, r, darkness, lightness);
		float ratio2 = getRatioByColor(y, g, darkness, lightness);
		float ratio3 = getRatioByColor(y, b, darkness, lightness);
		for (int x = 0; x < image_size; x++)
		{
			dst.at<Vec3b>(y, x)[2] = (int)(ratio1 * 255);
			dst.at<Vec3b>(y, x)[1] = (int)(ratio2 * 255);
			dst.at<Vec3b>(y, x)[0] = (int)(ratio3 * 255);
		}
	}

	GaussianBlur(dst, dst, Size(15, 15), 0, 0);
}

void drawRatio(int r, int g, int b, float darkness, float lightness, char* filename)
{
	//initialize everything
	int hist_w = 512; int hist_h = 400; int histSize = 256;
	int bin_w = cvRound((double)hist_w / histSize);

	//drawing into the image
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(getRatioByColor(i - 1, r, darkness, lightness) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(getRatioByColor(i, r, darkness, lightness) * hist_h)),
			Scalar(0, 0, 255), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(getRatioByColor(i - 1, g, darkness, lightness) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(getRatioByColor(i, g, darkness, lightness) * hist_h)),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(getRatioByColor(i - 1, b, darkness, lightness) * hist_h)),
			Point(bin_w * (i), hist_h - cvRound(getRatioByColor(i, b, darkness, lightness) * hist_h)),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	//output
	imwrite(filename, histImage);
}

Mat getAlpha(Mat markers, vector<vector<Point>> contours, int fst_index, int snd_index, float min, float max)
{
	Mat end_dst = Mat::zeros(markers.size(), CV_8UC3);

	for (int i = 0; i < end_dst.rows; i++)
	{
		for (int j = 0; j < end_dst.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				if (index > fst_index && index < snd_index)
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
			else
			{
				end_dst.at<Vec3b>(i, j)[0] = 0;
				end_dst.at<Vec3b>(i, j)[1] = 0;
				end_dst.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}

	cvtColor(end_dst, end_dst, COLOR_BGR2GRAY);

	Mat dist_trans;
	distanceTransform(end_dst, dist_trans, DIST_L2, 3);
	normalize(dist_trans, dist_trans, min, max, NORM_MINMAX);

	return dist_trans;
}

void getMarkers(Mat src, Mat& markers, vector<vector<Point>>& contours, float size_of_light_source, int size_of_section)
{
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

	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	markers = Mat::zeros(dist.size(), CV_32SC1);

	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);

	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);

	watershed(src, markers);
}

Mat fillSectionColor(Mat src, Mat markers, vector<vector<Point>> contours)
{
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);

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

	return colored;
}

void finalCombine(Mat ref1, Mat ref2, Mat dst1, Mat dst2, Mat markers, Mat& dst, bool noise = false)
{
	//setting Perlin noise
	resize(noiseimg, noiseimg, ref1.size());

	dst = Mat::zeros(markers.size(), CV_8UC3);
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				float result = (ref1.at<Vec3b>(i, j)[c] * dst1.at<float>(i, j) +
								ref2.at<Vec3b>(i, j)[c] * dst2.at<float>(i, j) );
				if (noise)
					result = result * (noiseimg.at<Vec3b>(i, j)[0] / 255.0f);				if (result > 255)
					dst.at<Vec3b>(i, j)[c] = 255;
				else
					dst.at<Vec3b>(i, j)[c] = result;
			}
		}
	}
}
