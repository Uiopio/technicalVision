// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "gauss.h"
#include <ctime>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	current_image ReadyForGauss(2);
	int ErroreCode = 0;
	ErroreCode = ReadyForGauss.setImage("Lena.png");
	clock_t t = clock();
	ErroreCode = ReadyForGauss.processImage();
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;

	const Mat myimage = ReadyForGauss.getImage();

	imshow("image", imread("Lena.png"));
	imshow("test", myimage);

	// Проверки с помощью библиотечной функции, не забудь раскомментить в функции!
	Mat testImage;
	t = clock();
	GaussianBlur(imread("Lena.png"), testImage, Size(7, 7), 2, 0, BORDER_CONSTANT);
	t = clock()-t;
	cout << (float)t / CLOCKS_PER_SEC << endl;

	Mat rez;
	absdiff(testImage, myimage, rez);

	//float SumR = 0;
	//float SumG = 0;
	//float SumB = 0;
	//int pixCount = 0;
	//for(int i=0; i<rez.rows; i++)
	//{
	//	for(int j=0; j<rez.cols;j++)
	//	{
	//		SumB+=rez.at<Vec3b>(i, j)[0];
	//		SumG+=rez.at<Vec3b>(i, j)[1];
	//		SumR+=rez.at<Vec3b>(i, j)[2];
	//		pixCount++;
	//	}
	//}
	//SumB/=pixCount;
	//SumG/=pixCount;
	//SumR/=pixCount;

	//cout << SumB << endl << SumG << endl << SumR;

	bitwise_not(rez, rez);
	
	imshow("testImage", testImage);
	imshow("rez", rez);

	while (waitKey(1));
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.