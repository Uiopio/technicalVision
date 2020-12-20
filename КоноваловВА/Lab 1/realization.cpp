#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "my_robot.h"

int main()
{
    // набор цветов
    const Scalar pink(252, 15, 192);
    const Scalar black(0, 0, 0);

    // ширина и высота робота
    float width = 20;
    float height = 60;

    // ширина и диаметр колес
    float wheelWidth = 5;
    float wheelDiameter = 15;

    // создание робота
    MyRobot robot(width, height, wheelWidth, wheelDiameter, 0, 0);
    robot.setSpeed(7);
    robot.setAngularSpeed(2);


    // создание поле и определение центра
    Size size(900, 700);
    Mat field(size, CV_8UC3, pink);
    robot.setArea(field);
    robot.setCenter(field);
    robot.setCircleCenter(field);
    char key = 0;

    // управление роботом
    while (waitKey(25) != 27) //27 -- Esc
    {
        key = waitKey(0);

        Mat imageOfRobot(size, CV_8UC3, pink);

        // движение вдоль осей
        if ((key == 'w') || (key == 'W') || (key == 'ц') || (key == 'Ц'))
        {
            robot.move(1);
        }

        if ((key == 'a') || (key == 'A') || (key == 'ф') || (key == 'Ф'))
        {
            robot.move(2);
        }

        if ((key == 's') || (key == 'S') || (key == 'ы') || (key == 'Ы'))
        {
            robot.move(3);
        }

        if ((key == 'd') || (key == 'D') || (key == 'в') || (key == 'В'))
        {
            robot.move(4);
        }


        // поворот по часовой
        if ((key == 'e') || (key == 'E') || (key == 'у') || (key == 'У'))
        {
            robot.rotate(1);
        }

        // поворот против часовой
        if ((key == 'q') || (key == 'Q') || (key == 'й') || (key == 'Й'))
        {
            robot.rotate(2);
        }

        // перерисовка
        robot.draw(field, imageOfRobot);

        imshow("imageOfRobot", imageOfRobot);
    }

    return 0;

}