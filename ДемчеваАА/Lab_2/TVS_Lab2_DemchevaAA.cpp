#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <cmath>
#include <ctime>
#include "myGaussianBlur.h"

using namespace cv;
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian");

	myGaussianBlur myGauss;

	double sigma = 10; 
	int kernelRows = 7;
	int kernelCols = 7;

//Ядро	
	myGauss.setKernel(sigma, kernelRows, kernelCols);

//Оригинальное изображение
	//Mat image = imread("C:/222.bmp", IMREAD_GRAYSCALE);
	Mat image = imread("C:/222.bmp", 1);
	cout << "source image chanels = " << image.channels() << endl;
	myGauss.setSrcImg(image);
	myGauss.getSrcImg();

//Встроенный фильтр
	Mat builtInGaussImg = image.clone();

	int builtInStart = clock();
	GaussianBlur(image, builtInGaussImg, Size2i(7, 7), 10, 10, 1);
	int builtInEnd = clock();

	imshow("builtInGauss", builtInGaussImg);

	cout << "Число тактов для выполнения встроенной функции:  " << (builtInEnd - builtInStart) << endl;
	cout << "Время выполнения встроенной функции:  " << ((double)(builtInEnd - builtInStart) / CLOCKS_PER_SEC) << endl;


//Пользовательский фильтр Гаусса
	myGauss.expandImage();
	myGauss.getExpandedImg();

	int myGaussStart = clock();
	myGauss.smooth();
	int  myGaussEnd = clock();
	myGauss.getResultImg();

	cout << "Число тактов для выполнения пользовательской функции: "  << (myGaussEnd - myGaussStart) << endl;
	cout << "Время выполнения пользовательской функции: " << ((double)(myGaussEnd - myGaussStart) / CLOCKS_PER_SEC) << endl;
	
//Вычисляем разницу 
	myGauss.calcDiff(builtInGaussImg);
	myGauss.getDiffImg();
	myGauss.squareDeviation();
    
	

	while (waitKey(1) != 27)
	{
		;
	}
	
	return 0;
}


//*Mat imageChess = imread("C:/Bezymyanny100.bmp", 1);
//imshow("Chess", imageChess);
//
//double scaleX = 1.1;
//double scaleY = 1.1;
//
//Mat imgLarge
//(int)(scaleX)
//
//while (waitKey(1) != 27)
//{
//	;
//}*/