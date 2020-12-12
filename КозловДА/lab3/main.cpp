#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "Fourier2D.h"

#define LAB    1
#define ADDITIONAL_TASK  2

#define ASSIGNMENT_CURRENT_PART 2

using namespace cv;
using namespace std;

#if ASSIGNMENT_CURRENT_PART == ADDITIONAL_TASK
float newPhase = 0;
float newAmplitude = 1;
int radius = 0;
int i = 0, numOfResult = 1;
int action = 0; //0 - drawing point, 1 - drawing circle in center, 2 - drawing rectangle, 3 - drawing line
int rectWidth = 0, rectHeight = 0;
Point pt[2]{ Point(0, 0),  Point(0, 0) };
Point point;
Fourier2D fourier;

void mouseHandler(int event, int x, int y, int, void*);
#endif

int main()
{
#if ASSIGNMENT_CURRENT_PART == LAB
	/*	1. Сравнение реализованного ДПФ с библиотечным ДПФ	*/

	//Прямое
	Mat source = imread("fourier_62x77.jpg", IMREAD_GRAYSCALE);
	namedWindow("Исходное изображение");
	imshow("Исходное изображение", source);


	Fourier2D fourier(source);
	fourier.forwardTransform();
	Mat myImage(source.size(), CV_32FC2, cv::Scalar(0, 0));
	fourier.getImage(myImage);
	Mat sourceForLib;
	source.convertTo(sourceForLib, CV_32FC1);
	Mat libImage(sourceForLib.size(), CV_32FC2, cv::Scalar(0, 0));
	dft(sourceForLib, libImage, cv::DFT_COMPLEX_OUTPUT);
	float sForwardRe = findS(myImage, libImage, 0);
	float sForwardIm = findS(myImage, libImage, 1);

	//Обратное
	Mat myInverse;
	fourier.inverseTransformFromImage();
	fourier.getResult(myInverse);
	Mat libInverse;
	dft(myImage, libInverse, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
	normalize(libInverse, libInverse, 0, 1, cv::NORM_MINMAX);
	libInverse.convertTo(libInverse, CV_8UC1, 255);
	/*imshow("myInverse", myInverse);
	imshow("source2", source);*/
	float sInverse = compare(myInverse, source);



	/*	2. Фильтр Баттерворта	*/

	//Режекторный
	source = imread("fullCat.jpg", IMREAD_GRAYSCALE);
	namedWindow("Исходное изображение");
	imshow("Исходное изображение", source);
	fourier.setSource(source);
	fourier.filterNotchButterworth(100, 100);
	Mat result;
	fourier.getResult(result);
	namedWindow("Результат для режекторного фильта Баттерворта");
	imshow("Результат для режекторного фильта Баттерворта", result);

	while (waitKey(1) != 27)
	{

	}
	
	//Полосовой
	fourier.filterBandPassButterworth(100, 50);
	fourier.getResult(result);
	namedWindow("Результат для полосового фильта Баттерворта");
	imshow("Результат для полосового фильта Баттерворта", result);

	while (waitKey(1) != 27)
	{

	}



	/*	3, 4. Свертка с различными фильтрами	*/

	/*Mat source = imread("fullCat.jpg", IMREAD_GRAYSCALE);
	namedWindow("Исходное изображение");
	imshow("Исходное изображение", source);*/

	//Фильтр Собеля по горизонтали
 	fourier.setSource(source);
	fourier.filterSobelHorizontal();
	fourier.getResult(result);
	namedWindow("Результат для фильтра Собеля по горизонтали");
	imshow("Результат для фильтра Собеля по горизонтали", result);

	while (waitKey(1) != 27)
	{

	}

	//Фильтр Собеля по вертикали
	fourier.filterSobelVertical();
	fourier.getResult(result);
	namedWindow("Результат для фильтра Собеля по вертикали");
	imshow("Результат для фильтра Собеля по вертикали", result);

	while (waitKey(1) != 27)
	{

	}

	//Усредняющий фильтр
	fourier.filterBox();
	fourier.getResult(result);
	namedWindow("Результат для усредняющего фильтра");
	imshow("Результат для усредняющего фильтра", result);

	while (waitKey(1) != 27)
	{

	}

	//Фильтр Лапласа
	fourier.filterLaplas();
	fourier.getResult(result);
	namedWindow("Результат для фильтра Лапласа");
	imshow("Результат для фильтра Лапласа", result);

	while (waitKey(1) != 27)
	{

	}

	//Спектр образа Фурье источника
	Mat sourceSpectre;
	fourier.getImageSpectre(sourceSpectre);
	namedWindow("Спектр образа Фурье исходного изображения");
	imshow("Спектр образа Фурье исходного изображения", sourceSpectre);

	while (waitKey(1) != 27)
	{

	}



	/*	5. Корреляция	*/

	Mat number = imread("Fourier_1.jpg", IMREAD_GRAYSCALE);
	namedWindow("Номер");
	imshow("Номер", number);

	//Глаза Фурье
	Mat symbol = imread("glaza.png", IMREAD_GRAYSCALE);
	namedWindow("Глаза Фурье");
	imshow("Глаза Фурье", symbol);

	Fourier2D correlation(number);
	correlation.correlation(symbol, "глаза Фурье");
	correlation.getResult(result);
	namedWindow("Результат для глаз Фурье");
	imshow("Результат для глаз Фурье", result);

	while (waitKey(1) != 27)
	{

	}

	//Символ '6'
	symbol = imread("symbol6.jpg", IMREAD_GRAYSCALE);
	namedWindow("Символ '6'");
	imshow("Символ '6'", symbol);

	correlation.correlation(symbol, "Символ '6'");
	correlation.getResult(result);
	namedWindow("Результат для символа '6'");
	imshow("Результат для символа '6'", result);

	while (waitKey(1) != 27)
	{

	}

	//Символ '78'
	symbol = imread("symbol78.jpg", IMREAD_GRAYSCALE);
	namedWindow("Символ '78'");
	imshow("Символ '78'", symbol);

	correlation.correlation(symbol, "Символ '78'");
	correlation.getResult(result);
	namedWindow("Результат для символа '78'");
	imshow("Результат для символа '78'", result);
#endif

#if ASSIGNMENT_CURRENT_PART == ADDITIONAL_TASK
	Mat source(Size(250, 200), CV_8UC1, Scalar(0));
	fourier.setSource(source);
	Mat spectre;
	fourier.getImageSpectre(spectre);
	cv::imshow("Spectre", spectre);
	Mat phase;
	fourier.getImagePhase(phase);
	cv::imshow("Phase", phase);
	setMouseCallback("Spectre", mouseHandler, NULL);
	setMouseCallback("Phase", mouseHandler, NULL);

	while (1)
	{
		int pressedKey = waitKey(1);
		if (pressedKey == 112)
		{
			action = 0;
			std::cout << "DRAWING POINT" << std::endl;
		}
		else if (pressedKey == 99)
		{
			action = 1;
			std::cout << "DRAWING CIRCLE" << std::endl;
		}
		else if (pressedKey == 114)
		{
			action = 2;
			std::cout << "DRAWING RECTANGLE" << std::endl;
		}
		else if (pressedKey == 108)
		{
			action = 3;
			std::cout << "DRAWING LINE" << std::endl;
			i += (i % 2);
		}
		if (pressedKey == 32)
		{
			Mat inverse;
			fourier.inverseTransformFromSpectrePhase();
			fourier.getResult(inverse);
			string winName = "Result " + to_string(numOfResult);
			cv::imshow(winName, inverse);
			numOfResult++;
		}
		else if (pressedKey == 27)
		{
			break;
		}
	}

	return 0;
#endif
}

#if ASSIGNMENT_CURRENT_PART == ADDITIONAL_TASK
void mouseHandler(int event, int x, int y, int, void*)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		if (action == 0)
		{
			std::cout << "Point" << i << "(" << x << ", " << y << ")" << std::endl;
			point.x = x;
			point.y = y;
		}
		if (action == 1)
		{
			std::cout << "Enter radius of circle: " << std::endl;
			std::cin >> radius;
			std::cout << std::endl;
		}
		if (action == 2)
		{
			std::cout << "Enter width of rectangle: " << std::endl;
			std::cin >> rectWidth;
			std::cout << "Enter height of rectangle: " << std::endl;
			std::cin >> rectHeight;
			std::cout << std::endl;
		}
		if (action == 3)
		{
			pt[i % 2].x = x;
			pt[i % 2].y = y;
		}
	}
	else if (event == cv::EVENT_LBUTTONUP)
	{
		Mat tempSpectre;
		fourier.getImageSpectre(tempSpectre, CV_32FC1);
		if (action == 0)
		{
			tempSpectre.at<float>(point.y, point.x) = newAmplitude;
			tempSpectre.at<float>(tempSpectre.rows - point.y, tempSpectre.cols - point.x) = newAmplitude;
		}
		else if (action == 1)
		{
			circle(tempSpectre, Point(tempSpectre.cols / 2, tempSpectre.rows / 2), radius, Scalar(newAmplitude), 1);
		}
		else if (action == 2)
		{
			Rect rect(tempSpectre.cols / 2 - rectWidth / 2, tempSpectre.rows / 2 - rectHeight / 2, rectWidth, rectHeight);
			rectangle(tempSpectre, rect, Scalar(newAmplitude));
		}
		else if ((action == 3) && (i % 2 == 1))
		{
			line(tempSpectre, pt[0], pt[1], Scalar(newAmplitude));
			line(tempSpectre, Point(tempSpectre.cols - pt[0].x, tempSpectre.rows - pt[0].y), Point(tempSpectre.cols - pt[1].x, tempSpectre.rows - pt[1].y), Scalar(newAmplitude));
		}
		fourier.setImageSpectre(tempSpectre);
		fourier.getImageSpectre(tempSpectre);
		cv::imshow("Spectre", tempSpectre);

		Mat tempPhase;
		fourier.getImagePhase(tempPhase, CV_32FC1);
		if (action == 0)
		{
			tempPhase.at<float>(point.y, point.x) = newPhase;
			tempPhase.at<float>(tempPhase.rows - point.y, tempPhase.cols - point.x) = newPhase;
		}
		else if (action == 1)
		{
			circle(tempPhase, Point(tempPhase.cols / 2, tempPhase.rows / 2), radius, Scalar(newPhase), 1);
		}
		else if (action == 2)
		{
			Rect rect(tempSpectre.cols / 2 - rectWidth / 2, tempSpectre.rows / 2 - rectHeight / 2, rectWidth, rectHeight);
			rectangle(tempPhase, rect, Scalar(newPhase));
		}
		else if ((action == 3) && (i % 2 == 1))
		{
			line(tempPhase, pt[0], pt[1], Scalar(newPhase));
			line(tempPhase, Point(tempPhase.cols - pt[0].x, tempPhase.rows - pt[0].y), Point(tempPhase.cols - pt[1].x, tempPhase.rows - pt[1].y), Scalar(newPhase));
		}
		fourier.setImagePhase(tempPhase);
		fourier.getImagePhase(tempPhase);
		cv::imshow("Phase", tempPhase);

		i++;
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		Mat tempSpectre;
		fourier.getImageSpectre(tempSpectre, CV_32FC1);
		Mat emptySpectre(tempSpectre.size(), CV_32FC1, Scalar(0));
		fourier.setImageSpectre(emptySpectre);
		fourier.getImageSpectre(tempSpectre);
		cv::imshow("Spectre", tempSpectre);

		Mat tempPhase;
		fourier.getImagePhase(tempPhase, CV_32FC1);
		Mat emptyPhase(tempPhase.size(), CV_32FC1, Scalar(0));
		fourier.setImagePhase(emptyPhase);
		fourier.getImagePhase(tempPhase);
		cv::imshow("Phase", tempPhase);
	}
}
#endif