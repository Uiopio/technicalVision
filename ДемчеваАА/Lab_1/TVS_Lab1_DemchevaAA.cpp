#pragma once
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "my_robot\my_robot.h"
#include <math.h>

using namespace cv;
using namespace std;


int main()
{
	setlocale(LC_ALL, "Russian");

	Size size(640, 480);
	Scalar color(0, 255, 0);
	Mat field(size, CV_8UC3, color);

	MyRobot robot(25, 50, 12, 12); //MyRobot(width, height, wheelWidth, wheelDiameter)

	robot.setArea(field);
	robot.setBorders(100,100);

	robot.setStartPoint(field);
	robot.setSpeed(5);
	robot.setAngularSpeed(CV_PI / 6);

	robot.setPeriod(50);
	robot.setAmplitude(50);
	robot.setAngularFrequency(0.05);
	robot.setSynStep(2);

	while(1)
	{
		char key = waitKey(10);
		//robot.move(key);
		robot.moveSyn(key);
		
		Mat newField = field.clone();
		robot.draw(newField);
		imshow("MyRobot", newField);
	}

	return 0;
}