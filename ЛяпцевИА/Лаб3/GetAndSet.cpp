// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "fourier.h"
#include "opencv.hpp"
#include <ctime>
using namespace std;

int Fourier2D::setSource(const string sourceName)
{
	if (sourceName.empty())
	{
		return -1;
	}

	m_source = imread(sourceName, IMREAD_GRAYSCALE);
	m_inputSize = Size(m_source.cols, m_source.rows);
	imshow("m_source", m_source);
	while (waitKey(0) != 27)
	{
	}
	m_source.convertTo(m_source, CV_32FC1);
	m_source = expansion(m_source);
	return 0;
}

int Fourier2D::setFourierImage(const Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	m_outputImage = image;
	return 0;
}

int Fourier2D::getFourierImage(Mat& image) const
{
	if (m_outputImage.empty())
	{
		return -1;
	}

	image = m_outputImage;
	return 0;
}

int Fourier2D::setImageSpectre(const Mat spectre)
{
	if (spectre.empty())
	{
		return -1;
	}

	m_spectrum = spectre;
	return 0;
}

int Fourier2D::getImageSpectre(Mat& spectre) const
{
	if (m_spectrum.empty())
	{
		return -1;
	}

	spectre = m_spectrum;
	return 0;
}

int Fourier2D::setImagePhase(const Mat phase)
{
	if (phase.empty())
	{
		return -1;
	}

	m_phase = phase;
	return 0;
}

int Fourier2D::getImagePhase(Mat& phase) const
{
	if (m_phase.empty())
	{
		return -1;
	}

	phase = m_phase;
	return 0;
}

int Fourier2D::getSource(Mat& source) const
{
	if (m_source.empty())
	{
		return -1;
	}
	Mat finalResult(m_inputSize, m_source.type(), Scalar(0));
	const Rect rectangle = Rect(0, 0, m_inputSize.width, m_inputSize.height);
	m_source(rectangle).copyTo(finalResult);
	source = finalResult;
	return 0;
}
