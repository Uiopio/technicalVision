#pragma once

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

class MedianFilter
{
public:
	MedianFilter();
	MedianFilter(Mat src);

	~MedianFilter();

	//Рукописный фильтр (ядро 3х3, якорь в левом нижнем углу)
	void filteringWithMyAnchor();

	//Рукописный фильтр с ядром 3х3 и якорем по центру (для сравнения изображений)
	void filteringWithMiddleAnchor();

	//Рукописный фильтр (ядро 3х3, якорь в правом верхнем углу)
	void filteringWithOtherAnchor();

	//Стандартный фильтр OpenCV
	void filteringWithOpenCV();

	//Фильтр Гаусса
	void gaussFilter();

	//Разница между стандартным и рукописным фильтром (в пикселях)
	void showDifference(Mat src1, Mat src2);

	//Геттеры
	Mat getImageHW();
	Mat getImageHW2();
	Mat getImageCV();
	Mat getImageGauss();

private:
	Mat m_srcImage;
	Mat m_dstImageHW;
	Mat m_dstImageHW2;
	Mat m_dstImageCV;
	Mat m_dstImageGauss;
	int m_window[9];
};