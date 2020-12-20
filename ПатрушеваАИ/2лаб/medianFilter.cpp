#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MyRobo.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/types.hpp>

using namespace std;
using namespace cv;


int median_filter::medianBlur(Mat input, Mat outputOpenCV) 
{
	//увеличиваем изображение, чтобы ядро смогло охватить каждый пиксель исходного изображения
	Mat enlargedImage(Size(input.cols + 6, input.rows + 6), input.type(), Scalar(0, 0, 0));

	//cout << "stop 1"<< endl;

	//Увеличиваем и обрабатываем края
	borderReflect101(input, enlargedImage);

	imshow("borderReflect101", enlargedImage);

	/*while (waitKey(25) != 27)
	{
		;
	}*/

	//cout << "stop 7" << endl;

	//проходи ядром
	medianFilter(enlargedImage, outputOpenCV);

	return 0;
}

int median_filter::borderReflect101(Mat input, Mat enlargedImage)
{
	//копирование оригинального изображения в центр увеличенной картинки
	for (int i = 3; i < enlargedImage.rows - 3; i++)
	{
		for (int j = 3; j < enlargedImage.cols - 3; j++)
		{
			for (int channel = 0; channel < 3; channel++)
			{
				enlargedImage.at<Vec<uint8_t, 3>>(i, j)[channel] = input.at<Vec<uint8_t, 3>>(i - 3, j - 3)[channel];
			}
		}
	}

	cout << "stop 2" << endl;

	int32_t rightSideCols = input.cols - 2 ;
	//заполнение правого края слева направо
	for (int i = 3; i < enlargedImage.rows - 3; i++)
	{
		rightSideCols = input.cols - 2;
		for (int j = enlargedImage.cols - 3; j < enlargedImage.cols; j++) 
		{
			for (int channel = 0; channel < 3; channel++)
			{
				//cout << "j= " << rightSideCols << endl;
				enlargedImage.at<Vec<uint8_t, 3>>(i, j)[channel] = input.at<Vec<uint8_t, 3>>(i - 3 , rightSideCols)[channel]; 				
			}			
			rightSideCols--;
		}
	}

	cout << "stop 3" << endl;

	int32_t leftSideCols = 6;
	for (int i = 3; i < enlargedImage.rows - 3; i++)
	{
		leftSideCols = 6;
		for (int j = 0; j < 3; j++)
		{
			for (int channel = 0; channel < 3; channel++)
			{
				enlargedImage.at<Vec<uint8_t, 3>>(i, j)[channel] = input.at<Vec<uint8_t, 3>>(i - 3, leftSideCols)[channel];
				
			}
			leftSideCols--;
		}
	}

	cout << "stop 4" << endl;

	uint8_t topSideRows = 6;

	//заполнение верха 
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < enlargedImage.cols; j++)
		{
			//topSideRows = 6;
			for (int channel = 0; channel < 3; channel++)
			{
				enlargedImage.at<Vec<uint8_t, 3>>(i, j)[channel] = enlargedImage.at<Vec<uint8_t, 3>>(6-i, j)[channel];
				
			}
		}
		//topSideRows--;
	}


	cout << "stop 5" << endl;
	int32_t bottomSideRows = enlargedImage.rows - 5;

	//заполнение низа 
	for (int i = enlargedImage.rows-4; i < enlargedImage.rows; i++)
	{
		for (int j = 0; j < enlargedImage.cols; j++)
		{
			for (int channel = 0; channel < 3; channel++)
			{
				enlargedImage.at<Vec<uint8_t, 3>>(i, j)[channel] = enlargedImage.at<Vec<uint8_t, 3>>(bottomSideRows, j)[channel];
				
			}
		}
		bottomSideRows--;
	}
	cout << "stop 6" << endl;
	

	return 0;
}



void median_filter::heapsort(uint32_t* arr, int32_t size, int32_t count)
{
	int32_t indexofMax = count;
	int32_t left = 2 * count + 1;
	int32_t right = 2 * count + 2;

	// If left child is larger than root 
	if (left<size && arr[left]>arr[indexofMax])
		indexofMax = left;
	// If right child is larger than largest		
	if (right<size && arr[right]>arr[indexofMax])
		indexofMax = right;
	// If largest is not root     
	if (indexofMax != count)
	{
		int swap = arr[indexofMax];
		arr[indexofMax] = arr[count];
		arr[count] = swap;
		heapsort(arr, size, indexofMax);
	}
}



uint32_t median_filter::getMedian(uint32_t* arr, uint32_t size)
{
	
	// построение кучи
	for (int count = (size / 2) - 1; count >= 0; count--)
		heapsort(arr, size, count);

	// Один за другим извлекаем элементы из кучи 
	for (int count = size - 1; count >= 1; count--)
	{
		// Перемещаем текущий корень в конец
		int32_t swap = arr[count];
		arr[count] = arr[0];
		arr[0] = swap;
		// вызываем max heapsort на уменьшенной куче
		heapsort(arr, count, 0);
	}
	//возвращаем медиану
	return arr[size / 2];
}




void median_filter::medianFilter(Mat input, Mat outputOpenCV) 
{
	int colsImage = input.cols;
	int rowsImage = input.rows;

	//uint32_t kernel[7][7];

	uint32_t kernel[49];

	for (int z = 0; z < 7; z++)
	{
		kernel[z] = 0;		
	}

	uint8_t counter = 0;

	uint8_t result = 0; //значение в якорной точке ядра
	cout << "stop 8" << endl;

	//проход ядром
	for (int i = 3; i < input.rows - 3; i++) //строки
	{
		//cout << i << endl;

		for (int j = 3; j < input.cols - 3; j++) //столбцы
		{
			for (int channel = 0; channel < 3; channel++) //каналы 
			{
				for (int z = 0; z < 7; z++) //проход по пикселям вокруг якорной точки 
				{
					for (int v = 0; v < 7; v++)
					{
						//kernel[z][v] = input.at<Vec<uint8_t, 3>>(i - 3 + z, j - 3 + v)[channel];

						//запись в одномерный массив для сортировки
						kernel[counter] = input.at<Vec<uint8_t, 3>>(i - 3 + z, j - 3 + v)[channel]; 
						counter++;

						//cout << "stop 9" << endl;
					}
				}				

				result = getMedian(kernel, 49); // записываем медиану в нужное место на новом изображении
				counter = 0; //сбрасываем счетчик для одномерного массива

				outputOpenCV.at<Vec<uint8_t, 3>>(i - 3, j - 3)[channel] = result; 				
				
			}
		}
	}
	return;
}


