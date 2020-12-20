#pragma once

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "filter.h"
#include "time.h"

using namespace cv;
using namespace std;

float deviation(Mat& compare);

int main()
{
	clock_t t;
	Mat img = imread("D:/Technical vision/data/thresh_input.jpg", IMREAD_GRAYSCALE);
	imshow("img", img);
	while (waitKey(0) != 27)
	{
		;
	}
	Mat clone = img.clone();

	Mat result(Size(clone.cols, clone.rows), CV_8UC1, Scalar(255));
	AdaptiveThreshold filter(125, 10, 13);
	t = clock();
	filter.adaptiveThreshold(img, result);
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;
	imshow("result", result);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat result2(Size(clone.cols, clone.rows), CV_8UC1, Scalar(255));
	t = clock();
	adaptiveThreshold(clone, result2, 125, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;
	imshow("result2", result2);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat compare(Size(result.cols, result.rows), CV_8UC1, Scalar(0));
	absdiff(result2, result, compare);
	imshow("compare", compare);
	while (waitKey(0) != 27)
	{
		;
	}

	float error = deviation(compare);
	cout << error;

	while (waitKey(33) != 27) {}

	return 0;
}

float deviation(Mat& compare)
{
	int sum = 0;
	int n = compare.rows * compare.cols;

	for (int i = 0; i < compare.rows; i++)
	{
		for (int j = 0; j < compare.cols; j++)
		{
			sum += (int)compare.at<uint8_t>(i, j);
		}
	}

	float middle = (float)sum / n;

	float s = 0;
	float sum_2 = 0;

	for (int i = 0; i < compare.rows; i++)
	{
		for (int j = 0; j < compare.cols; j++)
		{
			sum_2 += ((float)compare.at<uint8_t>(i, j) - middle) * ((float)compare.at<uint8_t>(i, j) - middle);
		}
	}

	s = sqrt((sum_2 / n) / (n - 1));

	return s;
}