#include "median_filter.h"
#include "timer.h"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main()
{
	//Исходное изображение
	Mat imageBefore = cv::imread("D:/Programs/OpenCV/Projects/Lab_2/image_examples/photo.jpg", IMREAD_GRAYSCALE);
	//cv::Size imageSize(800, 600);
	//cv::resize(imageBefore, imageBefore, imageSize);
	//Фильтрация и замер времени
	MedianFilter filter(imageBefore);

	//Начало замера времени
	Timer t;

	filter.filteringWithMyAnchor();
	filter.filteringWithOtherAnchor();
	//filter.filteringWithMiddleAnchor();
	filter.filteringWithOpenCV();

	//Конец замера времени
	std::cout << "Time: " << t.elapsed() << endl;

	//Вывод изображения
	cv::imshow("Photo before filter", imageBefore);
	cv::imshow("Photo after handwritten filter", filter.getImageHW());
	cv::imshow("Photo after other handwritten filter", filter.getImageHW2());
	cv::imshow("Photo after OpenCV filter", filter.getImageCV());
	//cv::imshow("Photo after Gauss filter", filter.getImageGauss());

	//Сравнение
	filter.showDifference(filter.getImageCV(), filter.getImageHW());
	while (waitKey(1) != 27)
	{
		;
	}
	filter.showDifference(filter.getImageCV(), filter.getImageHW2());

	while (waitKey(1) != 27)
	{
		;
	}
}

MedianFilter::MedianFilter()
{
	;
}

MedianFilter::MedianFilter(Mat src):
	m_srcImage(src),
	m_dstImageCV(src.rows, src.cols, src.type()),
	m_dstImageHW(src.rows, src.cols, src.type()),
	m_dstImageHW2(src.rows, src.cols, src.type()),
	m_dstImageGauss(src.rows, src.cols, src.type())
{
	;
}

MedianFilter::~MedianFilter()
{
	;
}

//Рукописный фильтр (ядро 3х3, якорь в левом нижнем углу)
void MedianFilter::filteringWithMyAnchor()
{
	for (int image_rows = 2; image_rows < m_srcImage.rows; image_rows++)
	{
		for (int image_cols = 0; image_cols < m_srcImage.cols - 2; image_cols++)
		{
			int window[9] = { 0 };

			window[0] = m_srcImage.at<uchar>(image_rows - 2, image_cols);
			window[1] = m_srcImage.at<uchar>(image_rows - 2, image_cols + 1);
			window[2] = m_srcImage.at<uchar>(image_rows - 2, image_cols + 2);
			window[3] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
			window[4] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
			window[5] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 2);
			window[6] = m_srcImage.at<uchar>(image_rows, image_cols);
			window[7] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
			window[8] = m_srcImage.at<uchar>(image_rows, image_cols + 2);

			std::sort(std::begin(window), std::end(window));
			m_dstImageHW.at<uchar>(image_rows, image_cols) = window[4];
		}
	}
}

//Рукописный фильтр (для испытаний)
void MedianFilter::filteringWithOtherAnchor()
{
	//
	//for (int image_rows = 0; image_rows < m_srcImage.rows; image_rows++)
	//{
	//	for (int image_cols = 1; image_cols < m_srcImage.cols - 1; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
	//		window[1] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[2] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
	//		window[3] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 1);
	//		window[4] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
	//		window[5] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 1);
	//		window[6] = m_srcImage.at<uchar>(image_rows + 2, image_cols - 1);
	//		window[7] = m_srcImage.at<uchar>(image_rows + 2, image_cols);
	//		window[8] = m_srcImage.at<uchar>(image_rows + 2, image_cols + 1);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}
	//
	//for (int image_rows = 1; image_rows < m_srcImage.rows - 1; image_rows++)
	//{
	//	for (int image_cols = 0; image_cols < m_srcImage.cols; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
	//		window[1] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 2);
	//		window[3] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
	//		window[4] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 2);
	//		window[6] = m_srcImage.at<uchar>(image_rows + 2, image_cols);
	//		window[7] = m_srcImage.at<uchar>(image_rows + 2, image_cols + 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows + 2, image_cols + 2);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}
	
	//for (int image_rows = 2; image_rows < m_srcImage.rows - 2; image_rows++)
	//{
	//	for (int image_cols = 1; image_cols < m_srcImage.cols - 1; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows - 2, image_cols - 1);
	//		window[1] = m_srcImage.at<uchar>(image_rows - 2, image_cols);
	//		window[2] = m_srcImage.at<uchar>(image_rows - 2, image_cols + 1);
	//		window[3] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 1);
	//		window[4] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
	//		window[5] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
	//		window[6] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
	//		window[7] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[8] = m_srcImage.at<uchar>(image_rows, image_cols + 1);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}

	//for (int image_rows = 1; image_rows < m_srcImage.rows - 1; image_rows++)
	//{
	//	for (int image_cols = 2; image_cols < m_srcImage.cols; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 2);
	//		window[1] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
	//		window[3] = m_srcImage.at<uchar>(image_rows, image_cols - 2);
	//		window[4] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[6] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 2);
	//		window[7] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows + 1, image_cols);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}

	//for (int image_rows = 2; image_rows < m_srcImage.rows - 2; image_rows++)
	//{
	//	for (int image_cols = 2; image_cols < m_srcImage.cols - 2; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows - 2, image_cols - 2);
	//		window[1] = m_srcImage.at<uchar>(image_rows - 2, image_cols - 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows - 2, image_cols);
	//		window[3] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 2);
	//		window[4] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
	//		window[6] = m_srcImage.at<uchar>(image_rows, image_cols - 2);
	//		window[7] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows, image_cols);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}

	//for (int image_rows = 2; image_rows < m_srcImage.rows - 2; image_rows++)
	//{
	//	for (int image_cols = 0; image_cols < m_srcImage.cols; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows - 2, image_cols);
	//		window[1] = m_srcImage.at<uchar>(image_rows - 2, image_cols + 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows - 2, image_cols + 2);
	//		window[3] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
	//		window[4] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 2);
	//		window[6] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[7] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows, image_cols + 2);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}

	//for (int image_rows = 0; image_rows < m_srcImage.rows; image_rows++)
	//{
	//	for (int image_cols = 2; image_cols < m_srcImage.cols - 2; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows, image_cols - 2);
	//		window[1] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[3] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 2);
	//		window[4] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
	//		window[6] = m_srcImage.at<uchar>(image_rows + 2, image_cols - 2);
	//		window[7] = m_srcImage.at<uchar>(image_rows + 2, image_cols - 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows + 2, image_cols);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW2.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}

	//for (int image_rows = 0; image_rows < m_srcImage.rows; image_rows++)
	//{
	//	for (int image_cols = 0; image_cols < m_srcImage.cols; image_cols++)
	//	{
	//		int window[9] = { 0 };

	//		window[0] = m_srcImage.at<uchar>(image_rows, image_cols);
	//		window[1] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
	//		window[2] = m_srcImage.at<uchar>(image_rows, image_cols + 2);
	//		window[3] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
	//		window[4] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 1);
	//		window[5] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 2);
	//		window[6] = m_srcImage.at<uchar>(image_rows + 2, image_cols);
	//		window[7] = m_srcImage.at<uchar>(image_rows + 2, image_cols + 1);
	//		window[8] = m_srcImage.at<uchar>(image_rows + 2, image_cols + 2);

	//		std::sort(std::begin(window), std::end(window));
	//		m_dstImageHW.at<uchar>(image_rows, image_cols) = window[4];
	//	}
	//}


	for (int image_rows = 1; image_rows < m_srcImage.rows - 1; image_rows++)
	{
		for (int image_cols = 1; image_cols < m_srcImage.cols - 1; image_cols++)
		{
			int window[9] = { 0 };

			window[0] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 1);
			window[1] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
			window[2] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
			window[3] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
			window[4] = m_srcImage.at<uchar>(image_rows, image_cols);
			window[5] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
			window[6] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 1);
			window[7] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
			window[8] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 1);

			std::sort(std::begin(window), std::end(window));
			m_dstImageHW2.at<uchar>(image_rows - 1, image_cols - 1) = window[4];
			m_dstImageHW2.at<uchar>(image_rows - 1, image_cols + 1) = window[4];
			m_dstImageHW2.at<uchar>(image_rows + 1, image_cols - 1) = window[4];
			m_dstImageHW2.at<uchar>(image_rows + 1, image_cols + 1) = window[4];
		}
	}

}

//Рукописный фильтр с ядром 3х3 и якорем по центру (для сравнения изображений)
void MedianFilter::filteringWithMiddleAnchor()
{
	for (int image_rows = 1; image_rows < m_srcImage.rows - 1; image_rows++)
	{
		for (int image_cols = 1; image_cols < m_srcImage.cols - 1; image_cols++)
		{
			int window[9] = { 0 };

			window[0] = m_srcImage.at<uchar>(image_rows - 1, image_cols - 1);
			window[1] = m_srcImage.at<uchar>(image_rows - 1, image_cols);
			window[2] = m_srcImage.at<uchar>(image_rows - 1, image_cols + 1);
			window[3] = m_srcImage.at<uchar>(image_rows, image_cols - 1);
			window[4] = m_srcImage.at<uchar>(image_rows, image_cols);
			window[5] = m_srcImage.at<uchar>(image_rows, image_cols + 1);
			window[6] = m_srcImage.at<uchar>(image_rows + 1, image_cols - 1);
			window[7] = m_srcImage.at<uchar>(image_rows + 1, image_cols);
			window[8] = m_srcImage.at<uchar>(image_rows + 1, image_cols + 1);

			std::sort(std::begin(window), std::end(window));
			m_dstImageHW.at<uchar>(image_rows, image_cols) = window[4];
		}
	}
}

//Пробы фильтра Гаусса
void MedianFilter::gaussFilter()
{
	//int u = 0;
	//int sigm = 1;
	//for (int image_rows = 1; image_rows < m_srcImage.rows - 1; image_rows++)
	//{
	//	for (int image_cols = 1; image_cols < m_srcImage.cols - 1; image_cols++)
	//	{

	//		for (int i = image_rows - 1; i <= image_rows + 1; i++)
	//		{
	//			for (int i = image_cols - 1; i <= image_cols + 1; i++)
	//			{
	//				float gauss = (1 / (sigm * 2.5)) * exp((image_cols ^ 2 + image_rows ^ 2)/(2 * sigm ^ 2));
	//				m_dstImageGauss.at<uchar>(image_rows, image_cols) = gauss;
	//			}
	//		}

	//	}
	//}

}

//Стандартный фильтр OpenCV
void MedianFilter::filteringWithOpenCV()
{
	cv::medianBlur(m_srcImage, m_dstImageCV, 3);
}

//Разница между стандартным и рукописным фильтром (в пикселях)
void MedianFilter::showDifference(Mat src1, Mat src2)
{
	Mat difference(src1.rows, src1.cols, src1.type());
	cv::absdiff(src1, src2, difference);

	int res = 0;
	for (int i = 1; i < difference.rows - 1; i++)
		for (int j = 1; j < difference.cols - 1; j++)
		{
			if (difference.at<uchar>(i, j) != 0) res++;
		}

	imshow("Difference", difference);
	cout << "Different pixels: " << res << endl;
}

//Геттеры
Mat MedianFilter::getImageHW()
{
	return m_dstImageHW;
}

Mat MedianFilter::getImageHW2()
{
	return m_dstImageHW2;
}

Mat MedianFilter::getImageCV()
{
	return m_dstImageCV;
}

Mat MedianFilter::getImageGauss()
{
	return m_dstImageGauss;
}



