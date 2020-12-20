// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma once

#include <ctime>
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "myRobo/MyRobo.h"


using namespace cv;
using namespace std;


int main(void)
{
    srand(time(NULL));
    //создание класса
    median_filter median;

    //считываем картинку и создаем черное изображение того же размера 
    Mat imageForBlur = imread("photo.bmp");  

    Mat MyMedianFilter(imageForBlur.size(), imageForBlur.type(), Scalar(0, 0, 0));


    unsigned int startTime = clock();
    //применение реализованного медианного фильтра
    median.medianBlur(imageForBlur, MyMedianFilter);

    unsigned int endTime = clock();
    unsigned int interval = endTime - startTime;

    //перевод полученного интервала времени в секунды
    cout << '\n' << "myInterval =   " << (float)interval / CLOCKS_PER_SEC;
    //cout << "interval = " << interval << endl;

    Mat outputOpenCV;

    unsigned int startTime1 = clock();

    //вызов библиотечного медианного фильтра
    medianBlur(imageForBlur, outputOpenCV, 7);
    unsigned int endTime1 = clock();
    unsigned int interval1 = endTime1 - startTime1;

    //перевод полученного интервала времени в секунды
    cout << '\n' << "intervalForOpenCV =   " << (float)interval1 / CLOCKS_PER_SEC;



    Mat result;

    //разница между результатом реализованной и библиотечной функций
    absdiff(outputOpenCV, MyMedianFilter, result);

    result = result * 127;
    int nonzeroPixels = 0;
    for (int i = 0; i < result.rows; i++)
    {
        for (int j = 0; j < result.cols; j++)
        {
            for (int channel = 0; channel < 3; channel++)
            {
                if (result.at<Vec<uint8_t, 3>>(i, j)[0] != 0)
                {
                    nonzeroPixels++;
                }
            }
        }
    }

    cout << '\n' << "nonzeroPixels = "<< nonzeroPixels << endl;

    imshow("imageForBlur", imageForBlur);

    imshow("MyMedianFilter", MyMedianFilter);

    imshow("outputOpenCV", outputOpenCV);

    imshow("result", result);

    while (waitKey(25) != 27)
    {
        ;
    }
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
