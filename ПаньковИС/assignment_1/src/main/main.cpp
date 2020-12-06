#include <iostream>
#include <windows.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "robot.h"
#include "war_robot.h"

#define sleep Sleep

using namespace std;
using namespace cv;

int main()
{
    float width = 60;
    float lenght = 120;
    Wheel wheel = {10, 40};

    auto robot = WarRobot(width, lenght, wheel);
    robot.setSpeed(5.0f);
    robot.setAngularSpeed(0.1f);
    robot.combatModule().setAngularSpeed(0.2f);

    auto white = Scalar(0xFF, 0xFF, 0xFF);
    auto size = Size(1080, 720);
    auto area = Mat(size, CV_8UC3, white);
    
    robot.setArea(area);
    robot.setCenter(area);

    while (waitKey(1) != 27)
    {
        char key = waitKey(100);
        auto areaWithRobot = Mat(size, CV_8UC3, white);

        robot.doSomething(key);
        robot.draw(areaWithRobot);

        imshow("War Robot", areaWithRobot);
        area = areaWithRobot;
    }

    return 0;
}
