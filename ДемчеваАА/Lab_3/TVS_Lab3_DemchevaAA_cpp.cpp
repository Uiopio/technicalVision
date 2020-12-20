#pragma once

#include "FourierTransform.h"

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <math.h>
#include <complex>
#include <iomanip> 

using namespace cv;
using namespace std;

FourierTransform::FourierTransform()
{
	return;
}

FourierTransform::~FourierTransform()
{
	return;
}


//ОСНОВНЫЕ ФУНКЦИИ

Mat FourierTransform::directTransform(Mat& srcImg, Mat& realPart, Mat& imagPart)
{
	Mat source;
	srcImg.convertTo(source, CV_64FC1);

	//Расширение холста
	m_dftSize.width = getOptimalDFTSize(source.cols);
	m_dftSize.height = getOptimalDFTSize(source.rows);

	cout << "Optimal size:" << endl;
	cout << "width = " << m_dftSize.width << endl;
	cout << "height = " << m_dftSize.height << endl << endl;

	m_dftImg = Mat(m_dftSize, CV_64FC1, Scalar(0)); //расширенное изображение
	Mat tempROI(m_dftImg, Rect(0, 0, source.cols, source.rows)); //область интересов (ROI) холста

	source.copyTo(tempROI); //копируем исходное изображение в область интересов (ROI) холста


	//Прямое Фурье-преобразование
	Mat resImg(m_dftSize, CV_64FC2, Scalar(0));
	realPart = Mat(m_dftSize, CV_64FC1, Scalar(0));
	imagPart = Mat(m_dftSize, CV_64FC1, Scalar(0));


	int N = m_dftSize.height;
	int M = m_dftSize.width;

	double reSum = 0;
	double imSum = 0;


	for (int v = 0; v < N; v++)
	{
		for (int u = 0; u < M; u++)
		{
			for (int x = 0; x < M; x++)
			{
				for (int y = 0; y < N; y++)
				{
					double re = cos((double)CV_2PI * (((double)(u * x) / M) + ((double)(v * y) / N)));
					double im = sin((double)CV_2PI * (((double)(u * x) / M) + ((double)(v * y) / N)));

					reSum += m_dftImg.at<double>(y, x) * re;
					imSum += m_dftImg.at<double>(y, x) * -im;
				}
			}
			resImg.at<Vec2d>(v, u)[0] = reSum;
			realPart.at<double>(v, u) = reSum;
			reSum = 0;
			
			resImg.at<Vec2d>(v, u)[1] = imSum;
			imagPart.at<double>(v, u) = imSum;
			imSum = 0;
		}		
	}

	return resImg;
}

void FourierTransform::inverseTransform(Mat& srcImg, Mat& resImg, string name)
{
	resImg = Mat(m_dftSize, CV_64FC1, Scalar(0));
	

	int N = m_dftSize.height;
	int M = m_dftSize.width;

	double reSum = 0;

	for (int x = 0; x < M; x++)
	{
		for (int y = 0; y < N; y++)
		{
			for (int u = 0; u < M; u++)
			{
				for (int v = 0; v < N; v++)
				{
					double reImg = srcImg.at<Vec2d>(v, u)[0];
					double reExp = cos((double)CV_2PI * (((double)(u * x) / M) + ((double)(v * y) / N)));

					double imImg = srcImg.at<Vec2d>(v, u)[1];
					double imExp = sin((double)CV_2PI * (((double)(u * x) / M) + ((double)(v * y) / N)));

					reSum += (reImg * reExp - imImg * imExp);
				}
			}

			resImg.at<double>(y, x) = ((double)1 / (M * N)) * reSum;
			reSum = 0;
		}
	}

	normalize(resImg, resImg, 0, 255, NormTypes::NORM_MINMAX);
	resImg.convertTo(resImg, CV_8UC1);
	imshow(name, resImg);

	return;
	
}


Mat FourierTransform::buildSpectrum(Mat& realPart, Mat& imagPart)
{
	Mat resImg(m_dftSize, CV_64FC1, Scalar(0));

	int n = m_dftSize.height;
	int m = m_dftSize.width;

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			resImg.at<double>(j, i) = sqrt(pow(realPart.at<double>(j, i), 2) + pow(imagPart.at<double>(j, i), 2));
		}
	}
	return resImg;
}

void FourierTransform::showSpectrum(Mat& srcImg, string name)
{
	Mat tmpImg;
	tmpImg = srcImg.clone();
	swapSpectrum(tmpImg);

	tmpImg += Scalar::all(1);
    log(tmpImg, tmpImg);
    normalize(tmpImg, tmpImg, 0, 1, NormTypes::NORM_MINMAX);

	Mat spectrum_show;
	tmpImg.convertTo(spectrum_show, CV_8UC1, 255);
    imshow(name, spectrum_show);

	return;
}

void FourierTransform::swapSpectrum(Mat& magI)
{
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // верхний левый
	Mat q1(magI, Rect(cx, 0, cx, cy));  // верхний правый
	Mat q2(magI, Rect(0, cy, cx, cy));  // нижний левый
	Mat q3(magI, Rect(cx, cy, cx, cy)); // нижний правый

	Mat tmp;                           // обмен квадрантов 0-3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // обмен квадрантов 1-2
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

void FourierTransform::hpButterworthFilter(Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec)
{
	m_HPFilter = Mat(m_dftSize, CV_64FC1, Scalar(0));

	int N = m_dftSize.height;
	int M = m_dftSize.width;

	double D = 0;

	for (int v = 0; v < N; v++)
	{
		for (int u = 0; u < M; u++)
		{
			D = sqrt(pow((u - (M/2)) , 2) + pow((v - (N/2)) , 2));
			double denominator = pow((D * W / (pow(D, 2) - pow(cutoffD, 2))), (2 * n));
			m_HPFilter.at<double>(v, u) = (double)1 / (1 + denominator);

		}
	}
	 //Вывод изображения фильтра
	Mat filterImg;
	normalize(m_HPFilter, filterImg, 0, 255, NormTypes::NORM_MINMAX);
	filterImg.convertTo(filterImg, CV_8UC1);
	imshow("High-pass Butterworth filter", filterImg);


	//Применение фильтра к спектру
	filteredSpec = Mat(m_dftSize, CV_64FC1, Scalar(0));
	multiply(srcSpec, m_HPFilter, filteredSpec);


	//Применение фильтра к изображению
	vector<Mat> array; 
	vector<Mat> filtredArray(2);
	
	split(srcImg, array);
	multiply(array[0], m_HPFilter, filtredArray[0]);
	multiply(array[1], m_HPFilter, filtredArray[1]);
	merge(filtredArray, filteredImg);
	
	return;
}

void FourierTransform::lpButterworthFilter(Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec)
{
	m_LPFilter = Mat(m_dftSize, CV_64FC1, Scalar(0));

	int N = m_dftSize.height;
	int M = m_dftSize.width;

	double D = 0;

	for (int v = 0; v < N; v++)
	{
		for (int u = 0; u < M; u++)
		{
			D = sqrt(pow((u - (M / 2)), 2) + pow((v - (N / 2)), 2));
			double denominator = pow((D * W / (pow(D, 2) - pow(cutoffD, 2))), (2 * n));
			m_LPFilter.at<double>(v, u) = 1 - ((double)1 / (1 + denominator));

		}
	}
	//Вывод изображения фильтра
	Mat filterImg;
	normalize(m_LPFilter, filterImg, 0, 255, NormTypes::NORM_MINMAX);
	filterImg.convertTo(filterImg, CV_8UC1);
	imshow("Low-pass Butterworth filter", filterImg);


	//Применение фильтра к спектру
	filteredSpec = Mat(m_dftSize, CV_64FC1, Scalar(0));
	multiply(srcSpec, m_LPFilter, filteredSpec);


	//Применение фильтра к изображению
	vector<Mat> array;
	vector<Mat> filtredArray(2);

	split(srcImg, array);
	multiply(array[0], m_LPFilter, filtredArray[0]);
	multiply(array[1], m_LPFilter, filtredArray[1]);
	merge(filtredArray, filteredImg);

	return;
}

void FourierTransform::showFilterResult(Mat& filteredSpectrum, string filterName)
{
	//Вывод результата применения фильтра
	filteredSpectrum += Scalar::all(1);
	log(filteredSpectrum, filteredSpectrum);

	Mat filteredSpectrumShow;
	normalize(filteredSpectrum, filteredSpectrumShow, 0, 255, NormTypes::NORM_MINMAX);
	filteredSpectrumShow.convertTo(filteredSpectrumShow, CV_8UC1);
	imshow(filterName, filteredSpectrumShow);
	
	return;
}


Mat FourierTransform::convolution(Mat src, Mat filter, string imgName, string filterName, string resultName)
{
	int extendedCols = src.cols + filter.cols - 1;
	int extendedRows = src.rows + filter.rows - 1;

	Size optSize;
	optSize.width = getOptimalDFTSize(extendedCols);
	optSize.height = getOptimalDFTSize(extendedRows);

	
	//Изображение
	Mat srcImg;
	src.convertTo(srcImg, CV_64FC1);
	Mat srcExt(optSize, CV_64FC1, Scalar(0)); //копируем исходное изображение на расширенный холст
	Mat tempROI1(srcExt, Rect(0, 0, srcImg.cols, srcImg.rows)); 
	srcImg.copyTo(tempROI1); 

	Mat imgDFT;//ДПФ изображения на холсте
	dft(srcExt, imgDFT, DFT_COMPLEX_OUTPUT);

	vector<Mat> array1;//Вывод 
	split(imgDFT, array1);
	Mat imgSpectrum;
	magnitude(array1[0], array1[1], imgSpectrum);
	showSpectrum(imgSpectrum, imgName);


	//Фильтр
	Mat filterExt(optSize, CV_64FC1, Scalar(0));//копируем фильтр на расширенный холст
	Mat tempROI2(filterExt, Rect(0, 0, filter.cols, filter.rows));
	filter.copyTo(tempROI2); 

	Mat filterDFT;//ДПФ фильтра на холсте
	dft(filterExt, filterDFT, DFT_COMPLEX_OUTPUT);

	vector<Mat> array2;//Вывод 
	split(filterDFT, array2);
	Mat filterSpectrum;
	magnitude(array2[0], array2[1], filterSpectrum);
	showSpectrum(filterSpectrum, filterName);

	//Применение фильтра
	Mat multipliedSpec;
	mulSpectrums(imgDFT, filterDFT, multipliedSpec, 0, false);

	Mat convRes;
	dft(multipliedSpec, convRes, DFT_INVERSE | DFT_REAL_OUTPUT);

	//Обрезаем полученное изображение
	Mat result(srcImg.rows, srcImg.cols, CV_64FC1, Scalar(0)); //копируем исходное изображение на расширенный холст
	Mat resultROI(convRes, Rect(0, 0, srcImg.cols, srcImg.rows));
	resultROI.copyTo(result);

	normalize(result, result, 0, 255, NormTypes::NORM_MINMAX);
	result.convertTo(result, CV_8UC1);

	imshow(resultName, result);
	   
	return result;
}

Mat FourierTransform::correlation(Mat plate, Mat digitSrc, string plateName, string digitName, string resultName)
{
	int extendedCols = plate.cols + plate.cols - 1;
	int extendedRows = plate.rows + plate.rows - 1;

	Size optSize;
	optSize.width = getOptimalDFTSize(extendedCols);
	optSize.height = getOptimalDFTSize(extendedRows);


	//Номерной знак
	Mat srcImg;
	plate.convertTo(srcImg, CV_64FC1);

	Scalar MeanSrcPlate;
	Mat StdSrcPlate;
	meanStdDev(srcImg, MeanSrcPlate, StdSrcPlate);
	srcImg = srcImg - MeanSrcPlate;

	Mat srcExt(optSize, CV_64FC1, Scalar(0)); //копируем исходное изображение на расширенный холст
	Mat tempROI1(srcExt, Rect(0, 0, srcImg.cols, srcImg.rows));
	srcImg.copyTo(tempROI1);

	Mat plateDFT;//ДПФ изображения на холсте
	dft(srcExt, plateDFT, DFT_COMPLEX_OUTPUT);

	vector<Mat> array1;//Вывод 
	split(plateDFT, array1);
	Mat plateSpectrum;
	magnitude(array1[0], array1[1], plateSpectrum);
	showSpectrum(plateSpectrum, plateName);


	//Цифра
	Mat digit;
	digitSrc.convertTo(digit, CV_64FC1);

	Scalar MeanSrcDigit;
	Mat StdSrcDigit;
	meanStdDev(digit, MeanSrcDigit, StdSrcDigit);
	digit = digit - MeanSrcDigit;
	
	   
	Mat digitExt(optSize, CV_64FC1, Scalar(0));//копируем фильтр на расширенный холст
	Mat tempROI2(digitExt, Rect(0, 0, digit.cols, digit.rows));
	digit.copyTo(tempROI2);

	Mat digitDFT;//ДПФ фильтра на холсте
	dft(digitExt, digitDFT, DFT_COMPLEX_OUTPUT);

	vector<Mat> array2;//Вывод 
	split(digitDFT, array2);
	Mat digitSpectrum;
	magnitude(array2[0], array2[1], digitSpectrum);
	showSpectrum(digitSpectrum, digitName);

	//Применение фильтра
	Mat multipliedSpec;
	mulSpectrums(plateDFT, digitDFT, multipliedSpec, 0, true);

	Mat convRes;
	dft(multipliedSpec, convRes, DFT_INVERSE | DFT_REAL_OUTPUT);

	//Обрезаем полученное изображение
	Mat result(srcImg.rows, srcImg.cols, CV_64FC1, Scalar(0)); //копируем исходное изображение на расширенный холст
	Mat resultROI(convRes, Rect(0, 0, srcImg.cols, srcImg.rows));
	resultROI.copyTo(result);

	normalize(result, result, 0, 255, NormTypes::NORM_MINMAX);
		
	double max = 0;
	for (int j = 0; j < result.rows; j++)
	{
		for (int i = 0; i < result.cols; i++)
		{
			if (result.at<double>(j, i) > max)
			{
				max = result.at<double>(j, i);
			}
				
		}
	}

	double treshold = max - (double)0.04;

	Mat showResult(result.size(), CV_64FC1);

	for (int j = 0; j < result.rows; j++) // Threshold
	{
		for (int i = 0; i < result.cols; i++)
		{
			if (result.at<double>(j, i) > treshold)
			{
				showResult.at<double>(j, i) = max;
			}
			else
			{
				showResult.at<double>(j, i) = 0;
			}
		}
	}



	showResult.convertTo(showResult, CV_8UC1);

	imshow(resultName, showResult);

	return showResult;
}

void FourierTransform::thresholding(Mat& src, Mat& dst)
{
	/*Mat sample1D(src.size(), CV_64FC1);

	dst = sample1D.clone();*/

	double max = 0;
	for (int8_t j = 0; j < src.rows; j++)
	{
		for (int8_t i = 0; i < src.cols; i++)
		{
			if (src.at<double>(j, i) > max)
				max = src.at<double>(j, i);
		}
	}

	double treshold = max - (double)0.01;

	for (int j = 0; j < src.rows; j++) // Threshold
	{
		for (int i = 0; i < src.cols; i++)
		{
			if (src.at<double>(j, i) > treshold)
			{
				dst.at<double>(j, i) = max;
			}
			else
			{
				dst.at<double>(j, i) = 0;
			}
		}
	}

	return;
}

void FourierTransform::squareDeviation(Mat diffImg, string label)
{
	double diffSum = 0;
	double difSize = (diffImg.rows * diffImg.cols);

	for (int j = 0; j < diffImg.rows; j++)
	{
		for (int i = 0; i < diffImg.cols; i++)
		{
			diffSum = diffSum + pow((diffImg.at<double>(j, i)), 2);
			//cout << "diffSum = " << fixed << setprecision(5) << diffSum << endl << endl;
			//cout << "diffImg.at<double>(j, i) = " << diffImg.at<double>(j, i) << endl << endl;
			//cout << "pow((diffImg.at<double>(j, i)), 2)= " << pow((diffImg.at<double>(j, i)), 2) << endl;
		}    
	}

	//cout << "diffSum = " << fixed << setprecision(5) << diffSum << endl << endl;
//	cout << "difSize = " << fixed << setprecision(5) << difSize << endl << endl;
	//cout << "(difSize - (double)1) = " << fixed << setprecision(5) << (difSize - (double)1) << endl << endl;

	double squareDeviation = sqrt(diffSum / (difSize - (double)1));
	cout << label << endl;
	cout << "Cреднеквадратичное отклонение = " << fixed << setprecision(5) << squareDeviation << endl << endl;
	

	return;
}