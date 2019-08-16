/*
#include<opencv2/opencv.hpp>

#include <iostream>
#include <conio.h>      
#include <math.h>
#include <vector>

using namespace std;
using namespace cv;

struct PointXY
{
	int x;
	int y;
};

char window_name[] = "OpenCV";
const char* filename = "final_result_3.jpg";
Mat src, dst;

int T = 35;
float fc = 1.0f;
float fs = 0.5f;
float a = 1;
float fg = 1.0f;
float f = 1;
int minStrokeLength = 3;
int maxStrokeLength = 15;

void paint(Mat, Mat, int[], int);
void paintLayer(Mat, Mat, int, bool);
int makeSplineStroke(int, int, int, Mat, Mat, Mat, Mat);
void drawStroke(vector<PointXY>, Vec3b, Mat, int);
PointXY findMax(Mat, int, int, int);
int stand_deviation(vector<int>);
void mergeSort(int[], int, int);
void merge(int[], int, int, int);

int main() {
	src = imread(filename, IMREAD_COLOR);
	dst = Mat::zeros(src.size(), src.type());

	namedWindow(window_name, WINDOW_AUTOSIZE);

	int radius[4] = {20, 8, 4, 2};
	paint(src, dst, radius, 4);

	waitKey(0);
	return(0);
}

//main paint function
void paint(Mat src, Mat dst, int radius[], int size)
{
	//initialize the final matrix
	dst = Mat::zeros(src.size(), src.type());
	
	//sort the radius so that it is from largest to smallest
	mergeSort(radius, 0, size - 1);

	//see if the canvas is first paint
	bool firstPaint = true;
	for (int i = 0 ; i < size; i++)
	{
		Mat referenceImage;
		GaussianBlur(src, referenceImage, Size(radius[i] * f + 1, radius[i] * f + 1), 0, 0);
		paintLayer(dst, referenceImage, radius[i], firstPaint);
		firstPaint = false;
		imshow(window_name, dst);
		waitKey();
	}

	imwrite("output.jpg", dst);
}

//paintLayer function
void paintLayer(Mat dst, Mat ref, int radius, bool firstPaint)
{
	//S := a new set of strokes, initially empty
	vector<PointXY> points;
	// create a pointwise difference image
	//D := difference(dst, ref)
	Mat D;
	absdiff(ref, dst, D);

	int grid = fg * radius;

	for (int y = 0; y < D.rows; y += grid)
	{
		for (int x = 0; x < D.cols; x += grid)
		{
			float areaError = 0;
			if (firstPaint) areaError = T * 2;
			else
			{ 
				//M := the region
				for (int j = y - (grid / 2); j < y + (grid / 2); j++)
				{
					for (int i = x - (grid / 2); i < x + (grid / 2); i++)
					{
						if (i >= 0 && j >= 0 && i < D.cols && j < D.rows)
						{ 
							areaError += ((float)(D.at<Vec3b>(j, i)[0] + D.at<Vec3b>(j, i)[1] + D.at<Vec3b>(j, i)[2]) / (float)(grid * grid));
						}
					}
				}
			}
			if (areaError > T)
			{
				PointXY xy = findMax(D, x, y, grid);
				points.push_back(xy);
			}
		}
	}

	//shuffle
	random_shuffle(points.begin(), points.end());

	Mat gray;
	cvtColor(ref, gray, CV_BGR2GRAY, 0);

	Mat grad_x, grad_y, abs_x, abs_y;
	Scharr(gray, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_x);
	Scharr(gray, grad_y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_y);

	//randomly paint
	for (int i = 0; i < points.size(); i++)
	{
		makeSplineStroke(points[i].x, points[i].y, radius, dst, ref, abs_x, abs_y);
		//circle(dst, CvPoint(points[i].x, points[i].y), radius, ref.at<Vec3b>(points[i].y, points[i].x), -1);
	}
}

int makeSplineStroke(int x, int y, int R, Mat dst, Mat ref, Mat gx, Mat gy)
{
	//draw stroke in dst by ref
	Vec3b color = ref.at<Vec3b>(y, x);
	int refColor = color[0] + color[1] + color[2];
	vector<PointXY> K;
	PointXY xy;
	xy.x = x; xy.y = y;
	PointXY last;
	last.x = 0; last.y = 0;

	for (int i = 1; i < maxStrokeLength; i++)
	{
		if (xy.x >= 0 && xy.y >= 0 && xy.x < dst.cols && xy.y < dst.rows)
		{ 
			Vec3b canvas = dst.at<Vec3b>(xy.y, xy.x);
			int canColor = canvas[0] + canvas[1] + canvas[2];
			//maybe fix this
			if (i > minStrokeLength && refColor < canColor )
			{
				drawStroke(K, color, dst, R);
				return 1;
			}
			//gradientMag
			
			if (sqrt((float)gx.at<uchar>(xy.y, xy.x) * (float)gx.at<uchar>(xy.y, xy.x) + (float)gx.at<uchar>(xy.y, xy.x)  * (float)gx.at<uchar>(xy.y, xy.x))  == 0)
			{
				drawStroke(K, color, dst, R);
				return 2;
			}
		}

		PointXY g;
		if (xy.x >= 0 && xy.y >= 0 && xy.x < gx.cols && xy.y < gx.rows)
		{ 
			g.x = (float)gx.at<uchar>(xy.y, xy.x);
			g.y = (float)gy.at<uchar>(xy.y, xy.x);
		}
		else
		{
			g.x = 0;
			g.y = 0;
		}
		PointXY d;
		d.x = -g.y;
		d.y = g.x;

		if (last.x * d.x + last.y * d.y < 0){
			d.x = -d.x;		
			d.y = -d.y;	
		}
		
		d.x = fc * d.x + (1 - fc) * last.x;
		d.y = fc * d.y + (1 - fc) * last.y;
		d.x = d.x / sqrt(d.x * d.x + d.y * d.y);
		d.y = d.y / sqrt(d.x * d.x + d.y * d.y);
		xy.x = x + R * d.x;
		xy.y = y + R * d.y;
		last.x = d.x;
		last.y = d.y;

		K.push_back(xy);
	}
	drawStroke(K, color, dst, R);
	return 3;
}

void drawStroke(vector<PointXY> K, Vec3b color, Mat dst, int R)
{
	for (int i = 0; i < K.size(); i++)
	{
		if (K[i].x >= 0 && K[i].y >= 0 && K[i].x < dst.cols && K[i].y < dst.rows)
		{ 
			//dst.at<Vec3b>(K[i].y, K[i].x) = color;
			circle(dst, CvPoint(K[i].x, K[i].y), R, color, -1);
		}
	}
}

int stand_deviation(vector<int> numbers)
{
	float sum = 0.0f;
	float mean = 0.0f;
	float variance = 0.0f;
	for (int i = 0; i < numbers.size(); i++)
		sum += numbers[i];
	mean = sum / numbers.size();
	for (int i = 0; i < numbers.size(); ++i)
		variance += (numbers[i] - mean) * (numbers[i] - mean);
	variance /= numbers.size();
	return sqrt(variance);
}

PointXY findMax(Mat src, int x, int y, int grid)
{
	PointXY point;
	point.x = 0;
	point.y = 0;
	int max = 0;

	for (int j = y - grid / 2; j < y + grid / 2; j++)
	{
		for (int i = x - grid / 2; i < x + grid / 2; i++)
		{
			if (i >= 0 && j >= 0 && i < src.cols && j < src.rows)
			{
				int current = (src.at<Vec3b>(j, i)[0] + src.at<Vec3b>(j, i)[1] + src.at<Vec3b>(j, i)[2]);
				if (current > max)
				{
					point.x = i;
					point.y = j;
					max = current;
				}
			}
		}
	}
	return point;
}

//mergeSort(arr, 0, sizeof(arr) / sizeof(*arr))
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;

		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}

void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	int* L = new int[n1];
	int* R = new int[n2];

	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] >= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}

	delete L;
	delete R;
}
*/