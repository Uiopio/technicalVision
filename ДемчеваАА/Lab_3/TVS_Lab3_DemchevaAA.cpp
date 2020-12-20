#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "FourierTransform.h"
#include <math.h>

using namespace cv;
using namespace std;

void swapSpectrum(Mat& magI)
{
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // верхний левый
	Mat q1(magI, Rect(cx, 0, cx, cy));  // верхний правый
	Mat q2(magI, Rect(0, cy, cx, cy));  // нижний левый
	Mat q3(magI, Rect(cx, cy, cx, cy)); // нижний правый

	Mat tmp;                           // обмен квадрантов 0-3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // обмен квадрантов 1-2
	q2.copyTo(q1);
	tmp.copyTo(q2);
}


void part1_Transform_and_Batterworth()
{
	FourierTransform MyFourier1;

	//Исходное изображение
		Mat image = imread("sign.bmp", IMREAD_GRAYSCALE);
		imshow("original image ", image);

		//Пользовательская функция 
			
			Mat realPart;
			Mat imagPart;
		
			int myDirectStart = clock();
			Mat myDirRes = MyFourier1.directTransform(image, realPart, imagPart).clone(); //Mat srcImg, Mat realPart, Mat imagPart
			int myDirectEnd = clock();
			cout << "Число тактов для выполнения пользовательской функции (прямое ДПФ): " << (myDirectEnd - myDirectStart) << endl;
			cout << "Время выполнения пользовательской функции (прямое ДПФ):: " << ((double)(myDirectEnd - myDirectStart) / CLOCKS_PER_SEC) << endl << endl;


			//Вывод спектра
			Mat mySpectrum = MyFourier1.buildSpectrum(realPart, imagPart).clone(); 
			MyFourier1.showSpectrum(mySpectrum, "My spectrum");
			
			//Обратное преобразование
			Mat myResult;
			int myInverseStart = clock();
			MyFourier1.inverseTransform(myDirRes, myResult, "reverse transform result"); 
			int myInverseEnd = clock();
			cout << "Число тактов для выполнения пользовательской функции (обратное ДПФ): " << (myInverseEnd - myInverseStart) << endl;
			cout << "Время выполнения пользовательской функции (обратное ДПФ):: " << ((double)(myInverseEnd - myInverseStart) / CLOCKS_PER_SEC) << endl << endl;
		

		//Библиотечная функция
			Mat srcImg;
			image.convertTo(srcImg, CV_64FC1); 
		
			Mat bInDir;
			Mat bInResult;
		
			int builtInStart_Direct = clock();
			dft(srcImg, bInDir, DFT_COMPLEX_OUTPUT);
			int builtInEnd_Direct = clock();

			int builtInStart_Inverse = clock();
			dft(bInDir, bInResult, DFT_INVERSE | DFT_REAL_OUTPUT);
			int builtInEnd_Inverse = clock();
		
			cout << "Число тактов для выполнения встроенной функции (прямое ДПФ):  " << (builtInEnd_Direct - builtInStart_Direct) << endl;
			cout << "Время выполнения встроенной функции (прямое ДПФ):  " << ((double)(builtInEnd_Direct - builtInStart_Direct) / CLOCKS_PER_SEC) << endl << endl;

			cout << "Число тактов для выполнения встроенной функции (обратное ДПФ):  " << (builtInEnd_Inverse - builtInStart_Inverse) << endl;
			cout << "Время выполнения встроенной функции (обратное ДПФ):  " << ((double)(builtInEnd_Inverse - builtInStart_Inverse) / CLOCKS_PER_SEC) << endl << endl;



			//Вывод результатов
			normalize(bInResult, bInResult, 0, 255, NormTypes::NORM_MINMAX);
			bInResult.convertTo(bInResult, CV_8UC1);
			imshow("built in function Result", bInResult);
		
		
			vector<Mat> array; //Разбиваем изображение на массив одноканальных
		    split(bInDir, array);
		    
			Mat magnitudes;
			Mat spectrum;
		    magnitude(array[0], array[1], magnitudes); //Находим амплитуду для каждого элемента образа Фурье
		    
		    spectrum = magnitudes.clone();
		    swapSpectrum(spectrum); //Меняем местами квадранты спектра
		    
		    spectrum += Scalar::all(1); //Переходим к логарифмическому масштабу
		    log(spectrum, spectrum);
		    normalize(spectrum, spectrum, 0, 1, NormTypes::NORM_MINMAX);
		    
		    spectrum.convertTo(spectrum, CV_8UC1, 255);
		    imshow("built in function spectrum", spectrum);
		
		
		//Сравнение
			Mat diffImg;
			absdiff(bInResult, myResult, diffImg);
			imshow("diffImg", diffImg * 100); 

			MyFourier1.squareDeviation(diffImg, "My Fourier and Built in Fourier");
		

        	//Работа с фильтрами (Баттерворт)
        
        	Mat swappedSpec = mySpectrum.clone();
        	swapSpectrum(swappedSpec);
        
        	double W = 5; // W - ширина полосы
        	double cutoffD = 35; // cutoffD - частота среза
        	double n = 1; // n - порядок фильтра
        
        	//Фильтр верхних частот
        	Mat highFilteredSpec;
        	Mat highFilteredImg;
        	MyFourier1.hpButterworthFilter(myDirRes, swappedSpec, W, cutoffD, n, highFilteredImg, highFilteredSpec); //Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec
        	MyFourier1.showFilterResult(highFilteredSpec, "High-pass Butterworth filtered spectrum");
        
        	Mat InvHFRes;
        	MyFourier1.inverseTransform(highFilteredImg, InvHFRes, "high-filtered img"); // Mat& srcImg, Mat& resImg
        	
        	//Фильтр нижних частот
        	Mat lowFilteredSpec;
        	Mat lowFilteredImg;
        	MyFourier1.lpButterworthFilter(myDirRes, swappedSpec, 5, 10, 1, lowFilteredImg, lowFilteredSpec); //Mat& srcImg, Mat& srcSpec, double W, double cutoffD, double n, Mat& filteredImg, Mat& filteredSpec
        	MyFourier1.showFilterResult(lowFilteredSpec, "Low-pass Butterworth filtered spectrum");
        
        	Mat InvLFRes;
        	MyFourier1.inverseTransform(lowFilteredImg, InvLFRes, "low-filtered img"); // Mat& srcImg, Mat& resImg

	return;
}


void part2_Convolution()
{
	FourierTransform MyFourier2;

	//СВЕРТКА
	Mat image = imread("queensg.bmp", IMREAD_GRAYSCALE);
	imshow("original image ", image);

	Mat box = (Mat_<double>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	Mat horSobel = (Mat_<double>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	Mat verSobel = (Mat_<double>(3, 3) << 1, 0, -1, 2, 0,-2, 1, 0,-1);
	Mat laplace = (Mat_<double>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);

	Mat myBox = MyFourier2.convolution(image, box, "ext box image", "ext box filter", "box filter result");
	Mat myHorSobel = MyFourier2.convolution(image, horSobel, "ext horSobel image", "ext horSobel filter", "horSobel filter result");
	Mat myVerSobel = MyFourier2.convolution(image, verSobel, "ext verSobel image", "ext verSobel filter", "verSobel filter result");
	Mat myLaplace = MyFourier2.convolution(image, laplace, "ext laplace image", "ext laplace filter", "laplace filter result");


//ПРОВЕРКА ФИЛЬТРОВ
	//Box filter
	Mat boxCheckOutput;
	boxFilter(image, boxCheckOutput, CV_8UC1, Size(3, 3), Point(0, 0));
	imshow("CV box", boxCheckOutput);

	Mat diffImgBox;
	absdiff(boxCheckOutput, myBox, diffImgBox);
	imshow("diff box Img", diffImgBox);
	MyFourier2.squareDeviation(diffImgBox, "Box filter");


	//Vertical Sobel
	Mat verSobelCheckOutput;
	Sobel(image, verSobelCheckOutput, CV_64FC1, 1, 0); //src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
	normalize(verSobelCheckOutput, verSobelCheckOutput, 0, 255, NormTypes::NORM_MINMAX);
	verSobelCheckOutput.convertTo(verSobelCheckOutput, CV_8UC1);
	imshow("CV ver Sobel", verSobelCheckOutput);
	Mat diffImgVSobel;
	absdiff(verSobelCheckOutput, myVerSobel, diffImgVSobel);
	imshow("diff Vert Sobel Img", diffImgVSobel);
	MyFourier2.squareDeviation(diffImgVSobel, "Vertical Sobel filter");


	//Horizontal Sobel
	Mat horSobelCheckOutput;
	Sobel(image, horSobelCheckOutput, CV_64FC1, 0, 1);
	normalize(horSobelCheckOutput, horSobelCheckOutput, 0, 255, NormTypes::NORM_MINMAX);
	horSobelCheckOutput.convertTo(horSobelCheckOutput, CV_8UC1);
	imshow("CV hor Sobel", horSobelCheckOutput);
	Mat diffImgHSobel;
	absdiff(horSobelCheckOutput, myHorSobel, diffImgHSobel);
	imshow("diff Hor Sobel Img", diffImgHSobel);
	MyFourier2.squareDeviation(diffImgHSobel, "Horizontal Sobel filter");


	//Laplace
	Mat laplaceCheckOutput;
	Laplacian(image, laplaceCheckOutput, CV_64FC1, 3);
	normalize(laplaceCheckOutput, laplaceCheckOutput, 0, 255, NormTypes::NORM_MINMAX);
	laplaceCheckOutput.convertTo(laplaceCheckOutput, CV_8UC1);
	imshow("CV laplace", laplaceCheckOutput);
	Mat diffImgLaplace;
	absdiff(laplaceCheckOutput, myLaplace, diffImgLaplace);
	imshow("diff Laplace Img", diffImgLaplace);
	MyFourier2.squareDeviation(diffImgLaplace, "Laplace filter");


	return;
}


void part3_Correlation()
{
	FourierTransform MyFourier3;

	Mat plate = imread("license_plate.bmp", IMREAD_GRAYSCALE);
	imshow("original image ", plate);

	Mat digit = imread("digit_4.bmp", IMREAD_GRAYSCALE);
	imshow("digit_4", digit);

	Mat myCorr = MyFourier3.correlation(plate, digit, "plate", "digit", "Correlation result");

	return;
}




int main()
{
	setlocale(LC_ALL, "Russian");

	part1_Transform_and_Batterworth(); 
	//part2_Convolution();
	//part3_Correlation();

	while (waitKey(1) != 27)
	{
		;
	}

	return 0;
}