#pragma once
#include "Fourier.h"
#include "opencv.hpp"
using namespace std;
using namespace cv;
using namespace std;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void swapSpektr(Mat& magI)
{
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy)); // верхний левый
	Mat q1(magI, Rect(cx, 0, cx, cy)); // верхний правый
	Mat q2(magI, Rect(0, cy, cx, cy)); // нижний левый
	Mat q3(magI, Rect(cx, cy, cx, cy)); // нижний правый

	Mat tmp; // обмен квадрантов 0-3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp); // обмен квадрантов 1-2
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

int createGaussian(float sigma, Mat& returningImage, Mat source1, Mat source2) {
	const float amplitude = 1.0f;
	Mat m_gauss = Mat(Size(source1.cols, source1.rows), CV_32F);
	for (int r = 0; r < source1.cols; r++)
	{
		for (int c = 0; c < source1.rows; c++)
		{
			const float x = (c - source1.cols / 2) * (static_cast<float>(c) - source1.cols / 2) / (2.0f *
				sigma * sigma);
			const float y = (r - source1.rows / 2) * (static_cast<float>(r) - source1.rows / 2) / (2.0f *
				sigma * sigma);
			m_gauss.at<float>(r, c) = amplitude * exp(-(x + y));
		}
	}
	normalize(m_gauss, m_gauss, 0.0, 1.0, NORM_MINMAX);
	swapSpektr(m_gauss);
	Mat clone;
	clone = m_gauss;
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("m_gauss", clone);
	while (waitKey(0) != 27)
	{
	}

	Mat minus_gauss = Mat(Size(source1.cols, source1.rows), CV_32FC1);
	for (int r = 0; r < source1.cols; r++)
	{
		for (int c = 0; c < source1.rows; c++)
		{
			const float x = (c - source1.cols / 2) * (static_cast<float>(c) - source1.cols / 2) / (2.0f *
				sigma * sigma);
			const float y = (r - source1.rows / 2) * (static_cast<float>(r) - source1.rows / 2) / (2.0f *
				sigma * sigma);
			minus_gauss.at<float>(r, c) = 1 - (amplitude * exp(-(x + y))); 
		}
	}

	normalize(minus_gauss, minus_gauss, 0.0, 1.0, NORM_MINMAX);
	swapSpektr(minus_gauss);
	clone = minus_gauss;
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("minus_gauss", clone);
	while (waitKey(0) != 27)
	{
	}

	
	cout << endl<< type2str(source1.type()) << endl;
	cout << endl<< type2str(source2.type()) << endl;

	vector<Mat> temp1;
	split(source1, temp1);

	vector<Mat> temp2;
	split(source2, temp2);

	cout << endl<< type2str(temp2[1].type()) << endl;
	cout << endl<< type2str(temp2[0].type()) << endl;

	multiply(temp1[0], m_gauss, temp1[0]);
	multiply(temp1[1], m_gauss, temp1[1]);
	multiply(temp2[0], minus_gauss, temp2[0]);
	multiply(temp2[1], minus_gauss, temp2[1]);

	clone = temp1[0];
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("temp1[0]", clone);
	while (waitKey(0) != 27)
	{
	}

	clone = temp1[1];
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("temp1[1]", clone);
	while (waitKey(0) != 27)
	{
	}

	clone = temp2[0];
	clone.convertTo(clone, CV_8UC1, 255);
	imshow(" temp2[0]", clone);
	while (waitKey(0) != 27)
	{
	}

	clone = temp2[1];
	//normalize(clone, clone, 0.0, 1, NORM_MINMAX);
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("temp2[1]", clone);
	while (waitKey(0) != 27)
	{
	}
	
	

	vector<Mat> tempFin = { Mat(Size(source2.cols, source2.rows), CV_32FC1), Mat(Size(source2.cols, source2.rows), CV_32FC2) };

	tempFin[0] = temp1[0] + temp2[0];
	tempFin[1] = temp1[1] + temp2[1];

	merge(tempFin, returningImage);

	return 0;
}

int doSuperTask(string JacketName, string ForestName, float Sigma, Mat& result)
{
	Mat Jacket = imread(JacketName, IMREAD_GRAYSCALE);
	imshow("Source1", Jacket);
	while (waitKey(0) != 27)
	{
	}
	cout << endl<< type2str(Jacket.type()) << endl;
	Jacket.convertTo(Jacket, CV_32FC1);

	Mat Forest = imread(ForestName, IMREAD_GRAYSCALE);
	imshow("Source2", Forest);
	while (waitKey(0) != 27)
	{
	}
	cout << endl<< type2str(Forest.type()) << endl;
	Forest.convertTo(Forest, CV_32FC1);

	cout << endl<< type2str(Jacket.type()) << endl;
	cout << endl<< type2str(Forest.type()) << endl;

	Mat JacketReAndIm = Mat(Size(Jacket.rows, Jacket.cols), CV_32FC2);

	dft(Jacket, JacketReAndIm, DFT_COMPLEX_OUTPUT);

	Mat ForestReAndIm = Mat(Size(Forest.rows, Forest.cols), CV_32FC2);
	dft(Forest, ForestReAndIm, DFT_COMPLEX_OUTPUT);

	cout << endl<< type2str(JacketReAndIm.type()) << endl;
	cout << endl<< type2str(ForestReAndIm.type()) << endl;
	
	Mat resultSum = Mat(Size(ForestReAndIm.cols, ForestReAndIm.rows), CV_32FC2);
	createGaussian(Sigma, resultSum, JacketReAndIm, ForestReAndIm);

	Mat superResult = Mat(Size(resultSum.cols, resultSum.rows), CV_32FC1);
	dft(resultSum, superResult, DFT_INVERSE | DFT_REAL_OUTPUT);

	normalize(superResult, superResult, 0, 1, NORM_MINMAX);
	superResult.convertTo(superResult, CV_8UC1, 255);
	imshow("superResult", superResult);
	while (waitKey(0) != 27)
	{
	}

	result = superResult;

	return 0;
}
