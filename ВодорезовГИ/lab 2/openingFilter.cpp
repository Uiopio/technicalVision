#include "openingFilter.h"

void openingFilter::addBoarder()
{
	Mat buffer(m_image.rows+2, m_image.cols+2, m_image.type());

	for (int i = 0; i < m_image.rows; i++)
		for (int j = 0; j < m_image.cols; j++)
			buffer.at<uchar>(i + 1, j + 1) = m_image.at<uchar>(i, j);

	for (int i = 0; i < m_image.rows; i++)
	{
		buffer.at<uchar>(i + 1, 0) = m_image.at<uchar>(i, 0);
		buffer.at<uchar>(i + 1, buffer.rows - 1) = m_image.at<uchar>(i, m_image.rows - 1);
	}

	for (int j = 0; j < m_image.cols; j++)
	{
		buffer.at<uchar>(0, j + 1) = m_image.at<uchar>(0, j);
		buffer.at<uchar>(buffer.cols - 1, j + 1) = m_image.at<uchar>(m_image.cols - 1, j);
	}

	m_image = buffer;
}

void openingFilter::deleteBoarder()
{
	Mat buffer(m_image.rows - 2, m_image.cols - 2, m_image.type());

	for (int i = 1; i < m_image.rows - 1; i++)
		for (int j = 1; j < m_image.cols - 1; j++)
			buffer.at<uchar>(i - 1, j - 1) = m_image.at<uchar>(i, j);
	m_image = buffer;
}

void openingFilter::erodeCustom()
{
	Mat imageFiltered(m_image.rows, m_image.cols, m_image.type());

	for (int i = 1; i < m_image.rows - 1; i++)
		for (int j = 1; j < m_image.cols - 1; j++)
		{
			int flag = 0;
			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					if (m_kernel.at<uchar>(m, n) == 255 && m_image.at<uchar>(i - 1 + m, j - 1 + n) == 0)
						flag++;
			if (flag) imageFiltered.at<uchar>(i, j) = 0;
			else imageFiltered.at<uchar>(i, j) = m_image.at<uchar>(i, j);
		}
	m_image = imageFiltered;
}

void openingFilter::dilateCustom()
{
	Mat imageFiltered(m_image.rows, m_image.cols, m_image.type());

	for (int i = 1; i < m_image.rows - 1; i++)
		for (int j = 1; j < m_image.cols - 1; j++)
		{
			int flag = 0;
			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					if (m_kernel.at<uchar>(m, n) == 255 && m_image.at<uchar>(i - 1 + m, j - 1 + n) == 255)
						flag++;
			if (flag) imageFiltered.at<uchar>(i, j) = 255;
			else imageFiltered.at<uchar>(i, j) = m_image.at<uchar>(i, j);
		}
	m_image = imageFiltered;
}

void openingFilter::erodeOpencv()
{
	 cv::erode(m_image, m_image, m_kernel);
	
}

void openingFilter::dilateOpencv()
{
	cv::dilate(m_image, m_image, m_kernel);
}

void openingFilter::openingCustom(Mat src, Mat kernel)
{
	setKernel(kernel);
	setImage(src);
	addBoarder();
	erodeCustom();
	dilateCustom();
	deleteBoarder();
}

void openingFilter::openingOpenCV(Mat src, Mat kernel)
{
	morphologyEx(src, m_image, MORPH_OPEN, kernel, Point(-1,-1), 1,BORDER_REFLECT);
}

void openingFilter::setImage(Mat src)
{
	m_image.create(src.rows, src.cols, src.type());
	m_image = src;
}

void openingFilter::setKernel(Mat kernel)
{
	m_kernel = kernel;
}

Mat openingFilter::getImage()
{
	return m_image;
}

Mat openingFilter::getKernel()
{
	return m_kernel;
}
