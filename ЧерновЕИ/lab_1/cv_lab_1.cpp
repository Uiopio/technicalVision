#include <iostream>

#include "opencv2/opencv.hpp"

#include "my_robot/h/my_robot.h"
#include "my_robot/h/robot_output.h"

using namespace cv;

int main()
{
	MyRobot r = MyRobot(100, 150, 30, 40, 1, 0.01f);
	RobotOutput(r, Size2i(1000, 800)).draw();
	int i = 0;
	return i;
}
