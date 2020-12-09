#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "filterDft2D.h"
#include "iostream"

using namespace std;
using namespace cv;


int main() 
{
	const int part = 5;

	if (part == 1)
	{
		Mat	origin = imread("C:/openCVpictures/testImg.jpg", IMREAD_GRAYSCALE);	  //считываем монохромно

		filterDft2D filter;
		filter.setImage(origin);
		filter.processDFT();
	}
	else if (part == 2)
	{
		Mat	origin = imread("C:/openCVpictures/testImg.jpg", IMREAD_GRAYSCALE);	  //считываем монохромно

		filterDft2D filter;
		filter.setImage(origin);
		filter.processDFTGaussian();
	}
	else if (part == 3)
	{
		// 1 - Sobel vertical
		// 2 - Sobel horizontal
		// 3 - box filter
		// 4 - Laplas vertical
		const int filterNumber = 4;

		Mat	origin = imread("C:/openCVpictures/testImg.jpg", IMREAD_GRAYSCALE);	  //считываем монохромно

		filterDft2D filter;
		filter.setImage(origin);
		filter.processDFTFilter(filterNumber);
	}
	else if (part == 4)
	{
		// 1 - sign "3"
		// 2 - sign "м"
		// 3 - sign "76"
		const int signNumber = 1;

		Mat	carNumber = imread("C:/openCVpictures/carNumber.jpg", IMREAD_GRAYSCALE);
		Mat	sign;

		if (signNumber == 1 ) sign = imread("C:/openCVpictures/sign1.jpg", IMREAD_GRAYSCALE);
		else if (signNumber == 2) sign = imread("C:/openCVpictures/sign2.jpg", IMREAD_GRAYSCALE);
		else if (signNumber == 3) sign = imread("C:/openCVpictures/sign3.jpg", IMREAD_GRAYSCALE);

		//sign = imread("C:/openCVpictures/sign4.jpg", IMREAD_GRAYSCALE);

		filterDft2D filter;
		filter.setImage(carNumber);
		filter.setSign(sign);
		filter.processDFTCorrelation();
	} 
	else if (part == 5)
	{
		//Mat	carNumber = imread("C:/openCVpictures/carNumber9.jpg", IMREAD_GRAYSCALE);
		//Mat sign = imread("C:/openCVpictures/sign2.jpg", IMREAD_GRAYSCALE);

		Mat	carNumber = imread("c:/opencvpictures/carnumber5.jpg", IMREAD_GRAYSCALE);
		Mat sign = imread("c:/opencvpictures/sign A.jpg", IMREAD_GRAYSCALE);

		filterDft2D filter;
		filter.setImage(carNumber);
		filter.setSign(sign);
		filter.processDFTCorrelation();
	}
	
	while (waitKey(100) != 27);
	return 0;
}

