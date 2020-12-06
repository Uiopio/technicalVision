#include <iostream>
#include "opencv2/core.hpp";
#include "opencv2/highgui.hpp";
#include "opencv2/imgproc.hpp";
#include "Fourier.h"
#include "time.h"

using namespace cv;
using namespace std;

int main()
{
	clock_t t;
	Mat image = imread("apple.jpg", IMREAD_GRAYSCALE);
	Mat clone = image.clone();

	imshow("Image", clone);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat src;
	clone.convertTo(src, CV_32FC1);

	Mat resultForward(Size(clone.cols, clone.rows), CV_32FC2, Scalar());
	Mat resultInverse(Size(clone.cols, clone.rows), CV_32FC1, Scalar());
	Mat resultSpectrum(Size(clone.cols, clone.rows), CV_32FC1, Scalar());
	Mat resultGaussian;
	Mat resultLaplace;
	Mat resultSobel;
	Mat resultBoxFilter;

	Fourier example(clone);
	//src = example.optimalSize(src, 1);
	//resultForward = example.optimalSize(resultForward,2);

	//// прямое 
	//example.forwardTransform(src, resultForward);

	//// обратное
	//example.inverseTransform(resultForward, resultInverse);
	//normalize(resultInverse, resultInverse, 0, 1, NormTypes::NORM_MINMAX);
	//resultInverse.convertTo(resultInverse, CV_8UC1, 255);
	//imshow("ImageAfterInverseTransform", resultInverse);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////вывод спектра
	//example.spectrum(resultForward, resultSpectrum);
	//normalize(resultSpectrum, resultSpectrum, 0, 1, NormTypes::NORM_MINMAX);
	//resultSpectrum.convertTo(resultSpectrum, CV_8UC1, 255);
	//imshow("Spectrum", resultSpectrum);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////фильтр Гаусса
	//example.filterGaussian(src, resultGaussian, 15);
	//normalize(resultGaussian, resultGaussian, 0, 1, NormTypes::NORM_MINMAX);
	//resultGaussian.convertTo(resultGaussian, CV_8UC1, 255);
	//imshow("Result", resultGaussian);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////Лаплас
	//example.laplace(src, resultLaplace);
	//imshow("ResultLaplace", resultLaplace);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////Собель
	//example.sobel(src, resultSobel, 1);
	//imshow("ResultSobel", resultSobel);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////BoxFilter
	//example.boxFilter(src, resultBoxFilter);
	//imshow("ResultBoxFilter", resultBoxFilter);
	//while (waitKey(0) != 27)
	//{
	//	;
	//}

	////Поиск символа
	//Mat carNumber = imread("carNumber.jpg", IMREAD_GRAYSCALE);
	//carNumber.convertTo(carNumber, CV_32FC1);
	//Mat cloneCarNumber = carNumber.clone();

	//Mat symbol_A = imread("symbol_A.jpg", IMREAD_GRAYSCALE);
	//symbol_A.convertTo(symbol_A, CV_32FC1);
	//Mat cloneSymbol_A = symbol_A.clone();
	//example.carNumber2(cloneCarNumber, cloneSymbol_A);

	//Mat symbol_9 = imread("symbol_9.jpg", IMREAD_GRAYSCALE);
	//symbol_9.convertTo(symbol_9, CV_32FC1);
	//Mat cloneSymbol_9 = symbol_9.clone();
	//example.carNumber2(cloneCarNumber, cloneSymbol_9);

	//Mat symbol_0 = imread("symbol_0.jpg", IMREAD_GRAYSCALE);
	//symbol_0.convertTo(symbol_0, CV_32FC1);
	//Mat cloneSymbol_0 = symbol_0.clone();
	//example.carNumber2(cloneCarNumber, cloneSymbol_0);

	Mat face = imread("face.jpg", IMREAD_GRAYSCALE); 
	face.convertTo(face, CV_32FC1);
	Mat cloneFace = face.clone();

	Mat glaza = imread("glaza.jpg", IMREAD_GRAYSCALE);
	glaza.convertTo(glaza, CV_32FC1);
	Mat cloneGlaza = glaza.clone();
	example.carNumber2(cloneFace, cloneGlaza);

	return 0;
}
