#include <iostream>
#include "my_robot/my_robot.h"

int main()
{
	float widht = 20;
	float height = 60;
	float wheelWidth = 8;
	float wheelDiameter = 8;
	MyRobot RobotOne(widht, height, wheelWidth, wheelDiameter);
	RobotOne.Influence();
	RobotOne.Evolvent(1, 10, -1);//stepAngle, radius, direction, m.b. image

	return 0;
}
