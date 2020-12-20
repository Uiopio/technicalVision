// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "myRobo/MyRobo.h"


using namespace cv;
using namespace std;


int main(void)
{
    


    //color and size
    const Scalar white(255, 255, 255);
    const Scalar black(0, 0, 0);
    Size size(1000, 900);

	//robot's width and height
	float width = 60;
	float height = 120;

	//wheels
	float wheelWidth = 15;
	float wheelDiameter = 30;

	//creating a robot
	MyRobo robot(width, height, wheelWidth, wheelDiameter, 0, 10);
	robot.setSpeed(15);
	robot.setAngularSpeed(10);

	//creating an area
	Size sizeOfArea(1400, 900);
	cv::Mat whiteImage(sizeOfArea, CV_8UC3, white);

	//set size of area and defining the center
	robot.setArea(whiteImage);
	robot.setCenter(whiteImage);

    robot.setCenterOfBlackSquare(600, 300);    
    
    char key = 0;

    while (cv::waitKey(5) != 27) // '27' - code of "Esc"
    {        
        key = cv::waitKey(5);

        Mat imageWithRobot(sizeOfArea, CV_8UC3, white);

        if ((key == 'w') || (key == 'W'))
        {
            robot.move(1);
        }

        if ((key == 'a') || (key == 'A'))
        {
            robot.move(2);
        }

        if ((key == 's') || (key == 'S'))
        {
            robot.move(3);
        }

        if ((key == 'd') || (key == 'D'))
        {
            robot.move(4);
        }
        //rotate
        if ((key == 'q') || (key == 'Q')) // reducing the angle - Q
        {
            robot.rotate(1);
        }

        if ((key == 'e') || (key == 'E')) // increasing the angle E
        {
            robot.rotate(2);
        }

        //черный квадрат

        if ((key == 'y') || (key == 'Y'))
        {
            robot.move(5);
        }

        if ((key == 'g') || (key == 'G'))
        {
            robot.move(6);
        }

        if ((key == 'h') || (key == 'H'))
        {
            robot.move(7);
        }

        if ((key == 'j') || (key == 'J'))
        {
            robot.move(8);
        }
        
		//drawing on a new background
		robot.draw(imageWithRobot);
        robot.drawRestrictedZone(imageWithRobot);

		imshow("imageWithRobot", imageWithRobot);
    }

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
