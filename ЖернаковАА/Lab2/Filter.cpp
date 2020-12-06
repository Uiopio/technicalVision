#include <iostream>
#include "opencv2/core.hpp";
#include "opencv2/highgui.hpp";
#include "opencv2/imgproc.hpp";
#include "Filter.h"

using namespace cv;
using namespace std;

AdaptiveThreshold::AdaptiveThreshold(int m_maxValue, int m_constant, int m_positionOfAnchor) :
	m_maxValue(m_maxValue),
	m_constant(m_constant),
	m_positionOfAnchor(m_positionOfAnchor)
{
	;
}

double AdaptiveThreshold::meanValue(Mat& image, int row, int col)
{
	double mean = 0;
	mean = (image.at<uchar>(row - 1, col - 1) + image.at<uchar>(row - 1, col) + image.at<uchar>(row - 1, col + 1) +
			image.at<uchar>(row, col - 1) + image.at<uchar>(row, col) + image.at<uchar>(row, col + 1) +
			image.at<uchar>(row + 1, col - 1) + image.at<uchar>(row + 1, col) + image.at<uchar>(row + 1, col + 1)) / 9 - (double)m_constant;
	return mean;
}

void AdaptiveThreshold::adaptiveThreshold(Mat& inputImage, Mat& outputImage)
{
	int rowOfAnchor = 0;
	int colOfAnchor = 0;
	switch (m_positionOfAnchor)
	{
	case 1:
		rowOfAnchor = 0;
		colOfAnchor = 0;
		break;
	case 2:
		rowOfAnchor = 0;
		colOfAnchor = 1;
		break;
	case 3:
		rowOfAnchor = 0;
		colOfAnchor = 2;
		break;
	case 4:
		rowOfAnchor = 1;
		colOfAnchor = 0;
		break;
	case 5:
		rowOfAnchor = 1;
		colOfAnchor = 1;
		break;
	case 6:
		rowOfAnchor = 1;
		colOfAnchor = 2;
		break;
	case 7:
		rowOfAnchor = 2;
		colOfAnchor = 0;
		break;
	case 8:
		rowOfAnchor = 2;
		colOfAnchor = 1;
		break;
	case 9:
		rowOfAnchor = 2;
		colOfAnchor = 2;
		break;
	}

	Mat cloneOfImg = inputImage.clone();

	Mat bigClone(Size(cloneOfImg.cols + 2, cloneOfImg.rows + 2), cloneOfImg.type());
	Rect rectangle(colOfAnchor, rowOfAnchor, cloneOfImg.cols, cloneOfImg.rows);
	cloneOfImg.copyTo(bigClone(rectangle));
	
	Mat result(Size(outputImage.cols + 2, outputImage.rows + 2), outputImage.type());

	for (int i = 0; i < bigClone.rows - 2; i++) // строки
	{
		for (int j = 0; j < bigClone.cols - 2 ; j++) // столбцы
		{
			if ((double)bigClone.at<uchar>(i + rowOfAnchor, j + colOfAnchor) > meanValue(bigClone, i + 1, j + 1))
			{
				result.at<uchar>(i + rowOfAnchor, j + colOfAnchor) = m_maxValue;
			}
			else
			{
				result.at<uchar>(i + rowOfAnchor, j + colOfAnchor) = 0;
			}
		}
	}

	Rect rectangle2(colOfAnchor, rowOfAnchor, cloneOfImg.cols, cloneOfImg.rows);
	result(rectangle2).copyTo(outputImage);
}