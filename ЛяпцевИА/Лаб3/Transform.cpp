// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Fourier.h"
using namespace std;

int Fourier2D::inverseFromSpectrumAndPhase(Mat& result)
{
	Mat output(Size(m_source.rows, m_source.cols), CV_32FC2);
	for (int i = 0; i < output.rows; i++)
	{
		for (int j = 0; j < output.cols; j++)
		{
			output.at<Vec2f>(i, j)[0] = result.at<float>(i, j) * cos(m_phase.at<float>(i, j));
			output.at<Vec2f>(i, j)[1] = result.at<float>(i, j) * sin(m_phase.at<float>(i, j));
		}
	}

	m_outputImage = output.clone();
	//inverseTransform();
	fastInverseTransform();

	return 0;
}

int Fourier2D::inverseTransform()
{
	if (m_outputImage.empty())
	{
		return -1;
	}

	Mat temp(Size(m_source.rows, m_source.cols), CV_32FC1);

	for (int i = 0; i < m_outputImage.rows; i++)
	{
		for (int j = 0; j < m_outputImage.cols; j++)
		{
			for (int k = 0; k < m_source.rows; k++)
			{
				for (int r = 0; r < m_source.cols; r++)
				{
					const float trigArg = (static_cast<float>(i * k) / m_outputImage.cols + static_cast<float>(j * r) /
						m_outputImage.rows) * static_cast<float>(CV_2PI);
					temp.at<float>(i, j) += m_outputImage.at<Vec2f>(k, r)[0] * cos(trigArg) - m_outputImage.at<Vec2f
						>(k, r)[1] * sin(trigArg);
				}
			}
			temp.at<float>(i, j) = temp.at<float>(i, j) / (static_cast<float>(m_outputImage.cols) * static_cast<float>(
				m_outputImage.rows));
		}
	}
	m_source = temp.clone();
	return 0;
}

int Fourier2D::forwardTransform()
{
	if (m_source.empty())
	{
		return -1;
	}

	Mat temp(Size(m_source.rows, m_source.cols), CV_32FC2);

	for (int i = 0; i < m_source.rows; i++)
	{
		for (int j = 0; j < m_source.cols; j++)
		{
			for (int k = 0; k < m_source.rows; k++)
			{
				for (int r = 0; r < m_source.cols; r++)
				{
					const double trigArg = ((static_cast<double>(i * k) / m_source.rows) + (static_cast<double>(j * r) /
						m_source.cols)) * CV_2PI;
					temp.at<Vec2f>(i, j)[0] += m_source.at<float>(k, r) * cos(trigArg);
					temp.at<Vec2f>(i, j)[1] -= m_source.at<float>(k, r) * sin(trigArg);
				}
			}
		}
	}
	spectrum(temp, m_spectrum);
	m_outputImage = temp.clone();
	m_phase = Mat(Size(m_source.rows, m_source.cols), CV_32FC1);
	for (int i = 0; i < m_source.rows; i++)
	{
		for (int j = 0; j < m_source.cols; j++)
		{
			m_phase.at<Vec<float, 1>>(i, j) = atan2(m_outputImage.at<Vec2f>(i, j)[1], m_outputImage.at<Vec2f>(i, j)[0]);
		}
	}
	return 0;
}

int Fourier2D::fastInverseTransform()
{
	
	dft(m_outputImage, m_source, DFT_INVERSE | DFT_REAL_OUTPUT);
	return 0;
}

int Fourier2D::fastForwardTransform()
{
	dft(m_source, m_outputImage, DFT_COMPLEX_OUTPUT);
	m_phase = Mat(Size(m_source.rows, m_source.cols), CV_32FC1);
	for (int i = 0; i < m_source.rows; i++)
	{
		for (int j = 0; j < m_source.cols; j++)
		{
			m_phase.at<Vec<float, 1>>(i, j) = atan2(m_outputImage.at<Vec2f>(i, j)[1], m_outputImage.at<Vec2f>(i, j)[0]);
		}
	}
	return spectrum(m_outputImage, m_spectrum);
}


