#include <iostream>
#include "my_robot.h"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;


int main()
{
	MyRobot robot(30, 50, 10, 15, 5, 2, 0);
	Mat map(Size2i(640, 480), CV_8UC3, Scalar(255, 255, 255));
	Mat mapWithRobot(Size2i(640, 480), CV_8UC3, Scalar(255,255,255));
	robot.setArea(map);
	robot.setCenter(map);
	
	while (waitKey(1) != 27)
	{
		map = mapWithRobot.clone();
		robot.move();
		robot.rotate();
		for (int i = 0; i < 360; i++)
		{
			map = mapWithRobot.clone();
			robot.draw(map, map);
			robot.wayEllipse(map, 500, 250, i);
		}

		//robot.draw(map, map);

		//imshow("Map", map);
	}
	return 0;
}