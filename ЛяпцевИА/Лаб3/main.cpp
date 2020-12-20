// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Fourier.h"
#include "opencv.hpp"
#include <ctime>
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	Fourier2D ft{};
	

	///////////////////////////////Просто прямое и обратное + разница
	//
	//Считывание входного изображения
	//Mat inputImage = imread("Lena100x100.png", IMREAD_GRAYSCALE); //исходное
	//ft.setSource("Lena100x100.png");
	//ft.expansion(inputImage);
	////inputImage.convertTo(inputImage, CV_32FC1); //переводим в нужный формат

	////clock_t t = clock();
	//ft.forwardTransform();
	//ft.inverseTransform();
	////t = clock() - t;
	//Mat Forward3(inputImage.size(), CV_32FC1, Scalar(0));
	//ft.getSource(Forward3);
	////cout << (float)t / CLOCKS_PER_SEC << endl;
	////
	////Mat Forward1(inputImage.size(), CV_32FC2, Scalar(0));
	////Mat Forward2(inputImage.size(), CV_32FC1, Scalar(0));

	////t = clock();
	////dft(inputImage, Forward1, DFT_COMPLEX_OUTPUT);
	////dft(Forward1, Forward2, DFT_INVERSE | DFT_REAL_OUTPUT);
	////t = clock() - t;
	////cout << (float)t / CLOCKS_PER_SEC << endl;

	////normalize(Forward3, Forward3, 0, 1, NORM_MINMAX); //вот тут возможно и кроется проблема
	//Forward3.convertTo(Forward3, CV_8UC1, 255);
	//Mat Sup;
	////normalize(Sup, Sup, 0, 1, NORM_MINMAX);
	//Sup.convertTo(Sup, CV_8UC1, 255);
	//inputImage.convertTo(inputImage, CV_8UC1, 255);
	//absdiff(imread("Lena100x100.png", IMREAD_GRAYSCALE), Forward3, Sup);

	//imshow("Sup", Sup);
	//imshow("Result_Inverse", Forward3);
	//imshow("Input", imread("Lena100x100.png", IMREAD_GRAYSCALE));

	///*
	//Mat rez;
	//absdiff(Forward2, Forward3, rez);
	//static double quadroDiff = 0;
	//static int pixCount = 0;
	//
	//for (static int i = 0; i < rez.rows; i++)
	//{
	//	for (static int j = 0; j < rez.cols; j++)
	//	{
	//		quadroDiff += sqrt(rez.at<float>(i, j) * rez.at<float>(i, j));
	//		pixCount ++;
	//	}
	//}
	//
	//quadroDiff/= pixCount;
	//cout << quadroDiff;*/
	/////////////////////////////////////////////////////////////////////

	///////////////////////////////Гаусс
	//
	////Создаем спектр Гаусса и применяем его для ФНЧ
	//ft.forwardTransform();
	//ft.spectrum();
	//////ft.createGaussian(10, 10);
	//Mat resultInv;
	/////*ft.getSource(resultInv);
	////normalize(resultInv, resultInv, 0, 1, NORM_MINMAX);
	////resultInv.convertTo(resultInv, CV_8UC1, 255);
	////imshow("FinalGauss", resultInv);
	////while (waitKey(0) != 27)
	////{
	////}*/

	//////Создаем спектр Гаусса и применяем его для ФВЧ
	//ft.createMinusGaussian(10, 10);
	//ft.getSource(resultInv);

	//normalize(resultInv, resultInv, 0, 1, NORM_MINMAX);
	//resultInv.convertTo(resultInv, CV_8UC1, 255);
	//imshow("FinalGauss", resultInv);
	
	/////////////////////////////////////

	////////////////Разные фильтры
	////Прямое
	//ft.forwardTransform();
	//Mat Forward1;
	//ft.spectrum();
	//ft.getFourierImage(Forward1);
	//
	////Фильтр Лапласа
	//ft.laplas(Forward1);

	////Прямоугольный фильтр
	//ft.box(Forward1);

	////Фильтр Собеля
	//ft.sobel(Forward1, 0);
	//ft.sobel(Forward1, 1);
	
	///////////Поиск цифр
	//Mat FullNumber = imread("climate-change-history-inside-large.jpg", IMREAD_GRAYSCALE);
	//Mat Number = imread("glaza.png", IMREAD_GRAYSCALE);
	//ft.carNumber2(FullNumber, Number);
	/////////////////////////
	
	Mat Camouflage;
	doSuperTask("Forest1.jpg", "Jacket.jpg", 15, Camouflage);
	//doSuperTask("Jacket.jpg", "Forest1.jpg", 15, Camouflage);
	
	//"Jacket.jpg"
	//"Forest.jpg"
	while (waitKey(0) != 27)
	{
	}
	return 0;
}








