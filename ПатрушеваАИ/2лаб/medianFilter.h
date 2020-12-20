#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


using namespace std;
using namespace cv;


class median_filter
{
public:
	median_filter() = default;

	~median_filter() = default;

	//�������, ��������� �������� ���������� �����������, ��������� ������ � ��������� ������
	int medianBlur(Mat input, Mat outputOpenCV); 

	//���������� ��������� ����������� � �������� ����� ������ � ������� borderReflect101
	int borderReflect101(Mat input, Mat enlargedImage);

	//�������� � ����������� ������ ������� � �������, ���������� ����� 
	void medianFilter(Mat input, Mat outputOpenCV); 

	void heapsort(uint32_t* arr, int32_t size, int32_t count);
	uint32_t getMedian(uint32_t* arr, uint32_t size);

};


    


    




