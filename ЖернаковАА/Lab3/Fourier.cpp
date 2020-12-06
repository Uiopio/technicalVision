#include "Fourier.h"

Fourier::Fourier(Mat m_inputImage) :
	m_inputImage(m_inputImage)
{
	;
}

Mat Fourier::optimalSize(Mat& image, int channels)
{
	Size dftSize;
	dftSize.width = getOptimalDFTSize(image.cols);
	dftSize.height = getOptimalDFTSize(image.rows);
	cout << dftSize.width << endl << dftSize.height << endl;
	Rect rectangle(0, 0, image.cols, image.rows);
	if (channels == 1)
	{
		Mat dftImg(dftSize, CV_32FC1, Scalar(0));
		image.copyTo(dftImg(rectangle));
		return dftImg;
	}

	if (channels == 2)
	{
		Mat dftImg(dftSize, CV_32FC2, Scalar(0));
		image.copyTo(dftImg(rectangle));
		return dftImg;
	}
}

void Fourier::forwardTransform(Mat& inputImage, Mat& outputImage)
{
	int M = inputImage.rows;
	int N = inputImage.cols;
	for (int i = 0; i < M; i++) //rows
	{
		for (int j = 0; j < N; j++) //cols
		{
			for (int x = 0; x < M; x++)
			{
				for (int y = 0; y < N; y++)
				{
					float arg = (float)CV_2PI * (((float)(i * x) / M) + ((float)(j * y) / N));
					outputImage.at<Vec2f>(i, j)[0] = outputImage.at<Vec2f>(i, j)[0] + inputImage.at<Vec<float, 1>>(x, y)[0] * cos(arg);
					outputImage.at<Vec2f>(i, j)[1] = outputImage.at<Vec2f>(i, j)[1] - inputImage.at<Vec<float, 1>>(x, y)[0] * sin(arg);
				}
			}
		}
	}
}

void Fourier::inverseTransform(Mat& inputImage, Mat& outputImage)
{
	int M = inputImage.rows;
	int N = inputImage.cols;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int x = 0; x < M; x++)
			{
				for (int y = 0; y < N; y++)
				{
					float arg = (float)CV_2PI * (((float)(i * x) / M) + ((float)(j * y) / N));
					outputImage.at<float>(i, j) += inputImage.at<Vec2f>(x, y)[0] * cos(arg) - inputImage.at<Vec2f>(x, y)[1] * sin(arg);
				}
			}
			outputImage.at<float>(i, j) = ((float)1 / (M * N)) * outputImage.at<float>(i, j);
		}
	}
}

void Fourier::inverseFromSpectrumAndPhase(Mat& resultOfMultiply, Mat& outputImage)
{
	Mat reAndim(Size(resultOfMultiply.cols, resultOfMultiply.rows), CV_32FC2, Scalar(0));
	for (int i = 0; i < resultOfMultiply.rows; i++)
	{
		for (int j = 0; j < resultOfMultiply.cols; j++)
		{
			reAndim.at<Vec2f>(i, j)[0] = resultOfMultiply.at<float>(i, j) * cos(m_phase.at<float>(i, j));
			reAndim.at<Vec2f>(i, j)[1] = resultOfMultiply.at<float>(i, j) * sin(m_phase.at<float>(i, j));
		}
	}
	Mat res(Size(resultOfMultiply.cols, resultOfMultiply.rows), CV_32FC1, Scalar(0));
	inverseTransform(reAndim, res);
	outputImage = res.clone();
}

void Fourier::spectrum(Mat& imageAfterDFT, Mat& result)
{
	vector<Mat> temp;
	split(imageAfterDFT, temp);

	Mat magn;
	magnitude(temp[0], temp[1], magn);

	swapSpektr(magn);

	magn += Scalar::all(1);
	log(magn, magn); 

	normalize(magn, magn, 0.0f, 1.0f, NormTypes::NORM_MINMAX);
	result = magn.clone();
}

void Fourier::swapSpektr(Mat& magI)
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

void Fourier::filterGaussian(Mat& inputImage, Mat& outputImage, float sigma)
{
	Size originalSize(inputImage.cols, inputImage.rows);
	inputImage = optimalSize(inputImage, 1);

	Mat gaussian(Size(inputImage.cols, inputImage.rows), CV_32F);
	float uX = inputImage.cols / 2;
	float uY = inputImage.rows / 2;
	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			float x = (((float)j - uX) * ((float)j - uX)) / (2.0f * sigma * sigma);
			float y = (((float)i - uY) * ((float)i - uY)) / (2.0f * sigma * sigma);
			float value = exp(-(x + y));
			gaussian.at<float>(i, j) = value;
		}
	}
	normalize(gaussian, gaussian, 0.0f, 1.0f, NormTypes::NORM_MINMAX);
	
	Mat imageGaussian = gaussian.clone();
	imageGaussian.convertTo(imageGaussian, CV_8UC1, 255);
	imshow("ImageGaussian", imageGaussian);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat ssspec;
	Mat gaussianAfterDFT(Size(inputImage.cols, inputImage.rows), CV_32FC2, Scalar());
	dft(gaussian, gaussianAfterDFT, DFT_COMPLEX_OUTPUT);
	spectrum(gaussianAfterDFT, ssspec);
	normalize(ssspec, ssspec, 0, 1, NormTypes::NORM_MINMAX);
	ssspec.convertTo(ssspec, CV_8UC1, 255);
	imshow("gaussianAfterDFT", ssspec);
	while (waitKey(0) != 27)
	{
		;
	}

	//”ћЌќ∆≈Ќ»≈ ‘»Ћ№“–ј Ќј ќЅ–ј« »«ќЅ–ј∆≈Ќ»я
	swapSpektr(gaussian);
	dft(inputImage, m_imageAfterDFT, DFT_COMPLEX_OUTPUT);
	Mat forMul(Size(inputImage.cols, inputImage.rows), CV_32FC2, Scalar());
	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			forMul.at<Vec2f>(i, j)[0] = m_imageAfterDFT.at<Vec2f>(i, j)[0] * gaussian.at<float>(i, j);
			forMul.at<Vec2f>(i, j)[1] = m_imageAfterDFT.at<Vec2f>(i, j)[1] * gaussian.at<float>(i, j);
		}
	}

	Mat resultInverse(Size(inputImage.cols, inputImage.rows), CV_32FC1, Scalar());
	dft(forMul, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);

	Rect rectangle(0, 0, originalSize.width, originalSize.height);
	Mat backSize(originalSize, CV_32FC1, Scalar(0));
	resultInverse(rectangle).copyTo(backSize);
	outputImage = backSize.clone();
}

void Fourier::laplace(Mat& inputImage, Mat& outputImage)
{
	Mat exInputImage(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	Rect rectangle(0, 0, inputImage.cols, inputImage.rows);
	inputImage.copyTo(exInputImage(rectangle));
	Mat imageAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(exInputImage, imageAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat laplace(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	laplace.at<float>(0, 0) = 0;
	laplace.at<float>(0, 1) = 1;
	laplace.at<float>(0, 2) = 0;
	laplace.at<float>(1, 0) = 1;
	laplace.at<float>(1, 1) = -4;
	laplace.at<float>(1, 2) = 1;
	laplace.at<float>(2, 0) = 0;
	laplace.at<float>(2, 1) = 1;
	laplace.at<float>(2, 2) = 0;
	
	Mat laplaceAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(laplace, laplaceAfterDFT, DFT_COMPLEX_OUTPUT);
	Mat spec1;
	spectrum(laplaceAfterDFT, spec1);
	normalize(spec1, spec1, 0, 1, NormTypes::NORM_MINMAX);
	spec1.convertTo(spec1, CV_8UC1, 255);
	imshow("Spec1", spec1);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat forMul(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	for (int i = 0; i < inputImage.rows + 2; i++)
	{
		for (int j = 0; j < inputImage.cols + 2; j++)
		{
			float a1 = laplaceAfterDFT.at<Vec2f>(i, j)[0];
			float b1 = laplaceAfterDFT.at<Vec2f>(i, j)[1];
			float a2 = imageAfterDFT.at<Vec2f>(i, j)[0];
			float b2 = imageAfterDFT.at<Vec2f>(i, j)[1];

			forMul.at<Vec2f>(i, j)[0] = a1 * a2 - b1 * b2;
			forMul.at<Vec2f>(i, j)[1] = a1 * b2 + a2 * b1;
		}
	}
	Mat resultInverse(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	dft(forMul, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);
	Rect rectangle2(1, 1, inputImage.cols, inputImage.rows);
	resultInverse(rectangle2).copyTo(outputImage);
	normalize(outputImage, outputImage, 0, 1, NormTypes::NORM_MINMAX);
	outputImage.convertTo(outputImage, CV_8UC1, 255);
}

void Fourier::sobel(Mat& inputImage, Mat& outputImage, int flag)
{
	Mat exInputImage(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	Rect rectangle(0, 0, inputImage.cols, inputImage.rows);
	inputImage.copyTo(exInputImage(rectangle));
	Mat imageAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(exInputImage, imageAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat sobel(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	if (flag == 0)
	{
		sobel.at<float>(0, 0) = 1;
		sobel.at<float>(0, 1) = 2;
		sobel.at<float>(0, 2) = 1;
		sobel.at<float>(1, 0) = 0;
		sobel.at<float>(1, 1) = 0;
		sobel.at<float>(1, 2) = 0;
		sobel.at<float>(2, 0) = -1;
		sobel.at<float>(2, 1) = -2;
		sobel.at<float>(2, 2) = -1;
	}
	if (flag == 1)
	{
		sobel.at<float>(0, 0) = 1;
		sobel.at<float>(0, 1) = 0;
		sobel.at<float>(0, 2) = -1;
		sobel.at<float>(1, 0) = 2;
		sobel.at<float>(1, 1) = 0;
		sobel.at<float>(1, 2) = -2;
		sobel.at<float>(2, 0) = 1;
		sobel.at<float>(2, 1) = 0;
		sobel.at<float>(2, 2) = -1;
	}

	//if (flag == 1)
	//{
	//	int uX = (inputImage.cols + 2) / 2 - 1;
	//	int uY = (inputImage.rows + 2) / 2 - 1;
	//	sobel.at<float>(uY - 1, uX - 1) = 1;
	//	sobel.at<float>(uY - 1, uX) = 0;
	//	sobel.at<float>(uY - 1, uX + 1) = -1;
	//	sobel.at<float>(uY, uX - 1) = 2;
	//	sobel.at<float>(uY, uX) = 0;
	//	sobel.at<float>(uY, uX + 1) = -2;
	//	sobel.at<float>(uY + 1, uX - 1) = 1;
	//	sobel.at<float>(uY + 1, uX) = 0;
	//	sobel.at<float>(uY + 1, uX + 1) = -1;
	//}

	Mat sobelAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(sobel, sobelAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat spec2;
	spectrum(sobelAfterDFT, spec2);
	normalize(spec2, spec2, 0, 1, NormTypes::NORM_MINMAX);
	spec2.convertTo(spec2, CV_8UC1, 255);
	imshow("Spec2", spec2);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat forMul(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	for (int i = 0; i < inputImage.rows + 2; i++)
	{
		for (int j = 0; j < inputImage.cols + 2; j++)
		{
			float a1 = sobelAfterDFT.at<Vec2f>(i, j)[0];
			float b1 = sobelAfterDFT.at<Vec2f>(i, j)[1];
			float a2 = imageAfterDFT.at<Vec2f>(i, j)[0];
			float b2 = imageAfterDFT.at<Vec2f>(i, j)[1];

			forMul.at<Vec2f>(i, j)[0] = a1 * a2 - b1 * b2;
			forMul.at<Vec2f>(i, j)[1] = a1 * b2 + a2 * b1;
		}
	}

	Mat resultInverse(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	dft(forMul, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);
	Rect rectangle2(0, 0, inputImage.cols, inputImage.rows);
	resultInverse(rectangle2).copyTo(outputImage);
	normalize(outputImage, outputImage, 0, 1, NormTypes::NORM_MINMAX);
	outputImage.convertTo(outputImage, CV_8UC1, 255);
}

void Fourier::boxFilter(Mat& inputImage, Mat& outputImage)
{
	Mat exInputImage(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	Rect rectangle(0, 0, inputImage.cols, inputImage.rows);
	inputImage.copyTo(exInputImage(rectangle));
	Mat imageAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(exInputImage, imageAfterDFT, DFT_COMPLEX_OUTPUT);

	Mat boxFilter(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			boxFilter.at<float>(i, j) = 1;
		}
	}

	Mat boxFilterAfterDFT(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	dft(boxFilter, boxFilterAfterDFT, DFT_COMPLEX_OUTPUT);
	Mat spec3;
	spectrum(boxFilterAfterDFT, spec3);
	normalize(spec3, spec3, 0, 1, NormTypes::NORM_MINMAX);
	spec3.convertTo(spec3, CV_8UC1, 255);
	imshow("Spec3", spec3);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat forMul(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC2, Scalar());
	for (int i = 0; i < inputImage.rows + 2; i++)
	{
		for (int j = 0; j < inputImage.cols + 2; j++)
		{
			float a1 = boxFilterAfterDFT.at<Vec2f>(i, j)[0];
			float b1 = boxFilterAfterDFT.at<Vec2f>(i, j)[1];
			float a2 = imageAfterDFT.at<Vec2f>(i, j)[0];
			float b2 = imageAfterDFT.at<Vec2f>(i, j)[1];

			forMul.at<Vec2f>(i, j)[0] = a1 * a2 - b1 * b2;
			forMul.at<Vec2f>(i, j)[1] = a1 * b2 + a2 * b1;
		}
	}
	Mat resultInverse(Size(inputImage.cols + 2, inputImage.rows + 2), CV_32FC1, Scalar());
	dft(forMul, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);
	Rect rectangle2(1, 1, inputImage.cols, inputImage.rows);
	resultInverse(rectangle2).copyTo(outputImage);
	normalize(outputImage, outputImage, 0, 1, NormTypes::NORM_MINMAX);
	outputImage.convertTo(outputImage, CV_8UC1, 255);
}

//нахождение символа без кросс-коррел€ции
void Fourier::carNumber(Mat& number, Mat& symbol)
{
	Size originalSize(number.cols, number.rows);
	number = optimalSize(number, 1);

	Mat clone1 = number.clone();
	normalize(clone1, clone1, 0, 1, NormTypes::NORM_MINMAX);
	clone1.convertTo(clone1, CV_8UC1, 255);
	imshow("ExpandCarNumber", clone1);
	while (waitKey(0) != 27)
	{
		;
	}

	Mat expandSymbol(Size(number.cols, number.rows), CV_32FC1, Scalar());
	Rect rectangle(0, 0, symbol.cols, symbol.rows);
	Mat symbolOnExpandImage(Size(number.cols, number.rows), CV_32FC1, Scalar(0));
	symbol.copyTo(symbolOnExpandImage(rectangle));

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

	Mat resultMul = backSize.clone();
	normalize(resultMul, resultMul, 0, 1, NormTypes::NORM_MINMAX);
	resultMul.convertTo(resultMul, CV_8UC1, 255);
	imshow("ResulMul", resultMul);
	while (waitKey(0) != 27)
	{
		;
	}

	normalize(backSize, backSize, 0, 1, NormTypes::NORM_MINMAX);
	double maxValue = 0;
	minMaxLoc(backSize, NULL, & maxValue);
	double thresh = maxValue - 0.014;
	threshold(backSize, backSize, thresh, 0, THRESH_TOZERO);

	Mat clone3 = backSize.clone();
	normalize(clone3, clone3, 0, 1, NormTypes::NORM_MINMAX);
	clone3.convertTo(clone3, CV_8UC1, 255);
	imshow("Resulttt", clone3);
	while (waitKey(0) != 27)
	{
		;
	}
}

//нахождение символа с использованием кросс-коррел€ции
void Fourier::carNumber2(Mat& number, Mat& symbol)
{
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
	number = optimalSize(number, 1);

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