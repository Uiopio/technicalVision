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

	//функция, кототорая вызывает расширение изображения, обработку границ и медианный фильтр
	int medianBlur(Mat input, Mat outputOpenCV); 

	//Увеличение исходного изображения и создание новых границ с помощью borderReflect101
	int borderReflect101(Mat input, Mat enlargedImage);

	//размытие с применением поиска медианы в области, охваченной ядром 
	void medianFilter(Mat input, Mat outputOpenCV); 

	void heapsort(uint32_t* arr, int32_t size, int32_t count);
	uint32_t getMedian(uint32_t* arr, uint32_t size);

};


    


    




