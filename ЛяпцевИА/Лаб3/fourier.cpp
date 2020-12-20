// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Fourier.h"
using namespace std;
int specCount = 0;

void Fourier2D::swapSpektr(Mat& magI)
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

int Fourier2D::createGaussian(float sigmaX, float sigmaY)
{
	const float amplitude = 1.0f;
	Mat m_gauss = Mat(Size(m_outputImage.cols, m_outputImage.rows), CV_32F);
	for (int r = 0; r < m_outputImage.cols; r++)
	{
		for (int c = 0; c < m_outputImage.rows; c++)
		{
			const float x = (c - m_outputImage.cols / 2) * (static_cast<float>(c) - m_outputImage.cols / 2) / (2.0f *
				sigmaX * sigmaY);
			const float y = (r - m_outputImage.rows / 2) * (static_cast<float>(r) - m_outputImage.rows / 2) / (2.0f *
				sigmaX * sigmaY);
			m_gauss.at<float>(r, c) = amplitude * exp(-(x + y));
		}
	}
	normalize(m_gauss, m_gauss, 0.0f, 1.0f, NORM_MINMAX);

	Mat result;
	multiply(m_gauss, m_spectrum, result);
	Mat clone;
	clone = m_spectrum.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumAfterMultiply", clone);
	while (waitKey(0) != 27)
	{
	}
	clone = result.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumAfterMultiply", clone);
	while (waitKey(0) != 27)
	{
	}
	clone = m_gauss.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("Gauss", clone);
	while (waitKey(0) != 27)
	{
	}
	swapSpektr(result);
	clone = result.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumAfterMultiplyAndDecenter", clone);
	while (waitKey(0) != 27)
	{
	}
	//inverseFromSpectrumAndPhase(result);
	return 0;
}

//вывод спектра
int Fourier2D::spectrum()
{
	//Описание всего этого есть в документе от преподавателя.
	vector<Mat> temp;
	split(m_outputImage, temp);

	magnitude(temp[0], temp[1], m_spectrum);

	swapSpektr(m_spectrum);

	m_spectrum += Scalar::all(1);
	log(m_spectrum, m_spectrum); //Натуральный логарифм для каждого элемента

	normalize(m_spectrum, m_spectrum, 0, 1, NORM_MINMAX);
	Mat rezult;
	m_spectrum.convertTo(rezult, CV_8UC1, 255);
	const string specName = "outputSpec" + to_string(specCount);
	imshow(specName, rezult);
	specCount++;
	while (waitKey(25) != 27)
	{
	}

	return 0;
}

int Fourier2D::spectrum(Mat reAndIm, Mat spec) const
{
	//Описание всего этого есть в документе от преподавателя.
	vector<Mat> temp;
	split(reAndIm, temp);

	magnitude(temp[0], temp[1], spec);

	swapSpektr(spec);

	Mat tempSpec = spec.clone();

	tempSpec += Scalar::all(1);
	log(tempSpec, tempSpec); //Натуральный логарифм для каждого элемента

	normalize(tempSpec, tempSpec, 0, 1, NORM_MINMAX);

	Mat rezult;
	tempSpec.convertTo(rezult, CV_8UC1, 255);
	const string specName = "outputSpec" + to_string(specCount);
	imshow(specName, rezult);
	specCount++;
	while (waitKey(25) != 27)
	{
	}
	return 0;
}

//Расширение границ
Mat Fourier2D::expansion(const Mat inputImage)
{
	//Определение оптимального размера
	Size dftSize;
	dftSize.width = getOptimalDFTSize(inputImage.cols);
	dftSize.height = getOptimalDFTSize(inputImage.rows);

	const Rect rectangle(0, 0, inputImage.cols, inputImage.rows);
	Mat outputImage(dftSize, inputImage.type(), Scalar(0));

	inputImage.copyTo(outputImage(rectangle));
	return outputImage;
}

int Fourier2D::createMinusGaussian(float sigmaX, float sigmaY)
{
	const float amplitude = 1.0f;
	Mat m_gauss = Mat(Size(m_outputImage.cols, m_outputImage.rows), CV_32F);
	for (int r = 0; r < m_outputImage.cols; r++)
	{
		for (int c = 0; c < m_outputImage.rows; c++)
		{
			const float x = (c - m_outputImage.cols / 2) * (static_cast<float>(c) - m_outputImage.cols / 2) / (2.0f *
				sigmaX * sigmaY);
			const float y = (r - m_outputImage.rows / 2) * (static_cast<float>(r) - m_outputImage.rows / 2) / (2.0f *
				sigmaX * sigmaY);
			m_gauss.at<float>(r, c) = 1 - (amplitude * exp(-(x + y)));
		}
	}
	normalize(m_gauss, m_gauss, 0.0f, 1.0f, NORM_MINMAX);

	Mat result;
	multiply(m_gauss, m_spectrum, result);

	vector<Mat> temp;
	split(m_outputImage, temp);
	multiply(m_gauss, temp[0], temp[0]);
	multiply(m_gauss, temp[1], temp[1]);
	merge(temp, m_outputImage);

	Mat clone;
	clone = m_spectrum.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumBeforeMultiply", clone);
	while (waitKey(0) != 27)
	{
	}
	clone = result.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumAfterMultiply", clone);
	while (waitKey(0) != 27)
	{
	}
	clone = m_gauss.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("Gauss", clone);
	while (waitKey(0) != 27)
	{
	}
	swapSpektr(result);
	clone = result.clone();
	clone.convertTo(clone, CV_8UC1, 255);
	imshow("SpectrumAfterMultiplyAndDecenter", clone);
	while (waitKey(0) != 27)
	{
	}
	//inverseFromSpectrumAndPhase(result);
	return 0;
}

Mat Fourier2D::doCorrelation(Mat whereSearch, Mat search) const
{
	Size defaultSize = Size(whereSearch.cols, whereSearch.rows);
	whereSearch = expansion(whereSearch);
	Mat whatSearch(Size(whereSearch.cols, whereSearch.rows), CV_8UC1);
	Rect rectangle(0, 0, search.cols, search.rows);
	search.copyTo(whatSearch(rectangle));
	imshow("Where we are searching", whereSearch);
	imshow("What we are searching", whatSearch);
	while (waitKey(25) != 27)
	{
	}

	whatSearch.convertTo(whatSearch, CV_32FC1);
	whereSearch.convertTo(whereSearch, CV_32FC1);

	Mat whereSearchFourier(Size(whereSearch.cols, whereSearch.rows), CV_32FC2);
	dft(whereSearch, whereSearchFourier, DFT_COMPLEX_OUTPUT);
	//forwardTransform(whereSearch, whereSearchFourier);

	Mat whatSearchFourier(Size(whatSearch.cols, whatSearch.rows), CV_32FC2);
	dft(whatSearch, whatSearchFourier, DFT_COMPLEX_OUTPUT);
	//forwardTransform(whatSearch, whatSearchFourier);

	Mat mulFourier;
	mulSpectrums(whereSearchFourier, whatSearchFourier, mulFourier, 0, true);

	Mat resultCorr;
	dft(mulFourier, resultCorr, DFT_INVERSE | DFT_REAL_OUTPUT);
	//inverseTransform(mulFourier, rezultCorr);

	normalize(resultCorr, resultCorr, 0, 1, NORM_MINMAX);
	Mat temp = resultCorr;
	temp.convertTo(temp, CV_8UC1, 255);
	imshow("Specter", temp);
	double max;
	minMaxLoc(resultCorr, nullptr, &max);
	max -= 0.014;

	threshold(resultCorr, resultCorr, max, 0, THRESH_TOZERO);

	Mat finalResult(defaultSize, resultCorr.type());
	rectangle = Rect(0, 0, defaultSize.width, defaultSize.height);
	resultCorr(rectangle).copyTo(finalResult);
	finalResult.convertTo(finalResult, CV_8UC1, 255);
	imshow("Here it is", finalResult);

	return finalResult;
}

void Fourier2D::carNumber2(Mat& number, Mat& symbol)
{
	number.convertTo(number, CV_32FC1);
	symbol.convertTo(symbol, CV_32FC1);
	int mean1 = 0;
	for (int i = 0; i < number.rows; i++)
	{
		for (int j = 0; j < number.cols; j++)
		{
			mean1 = mean1 + number.at<float>(i, j);
		}
	}
	mean1 = mean1 / (number.rows * number.cols);
	Mat image1Mean = number - mean1;

	int mean2 = 0;
	for (int i = 0; i < symbol.rows; i++)
	{
		for (int j = 0; j < symbol.cols; j++)
		{
			mean2 = mean2 + symbol.at<float>(i, j);
		}
	}
	mean2 = mean2 / (symbol.rows * symbol.cols);
	Mat image2Mean = symbol - mean2;

	number = image1Mean.clone();
	symbol = image2Mean.clone();

	Size originalSize(number.cols, number.rows);
	expansion(number);

	Mat clone1 = number.clone();
	normalize(clone1, clone1, 0, 1, NormTypes::NORM_MINMAX);
	clone1.convertTo(clone1, CV_8UC1, 255);
	imshow("ExpandCarNumber", clone1);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat expandSymbol(Size(number.cols, number.rows), CV_32FC1, Scalar());
	Rect rect(0, 0, symbol.cols, symbol.rows);
	Mat symbolOnExpandImage(Size(number.cols, number.rows), CV_32FC1, Scalar(0));
	symbol.copyTo(symbolOnExpandImage(rect));

	Mat clone2 = symbolOnExpandImage.clone();
	normalize(clone2, clone2, 0, 1, NormTypes::NORM_MINMAX);
	clone2.convertTo(clone2, CV_8UC1, 255);
	imshow("SymbolOnExpandImage", clone2);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat carNumberAfterDFT(Size(number.cols, number.rows), CV_32FC2, Scalar());
	dft(number, carNumberAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat symbolAfterDFT(Size(number.cols, number.rows), CV_32FC2, Scalar());
	dft(symbolOnExpandImage, symbolAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat forMulSpectrums(Size(number.cols, number.rows), CV_32FC2, Scalar());
	mulSpectrums(carNumberAfterDFT, symbolAfterDFT, forMulSpectrums, 0, 1);

	Mat resultInverse(Size(number.cols, number.rows), CV_32FC1, Scalar());
	dft(forMulSpectrums, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);

	Rect rectangle1(0, 0, originalSize.width, originalSize.height);
	Mat backSize(originalSize, CV_32FC1, Scalar(0));
	resultInverse(rectangle1).copyTo(backSize);

	normalize(resultInverse, resultInverse, 0, 1, NormTypes::NORM_MINMAX);
	resultInverse.convertTo(resultInverse, CV_8UC1, 255);
	imshow("ResulMul", resultInverse);
	while (waitKey(0) != 27)
	{
		;
	}

	normalize(backSize, backSize, 0, 1, NormTypes::NORM_MINMAX);
	double maxValue = 0;
	minMaxLoc(backSize, NULL, &maxValue);
	double thresh = maxValue - 0.014;
	threshold(backSize, backSize, thresh, 0, THRESH_TOZERO);

	Point2i pt;
	for (int i = 0; i < symbol.rows; i++)
	{
		for (int j = 0; j < symbol.cols; j++)
		{
			if (backSize.at<float>(i, j) != 0)
			{
				pt.x = j;
				pt.y = i;
			}
		}
	}

	Mat clone4 = number.clone();
	Rect res(pt.x, pt.y, symbol.cols, symbol.rows);
	rectangle(clone4, res, Scalar(255), 1, 8, 0);
	normalize(clone4, clone4, 0, 1, NormTypes::NORM_MINMAX);
	clone4.convertTo(clone4, CV_8UC1, 255);
	imshow("Rect", clone4);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat clone3 = backSize.clone();
	normalize(clone3, clone3, 0, 1, NormTypes::NORM_MINMAX);
	clone3.convertTo(clone3, CV_8UC1, 255);
	imshow("Resulttt", clone3);
	while (waitKey(0) != 27)
	{
		;
	}
}