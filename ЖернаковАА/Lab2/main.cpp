#include <iostream>
#include "opencv2/core.hpp";
#include "opencv2/highgui.hpp";
#include "opencv2/imgproc.hpp";
#include "Filter.h"
#include "time.h"

using namespace cv;
using namespace std;

int main()
{
	clock_t t;
	Mat img = imread("img.jpg", IMREAD_GRAYSCALE);
	imshow("img", img);
	while (waitKey(0) != 27)
	{
		;
	}
	Mat clone = img.clone();

	Mat result(Size(clone.cols, clone.rows), CV_8UC1);
	AdaptiveThreshold filter(120, 10, 5);
	t = clock();
	filter.adaptiveThreshold(img, result);
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;
	imshow("result", result);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat result2(Size(clone.cols, clone.rows), CV_8UC1);
	t = clock();
	adaptiveThreshold(clone, result2, 120, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 10);
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;
	imshow("result2", result2);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat compare(Size(result.cols, result.rows), CV_8UC1, Scalar(0));
	absdiff(result2, result, compare);
	imshow("compare", compare*127);
	while (waitKey(0) != 27)
	{
		;
	}

	int sum = 0;
	float average = 0;
	int n = 0;
	int counter = 0;
	float S = 0;
	n = compare.rows * compare.cols;

	for (int i = 0; i < compare.rows; i++) // строки
	{
		for (int j = 0; j < compare.cols; j++) // столбцы
		{
			S += compare.at<uchar>(i, j);
			if (compare.at<uchar>(i, j) != 0)
				counter++;
		}
	}

	float S2 = 0;
	S2 = sqrt(S * S / n);
	cout << counter << endl;
	double prob = (double)counter / (double)n;
	cout << S2 << endl;
	cout << prob * 100 << endl;

	//average = (float)sum / (float)n;
	//for (int i = 0; i < compare.rows - 2; i++) // строки
	//{
	//	for (int j = 0; j < compare.cols - 2; j++) // столбцы
	//	{
	//		if(compare.at<uchar>(i, j) > 1)
	//		S += ((float)compare.at<uchar>(i, j) - average) * ((float)compare.at<uchar>(i, j) - average);
	//	}
	//}	

	return 0;
}