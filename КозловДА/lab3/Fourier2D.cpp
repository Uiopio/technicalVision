#include "Fourier2D.h"
#include <vector>
#include <math.h>
//#include <opencv2/imgproc.hpp>

Fourier2D::Fourier2D()
{
}

Fourier2D::Fourier2D(const cv::Mat source)
{
	source.convertTo(m_source, CV_32FC1);
	m_sourceSize = m_source.size();
}

Fourier2D::~Fourier2D()
{
}

int Fourier2D::setSource(const cv::Mat source)
{
	if (source.empty())
	{
		return -1;
	}

	source.convertTo(m_source, CV_32FC1);
	m_sourceSize = m_source.size();
	return 0;
}

int Fourier2D::getSource(cv::Mat& source)
{
	if (m_source.empty())
	{
		return -1;
	}

	normalize(m_source, source, 0, 1, cv::NORM_MINMAX);
	source.convertTo(source, CV_8UC1, 255);

	return 0;
}

int Fourier2D::getResult(cv::Mat& result)
{
	if (m_result.empty())
	{
		return -1;
	}

	normalize(m_result, result, 0, 1, cv::NORM_MINMAX);
	result.convertTo(result, CV_8UC1, 255);

	return 0;
}

int Fourier2D::forwardTransform()
{
	return forwardDFT(m_source, m_image);
}

int Fourier2D::inverseTransformFromImage()
{
	if (m_image.empty())
	{
		return -1;
	}

	inverseDFT(m_image, m_result);

	return 0;
}

int Fourier2D::inverseTransformFromSpectrePhase()
{
	if (m_spectre.empty())
	{
		return -1;
	}
	if (m_phase.empty())
	{
		return -2;
	}
	if (m_spectre.size() != m_phase.size())
	{
		return -3;
	}

	swapSpectre(m_spectre);

	cv::Mat tempImage(m_spectre.size(), CV_32FC2, cv::Scalar(0, 0));

	for (int x = 0; x < m_spectre.rows; x++)
	{
		for (int y = 0; y < m_spectre.cols; y++)
		{
			tempImage.at<cv::Vec2f>(x, y)[0] = m_spectre.at<float>(x, y) * cos(m_phase.at<float>(x, y));
			tempImage.at<cv::Vec2f>(x, y)[1] = m_spectre.at<float>(x, y) * sin(m_phase.at<float>(x, y));
		}
	}

	m_image = tempImage.clone();

	inverseTransformFromImage();

	swapSpectre(m_spectre);

	return 0;
}

int Fourier2D::calculateSpectre(cv::Mat& image, cv::Mat& spectre)
{
	if (image.empty())
	{
		return -1;
	}

	std::vector<cv::Mat> ReIm;
	cv::split(image, ReIm);
	magnitude(ReIm[0], ReIm[1], spectre);
	swapSpectre(spectre);

	return 0;
}

int Fourier2D::calculatePhase(cv::Mat& image, cv::Mat& phase)
{
	if (image.empty())
	{
		return -1;
	}
	
	cv::Mat tempPhase(image.size(), CV_32FC1);
	for (int i = 0; i < tempPhase.rows; i++)
	{
		for (int j = 0; j < tempPhase.cols; j++)
		{
			tempPhase.at<float>(i, j) = atan2(image.at<cv::Vec2f>(i, j)[1], m_image.at<cv::Vec2f>(i, j)[0]);
		}
	}

	phase = tempPhase.clone();

	return 0;
}

int Fourier2D::setImage(const cv::Mat image)
{
	if (image.empty())
	{
		return -1;
	}
	if (image.type() != CV_32FC2)
	{
		return -2;
	}

	m_image = image.clone();

	return 0;
}

int Fourier2D::getImage(cv::Mat& image)
{
	if (m_image.empty())
	{
		return -1;
	}

	image = m_image.clone();

	return 0;
}

int Fourier2D::setImageSpectre(cv::Mat& spectre)
{
	if (spectre.empty())
	{
		return -1;
	}

	if (spectre.type() != CV_32FC1)
	{
		spectre.convertTo(m_spectre, CV_32FC1);
	}
	else
	{
		m_spectre = spectre.clone();
	}

	return 0;
}

int Fourier2D::getImageSpectre(cv::Mat& spectre, int type)
{
	if (m_image.empty())
	{
		forwardTransform();
	}

	if (m_spectre.empty())
	{
		calculateSpectre(m_image, m_spectre);
	}

	spectre = m_spectre.clone();

	if (type == CV_32FC1)
	{
		return 0;
	}

	spectre += cv::Scalar::all(1);
	log(spectre, spectre);
	normalize(spectre, spectre, 0, 1, cv::NORM_MINMAX);
	spectre.convertTo(spectre, CV_8UC1, 255);

	return 0;
}

int Fourier2D::filterBandPassButterworth(float cutoffFrequency, float width)
{
	return filterButterworth(cutoffFrequency, width, false);
}

int Fourier2D::filterSobelHorizontal()
{
	return filterConvolution(0);
}

int Fourier2D::filterSobelVertical()
{
	return filterConvolution(1);
}

int Fourier2D::filterBox()
{
	return filterConvolution(2);
}

int Fourier2D::filterLaplas()
{
	return filterConvolution(3);
}

int Fourier2D::correlation(cv::Mat img, std::string name)
{
	if (m_source.empty() | img.empty())
	{
		return -1;
	}
	if (img.type() != CV_32FC1)
	{
		img.convertTo(img, CV_32FC1);
	}

	cv::Scalar mean1;
	cv::Mat std1;
	cv::meanStdDev(img, mean1, std1);

	cv::Scalar mean2;
	cv::Mat std2;
	cv::meanStdDev(m_source, mean2, std2);

	img -= mean1;
	m_source -= mean2;

	expandCanvas(img, m_source.size());
	forwardDFT(m_source, m_image);
	cv::Mat m_imgF;
	forwardDFT(img, m_imgF);

	m_result.convertTo(m_result, CV_32FC2);
	cv::mulSpectrums(m_image, m_imgF, m_result, 0, 1);

	inverseDFT(m_result, m_result);
	normalize(m_result, m_result, 0, 1, cv::NORM_MINMAX);

	cv::Mat m_resultWithoutThresh;
	m_result.convertTo(m_resultWithoutThresh, CV_8UC1, 255);
	cv::namedWindow("Результат без пороговой фильтрации - " + name);
	cv::imshow("Результат без пороговой фильтрации - " + name, m_resultWithoutThresh);

	double thresh;
	cv::minMaxLoc(m_result, NULL, &thresh);
	thresh -= 0.02;
	cv::threshold(m_result, m_result, thresh, 1, cv::THRESH_BINARY);
	m_result.convertTo(m_result, CV_8UC1, 255);

	return 0;
}

int Fourier2D::filterConvolution(int coreType)
{
	if (m_source.empty())
	{
		return -1;
	}
	
	forwardTransform();

	cv::Mat m_core(m_sourceSize, CV_32FC1, cv::Scalar(0));
	if (coreType == 0)
	{
		m_core.at<float>(0, 0) = 1;		m_core.at<float>(0, 1) = 2;		m_core.at<float>(0, 2) = 1;
		m_core.at<float>(1, 0) = 0;		m_core.at<float>(1, 1) = 0;		m_core.at<float>(1, 2) = 0;
		m_core.at<float>(2, 0) = -1;	m_core.at<float>(2, 1) = -2;	m_core.at<float>(2, 2) = -1;
	}
	else if (coreType == 1)
	{
		m_core.at<float>(0, 0) = 1;		m_core.at<float>(0, 1) = 0;		m_core.at<float>(0, 2) = -1;
		m_core.at<float>(1, 0) = 2;		m_core.at<float>(1, 1) = 0;		m_core.at<float>(1, 2) = -2;
		m_core.at<float>(2, 0) = 1;		m_core.at<float>(2, 1) = 0;		m_core.at<float>(2, 2) = -1;
	}
	else if (coreType == 2)
	{
		m_core.at<float>(0, 0) = 1;		m_core.at<float>(0, 1) = 1;		m_core.at<float>(0, 2) = 1;
		m_core.at<float>(1, 0) = 1;		m_core.at<float>(1, 1) = 1;		m_core.at<float>(1, 2) = 1;
		m_core.at<float>(2, 0) = 1;		m_core.at<float>(2, 1) = 1;		m_core.at<float>(2, 2) = 1;
	}
	else if (coreType == 3)
	{
		m_core.at<float>(0, 0) = 0;		m_core.at<float>(0, 1) = 1;		m_core.at<float>(0, 2) = 0;
		m_core.at<float>(1, 0) = 1;		m_core.at<float>(1, 1) = -4;	m_core.at<float>(1, 2) = 1;
		m_core.at<float>(2, 0) = 0;		m_core.at<float>(2, 1) = 1;		m_core.at<float>(2, 2) = 0;
	}

	cv::Mat m_coreImage(m_core.size(), CV_32FC2, cv::Scalar(0, 0));
	forwardDFT(m_core, m_coreImage);

	for (int x = 0; x < m_image.rows; x++)
	{
		for (int y = 0; y < m_image.cols; y++)
		{
			float a1 = m_image.at<cv::Vec2f>(x, y)[0];
			float b1 = m_image.at<cv::Vec2f>(x, y)[1];
			float a2 = m_coreImage.at<cv::Vec2f>(x, y)[0];
			float b2 = m_coreImage.at<cv::Vec2f>(x, y)[1];

			m_image.at<cv::Vec2f>(x, y)[0] = a1 * a2 - b1 * b2;
			m_image.at<cv::Vec2f>(x, y)[1] = a1 * b2 + a2 * b1;
		}
	}

	cv::Mat coreSpectre;
	calculateSpectre(m_coreImage, coreSpectre);
	coreSpectre += cv::Scalar::all(1);
	log(coreSpectre, coreSpectre);
	normalize(coreSpectre, coreSpectre, 0, 1, cv::NORM_MINMAX);
	coreSpectre.convertTo(coreSpectre, CV_8UC1, 255);
	if (coreType == 0)
	{
		cv::imshow("Спектр фильтра Собеля по горизонтали", coreSpectre);
	}
	if (coreType == 1)
	{
		cv::imshow("Спектр фильтра Собеля по вертикали", coreSpectre);
	}
	if (coreType == 2)
	{
		cv::imshow("Спектр усредняющего фильтра", coreSpectre);
	}
	if (coreType == 3)
	{
		cv::imshow("Спектр фильтра Лапласа", coreSpectre);
	}

	inverseTransformFromImage();

	cutToOriginalSize(m_source);

	return 0;
}

int Fourier2D::expandCanvas(cv::Mat& img32FC1, cv::Size size)
{
	if (img32FC1.empty())
	{
		return -1;
	}
	if (img32FC1.rows > size.height || img32FC1.cols > size.width)
	{
		return -2;
	}

	cv::Mat expandedImg(size, CV_32FC1, cv::Scalar(0));
	cv::Mat tempROI(expandedImg, cv::Rect(0, 0, img32FC1.cols, img32FC1.rows));
	img32FC1.copyTo(tempROI);

	img32FC1 = expandedImg.clone();

	return 0;
}

int Fourier2D::cutCanvas(cv::Mat& img32FC1, cv::Size size)
{
	if (img32FC1.empty())
	{
		return -1;
	}
	if (img32FC1.rows < size.height || img32FC1.cols < size.width)
	{
		return -2;
	}

	cv::Mat cuttedImg(size, CV_32FC1, cv::Scalar(0));
	cv::Mat tempROI(img32FC1, cv::Rect(0, 0, cuttedImg.cols, cuttedImg.rows));
	tempROI.copyTo(cuttedImg);

	img32FC1 = cuttedImg.clone();

	return 0;
}

int Fourier2D::expandToOptimalSize(cv::Mat& img)
{
	if (img.empty())
	{
		return -1;
	}

	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(img.cols + 1);
	dftSize.height = cv::getOptimalDFTSize(img.rows + 1);
	cv::Mat dftImg(dftSize, CV_32FC1, cv::Scalar(0));
	cv::Mat tempROI(dftImg, cv::Rect(0, 0, img.cols, img.rows));
	img.copyTo(tempROI);
	img = dftImg.clone();

	return 0;
}

int Fourier2D::cutToOriginalSize(cv::Mat& img)
{
	if (img.empty())
	{
		return -1;
	}
	if (m_sourceSize == cv::Size(0, 0) & !m_spectre.empty())
	{
		m_sourceSize = m_spectre.size();
	}
	cv::Mat origImg(m_sourceSize, CV_32FC1, cv::Scalar(0));
	cv::Mat tempROI(img, cv::Rect(0, 0, origImg.cols, origImg.rows));
	tempROI.copyTo(origImg);

	img = origImg.clone();

	return 0;
}

int Fourier2D::setImagePhase(cv::Mat& phase)
{
	if (phase.empty())
	{
		return -1;
	}

	if (phase.type() != CV_32FC1)
	{
		phase.convertTo(m_phase, CV_32FC1);
	}
	else
	{
		m_phase = phase.clone();
	}

	return 0;
}

int Fourier2D::getImagePhase(cv::Mat& phase, int type)
{
	if (m_image.empty())
	{
		forwardTransform();
	}

	if (m_phase.empty())
	{
		calculatePhase(m_image, m_phase);
	}

	phase = m_phase.clone();

	if (type == CV_8UC1)
	{
		normalize(phase, phase, 0, 255, cv::NORM_MINMAX);
		phase.convertTo(phase, CV_8UC1);
	}

	return 0;
}

int Fourier2D::filterNotchButterworth(float cutoffFrequency, float width)
{
	return filterButterworth(cutoffFrequency, width);
}

int Fourier2D::filterButterworth(float D0, float W, bool isNotchFilter)
{
	if (m_source.empty())
	{
		return -1;
	}

	forwardTransform();
	calculateSpectre(m_image, m_spectre);
	calculatePhase(m_image, m_phase);
	
	cv::Mat butterworth(m_spectre.size(), CV_32FC1, cv::Scalar(0));
	
	float x0 = butterworth.rows / 2;
	float y0 = butterworth.cols / 2;

	for (int x = 0; x < butterworth.rows; x++)
	{
		for (int y = 0; y < butterworth.cols; y++)
		{
			float D = sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0));
			if (isNotchFilter)
			{
				butterworth.at<float>(x, y) 
					= 1 / (1 + (D * W / (D * D - D0 * D0)) * (D * W / (D * D - D0 * D0)));
			}
			else
			{
				butterworth.at<float>(x, y) 
					= 1 - 1 / (1 + (D * W / (D * D - D0 * D0)) * (D * W / (D * D - D0 * D0)));
			}
		}
	}

	cv::normalize(butterworth, butterworth, 0, 1, cv::NORM_MINMAX);

	for (int x = 0; x < butterworth.rows; x++)
	{
		for (int y = 0; y < butterworth.cols; y++)
		{
			m_spectre.at<float>(x, y) *= butterworth.at<float>(x, y);
		}
	}

	butterworth.convertTo(butterworth, CV_8UC1, 255);

	if (isNotchFilter)
	{
		cv::imshow("Режекторный фильтр", butterworth);
	}
	else
	{
		cv::imshow("Полосовой фильтр", butterworth);
	}

	inverseTransformFromSpectrePhase();

	return 0;
}

int Fourier2D::forwardDFT(cv::Mat& source, cv::Mat& dst)
{
	if (source.empty())
	{
		return -1;
	}

	expandToOptimalSize(source);

	cv::Mat m_tempImageReIm(source.size(), CV_32FC2, cv::Scalar(0, 0));

	dft(source, m_tempImageReIm, cv::DFT_COMPLEX_OUTPUT);

	//for (int u = 0; u < source.rows; u++)
	//{
	//	for (int v = 0; v < source.cols; v++)
	//	{
	//		for (int x = 0; x < source.rows; x++)
	//		{
	//			for (int y = 0; y < source.cols; y++)
	//			{
	//				m_tempImageReIm.at<cv::Vec2f>(u, v)[0] += source.at<float>(x, y)
	//					* cos(2 * CV_PI * ((float)u * x / source.rows + (float)v * y / source.cols));
	//				m_tempImageReIm.at<cv::Vec2f>(u, v)[1] -= source.at<float>(x, y)
	//					* sin(2 * CV_PI * ((float)u * x / source.rows + (float)v * y / source.cols));
	//			}
	//		}
	//	}
	//}

		dst = m_tempImageReIm.clone();

		return 0;
}

int Fourier2D::inverseDFT(cv::Mat& source, cv::Mat& dst)
{
	if (source.empty())
	{
		return -1;
	}

	//cv::Mat m_tempRe(source.size(), CV_32FC1);
	//cv::Mat m_tempIm(source.size(), CV_32FC1);

	//for (int x = 0; x < source.rows; x++)
	//{
	//	for (int y = 0; y < source.cols; y++)
	//	{
	//		for (int u = 0; u < source.rows; u++)
	//		{
	//			for (int v = 0; v < source.cols; v++)
	//			{
	//				float a1 = source.at<cv::Vec2f>(u, v)[0];
	//				float a2 = cos(2 * CV_PI * ((float)x * u / source.rows + (float)y * v / source.cols));
	//				float b1 = source.at<cv::Vec2f>(u, v)[1];
	//				float b2 = sin(2 * CV_PI * ((float)x * u / source.rows + (float)y * v / source.cols));

	//				m_tempRe.at<float>(x, y) += (a1 * a2 - b1 * b2);
	//				m_tempIm.at<float>(x, y) += (a1 * b2 + a2 * b1);
	//			}
	//		}
	//		m_tempRe.at<float>(x, y) /= (source.rows * source.cols);
	//		m_tempIm.at<float>(x, y) /= (source.rows * source.cols);
	//	}
	//}

	//dst = m_tempRe.clone();

	cv::dft(source, dst, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

	cutToOriginalSize(dst);

	return 0;
}

void Fourier2D::swapSpectre(cv::Mat& magI)
{
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // верхний левый
	cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // верхний правый
	cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // нижний левый
	cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // нижний правый

	cv::Mat tmp;                           // обмен квадрантов 0-3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // обмен квадрантов 1-2
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

float findS(const cv::Mat img1, const cv::Mat img2, const int ch)
{
	if (img1.total() != img2.total())
	{
		return -1;
	}
	float sumPow2 = 0;
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			sumPow2 += (img1.at<cv::Vec2f>(i, j)[ch] - img2.at<cv::Vec2f>(i, j)[ch])
				* (img1.at<cv::Vec2f>(i, j)[ch] - img2.at<cv::Vec2f>(i, j)[ch]);
		}
	}
	return sqrt(sumPow2 / img1.total());
}

float compare(const cv::Mat img1, const cv::Mat img2)
{
	if (img1.total() != img2.total())
	{
		return -1;
	}
	float sumPow2 = 0;
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			sumPow2 += (img1.at<uchar>(i, j) - img2.at<uchar>(i, j))
				* (img1.at<uchar>(i, j) - img2.at<uchar>(i, j));
		}
	}
	return sqrt(sumPow2 / img1.total());
}
