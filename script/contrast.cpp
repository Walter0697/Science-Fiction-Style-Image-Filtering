/*#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

char window_name[] = "OpenCV";
const char* filename = "n003.jpg";
Mat src;

int RGBSum(Vec3b)

int main() {
	src = imread(filename, 1);

	Mat new_image = Mat::zeros(src.size(), src.type());

	double alpha = 4.2;
	int beta = -100;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++) 
		{
			for (int c = 0; c < src.channels(); c++)
			{
			//try using condition
				new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar> (alpha * src.at<Vec3b>(y, x)[c] + beta );
			}
		}
	}

	namedWindow(window_name, WINDOW_AUTOSIZE);
	imshow(window_name, new_image);
	imwrite("output.jpg", new_image);
	waitKey(0);
	return(0);
}
*/