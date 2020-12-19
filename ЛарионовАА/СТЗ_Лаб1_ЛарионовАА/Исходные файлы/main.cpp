
#include <iostream>
#include "windows.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "my_robot\my_robot.h"

using namespace cv;
using namespace std;

int main()
{
    Size2i sizeSquare(600, 600);
    Scalar color(255);
    Mat square(sizeSquare, CV_8UC1, color);

    MyRobot robot(50, 50, 10, 0, 0);

    float speed = 0;
    float angularSpeed = 0;
    float speedStep = 1;
    float angularSpeedStep = 0.1;

    robot.setSpeed(speed);
    robot.setAngularSpeed(angularSpeed);
    robot.setArea(square);
    robot.setCenter(square);
    robot.setAngle(CV_PI);

    Mat squareWithRobot;
    squareWithRobot = square.clone();

    char command;

    //TickMeter timer; 
 
    //while (waitKey(33) != 27)
    //{
    //    //timer.start();
    //    //double start = timer.getTimeMilli();

    //    imshow("Square with robot", squareWithRobot);

    //    command = waitKey(33);

    //    if (command == 'w' || command == 'W')
    //    {
    //        robot.move(1, 0);
    //    }

    //    if (command == 's' || command == 'S')
    //    {
    //        robot.move(-1, 0);
    //    }

    //    if (command == 'a' || command == 'A')
    //    {
    //        robot.move(0, -1);
    //    }

    //    if (command == 'd' || command == 'D')
    //    {
    //        robot.move(0, 1);
    //    }

    //    if (command == 'e' || command == 'E')
    //    {
    //        robot.rotate(true);
    //    }

    //    if (command == 'q' || command == 'Q')
    //    {
    //        robot.rotate(false);
    //    }

    //    if (command == 'r' || command == 'R')
    //    {
    //        speed += speedStep;
    //        if (speed > 50)
    //        {
    //            speed = 50;
    //        }
    //        robot.setSpeed(speed);
    //    }

    //    if (command == 'f' || command == 'F')
    //    {
    //        speed -= speedStep;
    //        if (speed < 0)
    //        {
    //            speed = 0;
    //        }
    //        robot.setSpeed(speed);
    //    }

    //    if (command == 'v' || command == 'V')
    //    {
    //        angularSpeed += angularSpeedStep;
    //        if (angularSpeed > 1)
    //        {
    //            angularSpeed = 1;
    //        }
    //        robot.setAngularSpeed(angularSpeed);
    //    }

    //    if (command == 'c' || command == 'C')
    //    {
    //        angularSpeed -= angularSpeedStep;
    //        if (angularSpeed < 0)
    //        {
    //            angularSpeed = 0;
    //        }
    //        robot.setAngularSpeed(angularSpeed);
    //    }

    //    robot.draw(square, squareWithRobot);
    //    imshow("Square with robot", squareWithRobot);

    //    //timer.stop();
    //    //double end = timer.getTimeMilli();
    //    //cout << endl << "Time per iteration, ms " << (end - start) << endl;

    //    Sleep(3);
    //}

    while (waitKey(33) != 27)
    {
        robot.figure(120, 80, square, squareWithRobot);
        robot.draw(square, squareWithRobot);
        imshow("Square with robot", squareWithRobot);;

        Sleep(3);
    }

    return 0;
}
