#include <iostream>
#include "../23092020/my_robot/my_robot.h"

using namespace cv;
using namespace std;

int main()
{
	MyRobot fedor(75, 125, 15, 25, 10, 3 * (PI / 180));
	Size2i size(1200, 700);
	Scalar color_background(255, 255, 255);
	Mat backgroundImage(size, CV_8UC3, color_background);
	Mat robotImage = backgroundImage.clone();

	fedor.setArea(backgroundImage);
	fedor.setCenter(backgroundImage);
	namedWindow("robot");
	//fedor.draw(backgroundImage, robotImage);
	//imshow("robot", robotImage);
	char pressedKey = waitKey(1);

	Point2f ptRectangle(fedor.getCenter());
	float rectWidth = 400;
	float rectHeight = 300;
	ptRectangle.x -= rectWidth / 2;
	ptRectangle.y -= rectHeight / 2;
	Rect2f rect(ptRectangle.x, ptRectangle.y, rectWidth, rectHeight);
	rectangle(backgroundImage, rect, Scalar(150, 150, 150));
	fedor.setCenter(ptRectangle.x + rectWidth, ptRectangle.y + rectHeight);
	fedor.draw(backgroundImage, robotImage);
	imshow("robot", robotImage);

	float distance = 0;
	float prevAngle = 0;
	bool isVerMoving = true;
	bool isHorMoving = false;
	bool isLastVerMoving = isVerMoving;

	while (pressedKey != 27)
	{
		Point2f bufCenter = fedor.getCenter();
		float bufAngle = fedor.getAngle();
		if (pressedKey == 'f')
		{
			if (isVerMoving)
			{
				if (distance < rect.height)
				{
					fedor.move('u');
					distance += fedor.getSpeed();
				}
				else
				{
					isVerMoving = false;
					isLastVerMoving = true;
					distance = 0;
				}
			}
			else if (isHorMoving)
			{
				if (distance < rect.width)
				{
					fedor.move('u');
					distance += fedor.getSpeed();
				}
				else
				{
					isHorMoving = false;
					isLastVerMoving = false;
					distance = 0;
				}
			}
			else
			{
				if (fedor.getAngle() > (prevAngle - PI / 2))
				{
					fedor.rotate(-fedor.getAngularSpeed());
				}
				else if (isLastVerMoving)
				{
					isHorMoving = true;
					prevAngle -= PI / 2;
					fedor.setAngle(prevAngle);
				}
				else
				{
					isVerMoving = true;
					prevAngle -= PI / 2;
					fedor.setAngle(prevAngle);
				}
			}
		}
		else if (pressedKey == 'w')
		{
			fedor.move('u');
		}
		else if (pressedKey == 's')
		{
			fedor.move('d');
		}
		else if (pressedKey == 'a')
		{
			fedor.move('l');
		}
		else if (pressedKey == 'd')		
		{
			fedor.move('r');
		}
		else if (pressedKey == 'q')
		{
			fedor.rotate(-fedor.getAngularSpeed());
		}
		else if (pressedKey == 'e')
		{
			fedor.rotate(fedor.getAngularSpeed());
		}
		else if (pressedKey == 'r')
		{
			fedor.rotateTurret(-fedor.getAngularSpeed());
		}
		else if (pressedKey == 't')
		{
			fedor.rotateTurret(fedor.getAngularSpeed());
		}
		if (fedor.draw(backgroundImage, robotImage) == 0)
		{
			imshow("robot", robotImage);
			pressedKey = waitKey(1);
			continue;
		}
		fedor.setCenter(bufCenter.x,  bufCenter.y);
		fedor.setAngle(bufAngle);
		pressedKey = waitKey(1);
	}

	return 0;
}
