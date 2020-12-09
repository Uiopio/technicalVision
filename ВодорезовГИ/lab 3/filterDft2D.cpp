#include "filterDft2D.h"

filterDft2D::filterDft2D()
{
	;
}

int filterDft2D::forwardDFT(Mat &src, Mat &output)
{
	if (src.empty())
	{
		return -1;
	}

	Mat srcCopy;
	src.copyTo(srcCopy);

	dft(srcCopy, output, DFT_COMPLEX_OUTPUT);									//делаем само преобразование Фурье (скв надо реализовать)

	if (output.empty())
	{
		return -1;
	}

	return 0;
}

int filterDft2D::forwardDFTCustom(Mat &src, Mat &output)
{
	if (src.empty())
	{
		return -1;
	}

	//src.convertTo(src, CV_32FC1, 1.0 / 255.0);							//переводим в float32 (0-1)

	Mat orginComplex[2] = { Mat::zeros(src.size(),  CV_32F),
							Mat::zeros(src.size(),  CV_32F) };		//создаем 2-канальный буффер из нулевых матриц

	///////////тут самописный фурье//////////////////////
	cout << "Total rows: " << src.rows << endl;

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{ 
			for (int x = 0; x < src.rows; x++)
				for (int y = 0; y < src.cols; y++)
					{
						float value = (float)CV_2PI * (((float)(i * x) / src.rows) + ((float)(j * y) / src.cols));
						orginComplex[0].at<float>(i, j) = orginComplex[0].at<float>(i, j) + src.at<float>(x, y)*cos(value);
						orginComplex[1].at<float>(i, j) = orginComplex[1].at<float>(i, j) - src.at<float>(x, y)*sin(value);
					}
			if(j==100) cout << "row = " << i << endl;
		}

	merge(orginComplex, 2, output);
	/////////////////////////////////////////////////////

	if (output.empty())
	{
		return -1;
	}
	//calculateSpecPhase(output, "origin");

	return 0;
}

int filterDft2D::showSpectrum(Mat &src, String windowLabel)
{
	if (src.empty())
	{
		return -1;
	}

	Mat srcCopy;
	srcCopy = src.clone();

	Mat splitArray[2] = { Mat::zeros(srcCopy.size(), CV_32F),
						  Mat::zeros(srcCopy.size(), CV_32F) };					//буффер
	Mat spectrum;

	split(srcCopy, splitArray);													//разделяем m_image на 2 канала 
	
	magnitude(splitArray[0], splitArray[1], spectrum);					//считаем магнитуду (ее реализовать скв)
	
	spectrum += Scalar::all(1);											//добавляем 1 и логарифмируем
	log(spectrum, spectrum);

	normalize(spectrum, spectrum, 0, 1, NormTypes::NORM_MINMAX);	//нормализируем (хз чо такое)

	swapQuadrants(spectrum);

	showImage(spectrum, windowLabel);
	return 0;
}

int filterDft2D::padImage(Mat & src, Mat & output, Size newSize)
{
	if (src.empty())
	{
		return -1;
	}
	if (newSize.height == 0 || newSize.width == 0)
	{
		return -2;
	}

	Mat outputTemp(newSize, src.type(), Scalar(0));

	Mat tempROI(outputTemp, Rect(0, 0, src.cols, src.rows));

	src.copyTo(tempROI);

	output = outputTemp;
	return 0;
}

void filterDft2D::createFilter(int filterNumber)
{
	if (filterNumber < 1 || filterNumber > 4)
	{
		cout << "wrong number of filter" << endl;
		return;
	}
	float kernelSobelVertical[9] = { 1.0, 0.0, -1.0,
									 2.0, 0.0, -2.0,
									 1.0, 0.0, -1.0 };

	float kernelSobelHorizontal[9] =  { 1.0, 2.0, 1.0,
										0.0, 0.0, 0.0,
										-1.0, -2.0, -1.0 };

	float kernelBoxFilter[9] = { 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0 };

	float kernelLaplas[9] = { 0.0, 1.0, 0.0,
							  1.0, -4.0, 1.0,
							  0.0, 1.0, 0.0 };


	if (filterNumber == 1)
	{
		Mat filterTemp(3, 3, CV_32FC1, &kernelSobelVertical);
		filterTemp.copyTo(m_filter);
	}
	else if (filterNumber == 2)
	{
		Mat filterTemp(3, 3, CV_32FC1, &kernelSobelHorizontal);
		filterTemp.copyTo(m_filter);
	}
	else if (filterNumber == 3)
	{
		Mat filterTemp(3, 3, CV_32FC1, &kernelBoxFilter);
		filterTemp.copyTo(m_filter);
	}
	else if (filterNumber == 4)
	{
		Mat filterTemp(3, 3, CV_32FC1, &kernelLaplas);
		filterTemp.copyTo(m_filter);
	}
}

int filterDft2D::swapQuadrants(Mat &src)
{
	if (src.empty())
	{
		return -1;
	}

	int centerX = src.cols / 2;
	int centerY = src.rows / 2;

	Mat q1(src, Rect(0, 0, centerX, centerY));							// верхний левый
	Mat q2(src, Rect(centerX, 0, centerX, centerY));				    // верхний правый
	Mat q3(src, Rect(0, centerY, centerX, centerY));				    // нижний левый
	Mat q4(src, Rect(centerX, centerY, centerX, centerY));				// нижний правый
	Mat buffer;

	q1.copyTo(buffer);														    //меняем 1 и 4 квадрант местами
	q4.copyTo(q1);
	buffer.copyTo(q4);

	q2.copyTo(buffer);															//меняем 2 и 3 квадрант местами
	q3.copyTo(q2);
	buffer.copyTo(q3);

	return 0;
}

int filterDft2D::showImage(Mat & src, String wndLabel)
{
	if (src.empty())
	{
		return -1;
	}

	cout << src.channels() << " chanels " << wndLabel << endl;

	if (src.type() != CV_8UC1)
	{
		Mat temp;
		src.convertTo(temp, CV_8UC1, 255);
		imshow(wndLabel, temp);
	}
	else imshow(wndLabel, src);

	return 0;
}

int filterDft2D::invertDFT(Mat &src, Mat &output)
{
	if (src.empty())
	{
		return -1;
	}

	Mat srcCopy;
	src.copyTo(srcCopy);

	dft(srcCopy, output, DFT_INVERSE | DFT_REAL_OUTPUT );

	if (output.empty())
	{
		return -1;
	}

	normalize(output, output, 0, 1, NORM_MINMAX);

	return 0;
}

int filterDft2D::invertDFTCustom(Mat &src, Mat &output)
{
	if (src.empty())
	{
		return -1;
	}

	Mat srcCopy;
	src.copyTo(srcCopy);

	output.create(srcCopy.size(), CV_32FC1);

	Mat srcComplex[2] = { Mat::zeros(srcCopy.size(), CV_32F),
						  Mat::zeros(srcCopy.size(), CV_32F) };					//буффер

	split(srcCopy, srcComplex);

	for (int i = 0; i < srcCopy.rows; i++)
		for (int j = 0; j < srcCopy.cols; j++)
		{
			for (int x = 0; x < srcCopy.rows; x++)
				for (int y = 0; y < srcCopy.cols; y++)
				{
					float value = (float)CV_2PI * (((float)(i * x) / srcCopy.rows) + ((float)(j * y) / srcCopy.cols));
					output.at<float>(i, j) += srcComplex[0].at<float>(x, y)*cos(value) - srcComplex[1].at<float>(x, y)*sin(value);
				}
			output.at<float>(i, j) = ((float)output.at<float>(i, j) / (srcCopy.rows * srcCopy.cols));
			if (j == 100) cout << "current row = " << i  << "; value = " << output.at<float>(i, j) <<endl;
		}

	showSpectrum(srcCopy, "res spec");

	

	if (output.empty())
	{
		return -1;
	}

	//normalize(output, output, 0, 1, NORM_MINMAX);
	return 0;
}

int filterDft2D::processDFT()
{
	///////////////start time counting//////////////
	auto begin = std::chrono::steady_clock::now();
	////////////////////////////////////////////////

	showImage(m_imageOrigin, "orig");
	forwardDFTCustom(m_imageOrigin, m_imageFourier);
	showSpectrum(m_imageFourier, "origin spec");
	invertDFTCustom(m_imageFourier, m_imageFiltered);
	showImage(m_imageFiltered, "res");

	Mat res;
	absdiff(m_imageOrigin, m_imageFiltered, res);
	int error = 0;
	for (int i = 0; i < res.rows; i++)
		for (int j = 0; j < res.cols; j++)
			error += res.at<float>(i, j);
	cout << "error: " << error << endl;
	showImage(res, "different");

	////////////////////////////////////////////////
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "time: " << elapsed_ms.count() << " ms\n" << endl;
	////////////////finish time counting/////////////
	return 0;
}

int filterDft2D::processDFTGaussian()
{
	const float gaussSize = 20;

	showImage(m_imageOrigin, "orig");

	///////////////start time counting//////////////
	auto begin = std::chrono::steady_clock::now();
	////////////////////////////////////////////////

	forwardDFT(m_imageOrigin, m_imageFourier);
	showSpectrum(m_imageFourier, "origin spec");

	
	createGaussian(gaussSize, gaussSize, true);
	//forwardDFT(m_gaussian, m_gaussianFourier);
	showSpectrum(m_gaussianFourier, "gaussian spec");
	showImage(m_gaussian, "gaussian without dft");
	
	convolution(m_imageFourier, m_gaussian, m_imageFilteredFourier);
	//convolution(m_imageFourier, m_gaussianFourier, m_imageFilteredFourier);
	showSpectrum(m_imageFilteredFourier, "res Fourier");
	

	invertDFT(m_imageFilteredFourier, m_imageFiltered);
	//swapQuadrants(m_imageFiltered);
	showImage(m_imageFiltered, "res");
	
	////////////////////////////////////////////////
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "time: " << elapsed_ms.count() << " ms\n" << endl;
	////////////////finish time counting/////////////

	return 0;
}

int filterDft2D::processDFTFilter(int filterNumber)
{
	showImage(m_imageOrigin, "orig");
	createFilter(filterNumber);

	int rowSize = m_imageOrigin.rows + m_filter.rows - 1;
	int colSize = m_imageOrigin.cols + m_filter.cols - 1;
	Size newSize = Size(rowSize, colSize);

	padImage(m_filter, m_filter, newSize);
	padImage(m_imageOrigin, m_imageOrigin, newSize);

	showImage(m_filter, "sobel vert");
	showImage(m_imageOrigin, "orig padded");
	
	forwardDFT(m_imageOrigin, m_imageFourier);
	forwardDFT(m_filter, m_filterFourier);

	showSpectrum(m_imageFourier, "origin spec");
	showSpectrum(m_filterFourier, "sobel fourier");

	convolutionFourier(m_imageFourier, m_filterFourier, m_imageFilteredFourier);
	showSpectrum(m_imageFilteredFourier, "res Fourier");
	
	invertDFT(m_imageFilteredFourier, m_imageFiltered);
	showImage(m_imageFiltered, "res");

	return 0;
}

int filterDft2D::processDFTCorrelation()
{
	showImage(m_imageOrigin, "car number");
	showImage(m_sign, "sign");

	/////////////////////////////////////////////////////
	//находим среднее и вычитаем из номера и символа
	Scalar Mean1;
	Mat Std1;
	meanStdDev(m_imageOrigin, Mean1, Std1);
	m_imageOrigin -= Mean1;

	Scalar Mean2;
	Mat Std2;
	meanStdDev(m_sign, Mean2, Std2);
	m_sign -= Mean2;

	normalize(m_imageOrigin, m_imageFiltered, 0, 1, NORM_MINMAX);
	normalize(m_sign, m_imageFiltered, 0, 1, NORM_MINMAX);

	cout << Mean2 << endl;
	showImage(m_imageOrigin, "car number mean");
	showImage(m_sign, "sign mean");
	/////////////////////////////////////////////////////

	padImage(m_sign, m_sign, m_imageOrigin.size());
	showImage(m_sign, "sign padded");

	forwardDFT(m_imageOrigin, m_imageFourier);
	forwardDFT(m_sign, m_signFourier);

	showSpectrum(m_imageFourier, "car number spec");
	showSpectrum(m_signFourier, "sign spec");

	mulSpectrums(m_imageFourier, m_signFourier, m_imageFilteredFourier, 0, true);
	showSpectrum(m_imageFilteredFourier, "car number correlated");

	dft(m_imageFilteredFourier, m_imageFiltered, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
	normalize(m_imageFiltered, m_imageFiltered, 0, 1, NORM_MINMAX);
	showImage(m_imageFiltered, "res");

	cv::threshold(m_imageFiltered, m_imageFiltered,1-0.05, 1, cv::THRESH_BINARY);
	showImage(m_imageFiltered, "res bin");

	return 0;
}

int filterDft2D::createGaussian(float sigmaX, float sigmaY, bool invert)
{
	if (sigmaX <= 0 || sigmaY <= 0)
	{
		return -1;
	}
	if (!m_gaussian.empty())
	{
		return -2;
		//разобраться как очищать матрицу
	}

	m_gaussian.create(m_imageFourier.size(), CV_32F);

	int uX = m_imageFourier.rows / 2;
	int uY = m_imageFourier.cols / 2;
	const int R = 5;

	for(int i=0; i< m_gaussian.rows; i++)
		for (int j = 0; j < m_gaussian.cols; j++)
		{
			float value;
			float x = pow((i - uX),2) / (2.0f*pow(sigmaX, 2));
			float y = pow((j - uY), 2) / (2.0f*pow(sigmaY, 2));
			
			if (invert)  value = 1 - exp(-(x + y));
			else value = exp(-(x + y));
			m_gaussian.at<float>(i, j) = value;	

			//////////////круговой фильтр/////////////////////////
			//if ((uX - i)*(uX - i) + (uY - j)*(uY - j) <= R*R) m_gaussian.at<float>(i, j) = 1;
			//else m_gaussian.at<float>(i, j) = 0;
		}

	normalize(m_gaussian, m_gaussian, 1.0f, 0.0f, NORM_MINMAX);

	//showImage(m_gaussian, "gaussian without dftForward");

	return 0;
}

int filterDft2D::convolution(Mat &imageFourier, Mat &filter, Mat &output)
{
	if (imageFourier.empty())
	{
		return -1;
	}
	if (filter.empty())
	{
		return -2;
	}
	if (filter.size() != imageFourier.size())
	{
		return -3;
	}

	swapQuadrants(m_gaussian);

	Mat complex[2] = { Mat(imageFourier.size(), CV_32FC1, Scalar(0)),
					   Mat(imageFourier.size(), CV_32FC1, Scalar(0)) };
	split(imageFourier, complex);

	multiplyCustom(complex[0], filter, complex[0]);
	multiplyCustom(complex[1], filter, complex[1]);

	merge(complex, 2, output);

	showImage(m_gaussian, "gaussian + swap");

	return 0;
}

int filterDft2D::convolutionFourier(Mat & imageFourier, Mat & filter, Mat & output)
{
	if (imageFourier.empty())
	{
		return -1;
	}
	if (filter.empty())
	{
		return -2;
	}
	if (filter.size() != imageFourier.size())
	{
		return -3;
	}

	mulSpectrums(imageFourier, filter, output, 0);

	return 0;
}

int filterDft2D::multiplyCustom(Mat & src1, Mat & src2, Mat & output)
{
	if (src1.empty() || src2.empty())
	{
		return -1;
	}
	if (src1.size() != src2.size())
	{
		return -2;
	}

	output.create(src1.size(), src1.type());
	for (int i = 0; i < src1.rows; i++)
		for (int j = 0; j < src1.cols; j++)
		{
			output.at<float>(i, j) = src1.at<float>(i, j)*src2.at<float>(i, j);
		}
	return 0;
}

int filterDft2D::convertSpecToComplex(Mat & spec, Mat & output)
{
	Mat complexOutput[2] = { Mat(spec.size(), CV_32FC1, Scalar(0)), Mat(spec.size(), CV_32FC1, Scalar(0))};
	for (int i = 0; i < spec.rows; i++)
		for (int j = 0; j < spec.cols; j++)
		{
			complexOutput[0].at<float>(i, j) = spec.at<float>(i, j)*cos(m_phase.at<float>(i, j));
			complexOutput[1].at<float>(i, j) = spec.at<float>(i, j)*sin(m_phase.at<float>(i, j));
		}
	merge(complexOutput,2, output);
	calculateSpecPhase(output, "new");
	return 0;
}



int filterDft2D::calculateSpecPhase(Mat & src, String label)
{
	//проверочки
	if (src.empty())
	{
		return -1;
	}
	if (src.channels()!= 2)
	{
		return -1;
	}

	//считаем спектр
	Mat splitArray[2] = { Mat::zeros(m_imageFourier.size(), CV_32F),
						  Mat::zeros(m_imageFourier.size(), CV_32F) };					//буффер

	split(src, splitArray);													//разделяем m_image на 2 канала 

	magnitude(splitArray[0], splitArray[1], m_spectr);					//считаем магнитуду (ее реализовать скв)

	m_spectr += Scalar::all(1);											//добавляем 1 и логарифмируем
	log(m_spectr, m_spectr);

	normalize(m_spectr, m_spectr, 0, 1, NormTypes::NORM_MINMAX);	//нормализируем (хз чо такое)

	//swapQuadrants(m_spectr);

	//считаем фазу
	m_phase.create(src.size(), CV_32F);
	for(int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			m_phase.at<float>(i, j) = atan2(splitArray[1].at<float>(i, j), splitArray[0].at<float>(i, j));

	//swapQuadrants(m_phase);
	//отрисовываем
	Mat spectrum8UC;
	Mat phase8UC;

	m_spectr.convertTo(spectrum8UC, CV_8UC1, 255);
	m_phase.convertTo(phase8UC, CV_8UC1, 255);

	imshow("spectr "+ label, spectrum8UC);
	imshow("phase "+ label, phase8UC);

	return 0;
}

int filterDft2D::setImage(Mat src)
{
	if (src.empty())
	{
		return -1;
	}
	src.copyTo(m_imageOrigin);
	m_imageOrigin.convertTo(m_imageOrigin, CV_32FC1, 1.0 / 255.0);
	return 0;
}

int filterDft2D::setSign(Mat src)
{
	if (src.empty())
	{
		return -1;
	}
	src.copyTo(m_sign);
	m_sign.convertTo(m_sign, CV_32FC1, 1.0 / 255.0);
	return 0;
}

Mat filterDft2D::getImageFiltered()
{
	return m_imageFiltered;
}

Mat filterDft2D::getImageOrigin()
{
	return m_imageOrigin;
}
