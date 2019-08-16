/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "sort_source_009.jpg";
const char* outname = "sorted_009.jpg";
Mat src, temp, gray;
std::vector<int> grayscale, position;

int image_size = 256;

void blurImage(Mat in, Mat& out, int index);
void sort(std::vector<int>& src, std::vector<int>& ref);
void merge(std::vector<int>& src, std::vector<int>& ref, int l, int m, int r);
void mergeSort(std::vector<int>& src, std::vector<int>& ref, int l, int r);

int main() {
	src = imread(filename, 1);
	resize(src, src, cv::Size(image_size, image_size));

	//turn to gray
	cvtColor(src, temp, CV_BGR2GRAY);
	imwrite("sortgray.jpg", temp);	
	gray = imread("sortgray.jpg", 1);

	//create 1d array to store all pixels info
	for (int y = 0; y < gray.rows; y++)
	{
		for (int x = 0; x < gray.cols; x++)
		{
			grayscale.push_back(gray.at<Vec3b>(y, x)[0]);
			position.push_back(y * gray.rows + x);
		}
	}
	
	//sort 2 array, as one for reference
	sort(grayscale, position);

	//rearrange the original image to the newimage with the reference of the 1d array 
	Mat sorted_image = Mat::zeros(src.size(), src.type());
	for (int y = 0; y < sorted_image.rows; y++)
	{
		for (int x = 0; x < sorted_image.cols; x++)
		{
			int curr_pos = y * sorted_image.rows + x;
			int new_pos = position[curr_pos];
			int new_y = new_pos / sorted_image.rows;
			int new_x = new_pos % sorted_image.rows;
			sorted_image.at<Vec3b>(y, x) = src.at<Vec3b>(new_y, new_x);
		}
	}

	//pick only the last part of the image and stress it
	Mat color_pick = Mat::zeros(src.size(), src.type());
	for (int y = 0; y < color_pick.rows; y++)
	{
		for (int x = 0; x < color_pick.cols; x++)
		{
			color_pick.at<Vec3b>(y, x) = sorted_image.at<Vec3b>(y, image_size - 1);
		}
	}

	Mat blurred_image;
	blurImage(color_pick, blurred_image, 15);

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, blurred_image);

	//imwrite("step1_sorted.jpg", sorted_image);
	//imwrite("step2_colorp.jpg", color_pick);
	//imwrite("step3_blurred.jpg", blurred_image);
	imwrite(outname, blurred_image);
	waitKey(0);
	return(0);
}

void blurImage(Mat in, Mat& out, int index)
{
	GaussianBlur(in, out, Size(index, index), 0, 0);
}

void sort(std::vector<int>& src, std::vector<int>& ref)
{
	mergeSort(src, ref, 0, src.size() - 1);
}

void merge(std::vector<int>& src, std::vector<int>& ref, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	int* L = new int[n1];
	int* R = new int[n2];
	int* rL = new int[n1];
	int* rR = new int[n2];

	for (i = 0; i < n1; i++)
	{
		L[i] = src[l + i];
		rL[i] = ref[l + i];
	}
	for (j = 0; j < n2; j++)
	{
		R[j] = src[m + 1 + j];
		rR[j] = ref[m + 1 + j];
	}

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			src[k] = L[i];
			ref[k] = rL[i];
			i++;
		}
		else
		{
			src[k] = R[j];
			ref[k] = rR[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		src[k] = L[i];
		ref[k] = rL[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		src[k] = R[j];
		ref[k] = rR[j];
		j++;
		k++;
	}

	delete L;
	delete R;
	delete rL;
	delete rR;
}

void mergeSort(std::vector<int>& src, std::vector<int>& ref, int l, int r)
{
	if (l < r)
	{
 		int m = l + (r - l) / 2;
		
		mergeSort(src, ref, l, m);

		mergeSort(src, ref, m + 1, r);

		merge(src, ref, l, m, r);
	}
}
*/