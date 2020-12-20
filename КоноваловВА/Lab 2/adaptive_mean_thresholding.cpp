#pragma once

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "filter.h"

using namespace cv;
using namespace std;

AdaptiveThreshold::AdaptiveThreshold(int m_maxValue, int m_constant, int m_positionOfAnchor) :
	m_maxValue(m_maxValue),
	m_constant(m_constant),
	m_positionOfAnchor(m_positionOfAnchor)
{
	;
}

float AdaptiveThreshold::meanValue(Mat& image, int row, int col)
{
	float T = 0;
	float mean = (image.at<uint8_t>(row - 2, col - 2) + image.at<uint8_t>(row - 2, col - 1) + image.at<uint8_t>(row - 2, col) + image.at<uint8_t>(row - 2, col + 1) + image.at<uint8_t>(row - 2, col + 2) +
		image.at<uint8_t>(row - 1, col - 2) + image.at<uint8_t>(row - 1, col - 1) + image.at<uint8_t>(row - 1, col) + image.at<uint8_t>(row - 1, col + 1) + image.at<uint8_t>(row - 1, col + 2) +
		image.at<uint8_t>(row, col - 2) + image.at<uint8_t>(row, col - 1) + image.at<uint8_t>(row, col) + image.at<uint8_t>(row, col + 1) + image.at<uint8_t>(row, col + 2) +
		image.at<uint8_t>(row + 1, col - 2) + image.at<uint8_t>(row + 1, col - 1) + image.at<uint8_t>(row + 1, col) + image.at<uint8_t>(row + 1, col + 1) + image.at<uint8_t>(row + 1, col + 2) +
		image.at<uint8_t>(row + 2, col - 2) + image.at<uint8_t>(row + 2, col - 1) + image.at<uint8_t>(row + 2, col) + image.at<uint8_t>(row + 2, col + 1) + image.at<uint8_t>(row + 2, col + 2)) / 25;
	if (mean < (double)m_constant)
	{
		return -1;
	}
	else
	{
		T = mean - (double)m_constant;
		return T;
	}
}

void AdaptiveThreshold::adaptiveThreshold(Mat& inputImage, Mat& outputImage)
{
	Mat tmpImage(Size(inputImage.cols + 4, inputImage.rows + 4), inputImage.type(), Scalar(255));

	//Copy src
	for (int i = (5 / 2); i < (tmpImage.rows - (5 / 2)); i++)
	{
		for (int j = (5 / 2); j < (tmpImage.cols - (5 / 2)); j++)
		{
			tmpImage.at<uint8_t>(i, j) = inputImage.at<uint8_t>((i - (5 / 2)), (j - (5 / 2)));
		}
	}

	imshow("First", tmpImage);
	while (waitKey(0) != 27)
	{
		;
	}

	//border_reflect_101
	//left -> right -> top -> bottom
	int count = 1;

	for (int i = (5 / 2); i < (tmpImage.rows - (5 / 2)); i++)
	{
		for (int j = ((5 / 2) - 1); j >= 0; j--)
		{
			tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>(i, (j + (2 * count)));
			count++;
		}
		count = 1;
	}

	for (int i = (5 / 2); i < (tmpImage.rows - (5 / 2)); i++)
	{
		for (int j = (tmpImage.cols - (5 / 2)); j < tmpImage.cols; j++)
		{
			tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>(i, (j - (2 * count)));
			count++;
		}
		count = 1;
	}

	for (int j = 0; j < tmpImage.cols; j++)
	{
		for (int i = ((5 / 2) - 1); i >= 0; i--)
		{
			tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>((i + (2 * count)), j);
			count++;
		}
		count = 1;
	}

	for (int j = 0; j < tmpImage.cols; j++)
	{
		for (int i = (tmpImage.rows - (5 / 2)); i < tmpImage.rows; i++)
		{
			tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>((i - (2 * count)), j);
			count++;
		}
		count = 1;
	}
	imshow("Second", tmpImage);
	while (waitKey(0) != 27)
	{
		;
	}

	uint8_t rowOfAnchor = 0;
	uint8_t colOfAnchor = 0;
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
		rowOfAnchor = 0;
		colOfAnchor = 3;
		break;
	case 5:
		rowOfAnchor = 0;
		colOfAnchor = 4;
		break;
	case 6:
		rowOfAnchor = 1;
		colOfAnchor = 0;
		break;
	case 7:
		rowOfAnchor = 1;
		colOfAnchor = 1;
		break;
	case 8:
		rowOfAnchor = 1;
		colOfAnchor = 2;
		break;
	case 9:
		rowOfAnchor = 1;
		colOfAnchor = 3;
		break;
	case 10:
		rowOfAnchor = 1;
		colOfAnchor = 4;
		break;
	case 11:
		rowOfAnchor = 2;
		colOfAnchor = 0;
		break;
	case 12:
		rowOfAnchor = 2;
		colOfAnchor = 1;
		break;
	case 13:
		rowOfAnchor = 2;
		colOfAnchor = 2;
		break;
	case 14:
		rowOfAnchor = 2;
		colOfAnchor = 3;
		break;
	case 15:
		rowOfAnchor = 2;
		colOfAnchor = 4;
		break;
	case 16:
		rowOfAnchor = 3;
		colOfAnchor = 0;
		break;
	case 17:
		rowOfAnchor = 3;
		colOfAnchor = 1;
		break;
	case 18:
		rowOfAnchor = 3;
		colOfAnchor = 2;
		break;
	case 19:
		rowOfAnchor = 3;
		colOfAnchor = 3;
		break;
	case 20:
		rowOfAnchor = 3;
		colOfAnchor = 4;
		break;
	case 21:
		rowOfAnchor = 4;
		colOfAnchor = 0;
		break;
	case 22:
		rowOfAnchor = 4;
		colOfAnchor = 1;
		break;
	case 23:
		rowOfAnchor = 4;
		colOfAnchor = 2;
		break;
	case 24:
		rowOfAnchor = 4;
		colOfAnchor = 3;
		break;
	case 25:
		rowOfAnchor = 4;
		colOfAnchor = 4;
		break;
	}

	Mat result(Size(outputImage.cols + 4, outputImage.rows + 4), outputImage.type());

	for (int i = 0; i < tmpImage.rows - 4; i++) // строки
	{
		for (int j = 0; j < tmpImage.cols - 4; j++) // столбцы
		{
			if ((double)tmpImage.at<uint8_t>(i + rowOfAnchor, j + colOfAnchor) > meanValue(tmpImage, i + 2, j + 2))
			{
				result.at<uint8_t>(i + rowOfAnchor, j + colOfAnchor) = m_maxValue;
			}
			else
			{
				result.at<uint8_t>(i + rowOfAnchor, j + colOfAnchor) = 0;
			}
		}
	}

	Rect rectangle(colOfAnchor, rowOfAnchor, inputImage.cols, inputImage.rows);
	result(rectangle).copyTo(outputImage);
}
