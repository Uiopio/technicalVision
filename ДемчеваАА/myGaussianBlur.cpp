#pragma once
#include "myGaussianBlur.h"
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <cmath>
#include <ctime>  
#include <vector>

using namespace cv;
using namespace std; 



myGaussianBlur::myGaussianBlur()
	{
		;
	};

myGaussianBlur::~myGaussianBlur()
{
	;
};

//Сеттеры
void myGaussianBlur::setKernel(double sigma, int kernelRows, int kernelCols)  //Заполняем ядро коэффициентами
{
	m_sigma = sigma;
	m_kernel = Mat(kernelRows, kernelCols, CV_64F);

	int i = 0;
	int j = 0;

	cout << "Ядро фильтра Гаусса: " << endl;
	
	
	for (j = 0; j < m_kernel.rows; j++)
	{
		int y = j - 3;
		for (i = 0; i < m_kernel.cols; i++)
		{
			int x = i - 3;
			m_kernel.at<double>(j, i) = (1 / (2 * CV_PI * pow(m_sigma, 2))) * exp(-(pow(x, 2) + pow(y, 2)) / (2 * pow(m_sigma, 2)));
			m_kernelSum = m_kernelSum + m_kernel.at<double>(j, i);
			cout << m_kernel.at<double>(j, i) << "   ";
		}
		cout << endl;
	}
	cout << endl << endl;
	return;
}

void myGaussianBlur::setSrcImg(Mat srcImage)
{
	m_srcImage = srcImage.clone();
}


//Геттеры

void myGaussianBlur::getSrcImg()
{
	imshow("myGauss source image", m_srcImage);
}

void myGaussianBlur::getExpandedImg()
{
	imshow("myGauss expanded image", m_extImage);
}

void myGaussianBlur::getResultImg()
{
	imshow("myGauss result image", m_resImage);
}

void myGaussianBlur::getDiffImg()
{
	imshow("difference image", m_diffImg*63);
}


void myGaussianBlur::expandImage()
{
	int myGaussRows = m_srcImage.rows + m_kernel.rows - 1;
	int myGaussCols = m_srcImage.cols + m_kernel.cols - 1;
	int myGaussType = m_srcImage.type();

	m_extImage = Mat(myGaussRows, myGaussCols, myGaussType);

	m_Dot = Point2i((m_kernel.rows / 2), (m_kernel.cols / 2));

	Size2i Size;
	Size.width = m_srcImage.cols;
	Size.height = m_srcImage.rows;
	Rect Rectangle(m_Dot, Size);
	Mat Roi(m_extImage(Rectangle));

	m_srcImage.copyTo(Roi);//Копируем в расширенное изображение исходное 


	int leftBorder = (m_kernel.cols / 2);
	int rightBorder = (m_kernel.cols / 2) + m_srcImage.cols - 1;

	int topBorder = (m_kernel.rows / 2);
	int bottomBorder = (m_kernel.rows / 2) + m_srcImage.rows - 1;

	//верхняя граница
	for (int channel = 0; channel < m_srcImage.channels(); channel++) //каналы пикселя
	{
		for (int i = leftBorder; i <= rightBorder; i++)
		{
			for (int j = (topBorder - 1); j >= 0; j--)
			{
				m_extImage.at<Vec3b>(j, i)[channel] = m_extImage.at<Vec3b>(topBorder, i)[channel];
			}
		}
	}

	//нижняя граница
	for (int channel = 0; channel < m_srcImage.channels(); channel++) //каналы пикселя
	{
		for (int i = leftBorder; i <= rightBorder; i++)
		{
			for (int j = (bottomBorder + 1); j < m_extImage.rows; j++)
			{
				m_extImage.at<Vec3b>(j, i)[channel] = m_extImage.at<Vec3b>(bottomBorder, i)[channel];
			}
		}
	}

	//левая граница
	for (int channel = 0; channel < m_srcImage.channels(); channel++) //каналы пикселя
	{
		for (int j = 0; j < m_extImage.rows; j++)
		{
			for (int i = (leftBorder - 1); i >= 0; i--)
			{
				m_extImage.at<Vec3b>(j, i)[channel] = m_extImage.at<Vec3b>(j, leftBorder)[channel];
			}
		}
	}

	//правая граница
	for (int channel = 0; channel < m_srcImage.channels(); channel++) //каналы пикселя
	{
		for (int j = 0; j < m_extImage.rows; j++)
		{
			for (int i = (rightBorder + 1); i < m_extImage.cols; i++)
			{
				m_extImage.at<Vec3b>(j, i)[channel] = m_extImage.at<Vec3b>(j, rightBorder)[channel];
			}
		}
	}



	return;
}

void myGaussianBlur::smooth()
{
	m_resImage = Mat(m_srcImage.rows, m_srcImage.cols, m_srcImage.type());

	double loc_sum = 0;
	
	int leftBorder = m_kernel.cols / 2;
	int rightBorder = m_extImage.cols - (m_kernel.cols / 2) - 1;

	int topBorder = m_kernel.rows / 2;
	int bottomBorder = m_extImage.rows - (m_kernel.rows / 2) - 1;


	int exImX = 0; //Координаты пикселя расширенного изображения
	int exImY = 0;

	int resImX = 0; //Координаты пикселя итогового изображения
	int resImY = 0;


	for (m_anchor.y = topBorder; m_anchor.y <= bottomBorder; m_anchor.y++)
	{
		for (m_anchor.x = leftBorder; m_anchor.x <= rightBorder; m_anchor.x++)
		{
			for (int channel = 0; channel < m_resImage.channels(); channel++) //каналы пикселя
			{
				exImX = m_anchor.x - (m_kernel.cols / 2); //Устанавливаем координаты левой верхней точки на изображении,
				exImY = m_anchor.y - (m_kernel.rows / 2); // к которой будет приложена левая верхняя точка маски

				for (int kerY = 0; kerY < m_kernel.rows; kerY++)
				{
					for (int kerX = 0; kerX < m_kernel.cols; kerX++)
					{
						loc_sum = loc_sum + (m_kernel.at<double>(kerY, kerX) * m_extImage.at<Vec3b>(exImY, exImX)[channel]);
						exImX++;
					}

					exImX = m_anchor.x - (m_kernel.cols / 2);//Возвращаемся в начало строки
					exImY++; //Переходим к следующей строке
				}

				resImX = m_anchor.x - leftBorder;
				resImY = m_anchor.y - topBorder;

				m_resImage.at<Vec3b>(resImY, resImX)[channel] = (uint8_t)(loc_sum/m_kernelSum);
				loc_sum = 0;
			}

		}
	}
	return;
}

void myGaussianBlur::calcDiff(Mat builtInImg)
{
	int difRows = m_srcImage.rows;
	int difCols = m_srcImage.cols;
	int difType = m_srcImage.type();

	m_diffImg = Mat(difRows, difCols, difType);

	absdiff(m_resImage, builtInImg, m_diffImg);

	return;
}

void myGaussianBlur::squareDeviation()
{
	double diffSum = 0;
	double difSize = (m_diffImg.rows * m_diffImg.cols);

	for (int channel = 0; channel < m_srcImage.channels(); channel++)
	{
		for (int j = 0; j < m_diffImg.rows; j++)
		{
			for (int i = 0; i < m_diffImg.cols; i++)
			{
				diffSum = diffSum + pow((m_diffImg.at<Vec3b>(j, i)[channel]), 2);
			}
		}

		double squareDeviation = sqrt(diffSum / (difSize-1));
		cout << "канал  " << channel << endl;
		cout << "Cреднеквадратичное отклонение = " << squareDeviation << endl;
	}

	return;
}