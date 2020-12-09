#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <chrono>

using namespace std;
using namespace cv;

class filterDft2D
{
public:
	filterDft2D();

	int forwardDFT(Mat &src, Mat &output);
	int forwardDFTCustom(Mat &src, Mat &output);

	int invertDFT(Mat &src, Mat &output);
	int invertDFTCustom(Mat &src, Mat &output);

	int processDFT(); //1 часть
	int processDFTGaussian();  //2 часть
	int processDFTFilter(int filterNumber);	//3 часть
	int processDFTCorrelation(); //4 часть


	int createGaussian(float sigmaX, float sigmaY, bool invert = false);
	int convolution(Mat &imageFourier, Mat &filter, Mat &output); //свертка
	int convolutionFourier(Mat &imageFourier, Mat &filter, Mat &output); //свертка
	int multiplyCustom(Mat &src1, Mat &src2, Mat &output);


	int calculateSpecPhase(Mat &src, String label);

	int setImage(Mat src);
	int setSign(Mat src);
	Mat getImageFiltered();

	int showSpectrum(Mat &src, String windowLabel);

	int padImage(Mat &src, Mat &output, Size newSize);

	void createFilter(int filterNumber);

	//not used
	Mat getImageOrigin();
	int convertSpecToComplex(Mat &spec, Mat &output);

	
private:
	Mat m_imageOrigin;
	Mat m_gaussian;
	Mat m_imageFiltered;

	Mat m_phase;
	Mat m_spectr;

	Mat m_imageFourier;
	Mat m_gaussianFourier;
	Mat m_imageFilteredFourier;
	
	int swapQuadrants(Mat &src);
	int showImage(Mat &src, String wndLabel);

	Mat m_filter;
	Mat m_filterFourier;

	Mat m_sign;
	Mat m_signFourier;
	
};

