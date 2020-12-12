#pragma once
//#include <stdio.h>
//#include <math.h>
#include "opencv.hpp"


float findS(const cv::Mat img1, const cv::Mat img2, const int ch);
float compare(const cv::Mat img1, const cv::Mat img2);

class Fourier2D
{
public:

	Fourier2D();
	Fourier2D(const cv::Mat source);
	~Fourier2D();

	int setSource(const cv::Mat source);
	int getSource(cv::Mat& source);

	int getResult(cv::Mat& result);

	int forwardTransform();
	int inverseTransformFromImage();
	int inverseTransformFromSpectrePhase();

	int setImage(const cv::Mat image);
	int getImage(cv::Mat& image);

	int setImageSpectre(cv::Mat& spectre);
	int getImageSpectre(cv::Mat& spectre, int type = CV_8UC1);

	int setImagePhase(cv::Mat& phase);
	int getImagePhase(cv::Mat& phase, int type = CV_8UC1);

	int filterNotchButterworth(float cutoffFrequency, float width);
	int filterBandPassButterworth(float cutoffFrequency, float width);
	int filterSobelHorizontal();
	int filterSobelVertical();
	int filterBox();
	int filterLaplas();
	int correlation(cv::Mat img, std::string name);

private:

	cv::Mat m_source;
	cv::Mat m_image;
	cv::Mat m_spectre;
	cv::Mat m_phase;
	cv::Mat m_result;
	cv::Size m_sourceSize;

	void swapSpectre(cv::Mat& magI);
	int filterButterworth(float D0, float W, bool isNotchFilter = true);
	int forwardDFT(cv::Mat& source, cv::Mat& dst);
	int inverseDFT(cv::Mat& source, cv::Mat& dst);
	int calculateSpectre(cv::Mat& image, cv::Mat& spectre);
	int calculatePhase(cv::Mat& image, cv::Mat& phase);
	int filterConvolution(int coreType);
	int expandCanvas(cv::Mat& img32FC1, cv::Size size);
	int cutCanvas(cv::Mat& img32FC1, cv::Size size);
	int expandToOptimalSize(cv::Mat& img);
	int cutToOriginalSize(cv::Mat& img);
};