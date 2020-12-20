#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "my_robot.h"

int main()
{
    // ����� ������
    const Scalar pink(252, 15, 192);
    const Scalar black(0, 0, 0);

    // ������ � ������ ������
    float width = 20;
    float height = 60;

    // ������ � ������� �����
    float wheelWidth = 5;
    float wheelDiameter = 15;

    // �������� ������
    MyRobot robot(width, height, wheelWidth, wheelDiameter, 0, 0);
    robot.setSpeed(7);
    robot.setAngularSpeed(2);


    // �������� ���� � ����������� ������
    Size size(900, 700);
    Mat field(size, CV_8UC3, pink);
    robot.setArea(field);
    robot.setCenter(field);
    robot.setCircleCenter(field);
    char key = 0;

    // ���������� �������
    while (waitKey(25) != 27) //27 -- Esc
    {
        key = waitKey(0);

        Mat imageOfRobot(size, CV_8UC3, pink);

        // �������� ����� ����
        if ((key == 'w') || (key == 'W') || (key == '�') || (key == '�'))
        {
            robot.move(1);
        }

        if ((key == 'a') || (key == 'A') || (key == '�') || (key == '�'))
        {
            robot.move(2);
        }

        if ((key == 's') || (key == 'S') || (key == '�') || (key == '�'))
        {
            robot.move(3);
        }

        if ((key == 'd') || (key == 'D') || (key == '�') || (key == '�'))
        {
            robot.move(4);
        }


        // ������� �� �������
        if ((key == 'e') || (key == 'E') || (key == '�') || (key == '�'))
        {
            robot.rotate(1);
        }

        // ������� ������ �������
        if ((key == 'q') || (key == 'Q') || (key == '�') || (key == '�'))
        {
            robot.rotate(2);
        }

        // �����������
        robot.draw(field, imageOfRobot);

        imshow("imageOfRobot", imageOfRobot);
    }

    return 0;

}