#pragma once

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <math.h>
#include <complex>

using namespace cv;
using namespace std;


class FourierTransform
{
public:
	FourierTransform();
	~FourierTransform();

	
	Mat directTransform(Mat &srcImg, Mat &realPart, Mat &imagPart);
	void inverseTransform(Mat& srcImg, Mat& m_dstReverse, string name);
	Mat buildSpectrum(Mat &realPart, Mat &imagPart);
	void showSpectrum(Mat& srcImg, string name);
	void swapSpectrum(Mat& magI);


	void hpButterworthFilter(Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec);
	void lpButterworthFilter(Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec);
	void showFilterResult(Mat& filteredSpectrum, string filterName);


	Mat convolution(Mat srcImg, Mat filter, string imgName, string filterName, string resultName);
	Mat correlation(Mat plate, Mat digit, string plateName, string digitName, string resultName);
	void thresholding(Mat& src, Mat& dst);

	void squareDeviation(Mat diffImg, string label);
	
	
private:
	
	Mat m_dftImg;//определено
	Mat m_dirTrImg;
	Mat m_invTrImg;
	Size m_dftSize;

	Mat m_spectrum;

	Mat m_HPFilter;
	Mat m_LPFilter;

};

