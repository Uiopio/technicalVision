
#include "my_robot.h"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "cmath"

#define PI 3.14159265

using namespace cv;
using namespace std;

MyRobot::MyRobot()
{
	;
}


MyRobot::MyRobot(float width, float height,
			  	 float wheelWidth, float wheelDiameter,
				 float speed, float angularSpeed):
	m_width(width),
	m_height(height),
	m_wheelWidth(wheelWidth),
	m_wheelDiameter(wheelDiameter),
	m_speed(speed),
	m_angularSpeed(angularSpeed)
{
	;
}

MyRobot::~MyRobot()
{
	;
}

//Сеттеры
void MyRobot::setArea(cv::Size2i area)
{
	m_area = area;
	m_area.width = area.width;
	m_area.height = area.height;
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.width = m_area.width;
	m_rect.height = m_area.height;
}

int MyRobot::setArea(cv::Mat image)
{
	if (image.empty())
	{
		return -1;
	}
	m_area.width = image.cols;
	m_area.height = image.rows;

	return 0;
}

int MyRobot::setCenter(cv::Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	if ((image.cols / 2 > m_area.width) ||
		(image.rows / 2 > m_area.height))
	{
		return -2;
	}

	m_center.x = image.cols / 2;
	m_center.y = image.rows / 2;

	return 0;
}

int MyRobot::setCenter(int x, int y)
{
	if ((x > m_area.width) ||
		(y > m_area.height))
	{
		return -1;
	}
	
	m_center.x = x;
	m_center.y = y;

	return 0;
}

//Геттеры
float MyRobot::getSpeed()
{
	return m_speed;
}

float MyRobot::getAngle()
{
	return m_angle;
}

float MyRobot::getAngularSpeed()
{
	return m_angularSpeed;
}

//Управление движением
void MyRobot::move(int x, int y)
{
	m_center.x += (x * cos(m_angle*PI / 180) - y * sin(m_angle*PI / 180)) * m_speed;
	m_center.y += (x * sin(m_angle*PI / 180) + y * cos(m_angle*PI / 180)) * m_speed;
}

int MyRobot::rotate(float angle)
{
	m_angle += angle * m_angularSpeed;
	return 0;
}

//Проверка границ
bool MyRobot::checkBorders(Point2f point, std::vector<cv::Point> vectorTriangleArea, std::vector<cv::Point> vectorRobotArea)
{	
	if (m_rect.contains(point))
	{
		if (pointPolygonTest(vectorTriangleArea, point, false) >= 0)
		{
			return true;
		} 
		if ((pointPolygonTest(vectorRobotArea, vectorTriangleArea[0], true) >= 0) ||
			(pointPolygonTest(vectorRobotArea, vectorTriangleArea[1], true) >= 0) ||
			(pointPolygonTest(vectorRobotArea, vectorTriangleArea[2], true) >= 0))
		{
			return true;
		}
		return false;
	}
	return true;
}

//Отрисовка
int MyRobot::draw(cv::Mat &ioImage, char key)
{
	m_key = key;

	//if (ioImage.empty())
	//{
	//	return -1;
	//}

	//if ((ioImage.cols != m_area.width) || (ioImage.rows != m_area.height))
	//{
	//	return -2;
	//}
	
	Scalar scalarLine(255, 0, 0);
	//Точки для построения робота
	Point2f upLeftCorner(m_center.x - (m_width / 2 * cos(m_angle*PI / 180) + m_height / 2 * -sin(m_angle*PI / 180)),
						 m_center.y - (m_width / 2 * sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180)));

	Point2f upRightCorner(m_center.x + (m_width / 2 * cos(m_angle*PI / 180) + m_height / 2 * sin(m_angle*PI / 180)),
						  m_center.y - (m_width / 2 * -sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180)));

	Point2f downLeftCorner(m_center.x - (m_width / 2 * cos(m_angle*PI / 180) + m_height / 2 * sin(m_angle*PI / 180)),
						   m_center.y + (m_width / 2 * -sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180)));

	Point2f downRightCorner(m_center.x + (m_width / 2 * cos(m_angle*PI / 180) + m_height / 2 * -sin(m_angle*PI / 180)),
							m_center.y + (m_width / 2 * sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180)));

	//Точки для построения колес робота

	//Левое верхнее колесо
	Point2f upLeftWheel[3];
	//Левая верхняя точка
	upLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + m_height / 2 * -sin(m_angle*PI / 180));
	upLeftWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180));
	//Левая нижняя точка
	upLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*PI / 180));
	upLeftWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	//Правая нижняя точка
	upLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*PI / 180));
	upLeftWheel[2].y = m_center.y - (m_width / 2 * sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));

	//Правое верхнее колесо
	Point2f upRightWheel[3];
	//Правая верхняя точка
	upRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + m_height / 2 * sin(m_angle*PI / 180));
	upRightWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180));
	//Правая нижняя точка
	upRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*PI / 180));
	upRightWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	//Левая нижняя точка
	upRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*PI / 180));
	upRightWheel[2].y = m_center.y - (m_width / 2 * -sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	
	//Левое нижнее колесо
	Point2f downLeftWheel[3];
	//Левая нижняя точка
	downLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + m_height / 2 * sin(m_angle*PI / 180));
	downLeftWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180));
	//Левая верхняя точка
	downLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*PI / 180));
	downLeftWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	//Правая верхняя точка
	downLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*PI / 180));
	downLeftWheel[2].y = m_center.y + (m_width / 2 * -sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	
	//Правое нижнее колесо
	Point2f downRightWheel[3];
	//Правая нижняя точка
	downRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + m_height / 2 * -sin(m_angle*PI / 180));
	downRightWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle*PI / 180) + m_height / 2 * cos(m_angle*PI / 180));
	//Правая верхняя точка
	downRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*PI / 180));
	downRightWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));
	//Левая верхняя точка
	downRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*PI / 180));
	downRightWheel[2].y = m_center.y + (m_width / 2 * sin(m_angle*PI / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*PI / 180));

	//Зона робота
	// 1o-------------o2
	//  |             |
	//12o---o11  4o---o3
	//	    |	  |
	//      |     |
	//      |	  |
	// 9o---o10  5o---o6
	//  |             |
	// 8o-------------o7

	vector<cv::Point> robotArea;

	robotArea.push_back(upLeftWheel[0]);     //1
	robotArea.push_back(upRightWheel[0]);    //2
	robotArea.push_back(upRightWheel[1]);    //3
	robotArea.push_back(upRightWheel[2]);    //4
	robotArea.push_back(downRightWheel[2]);  //5
	robotArea.push_back(downRightWheel[1]);  //6
	robotArea.push_back(downRightWheel[0]);  //7
	robotArea.push_back(downLeftWheel[0]);   //8
	robotArea.push_back(downLeftWheel[1]);   //9
	robotArea.push_back(downLeftWheel[2]);   //10
	robotArea.push_back(upLeftWheel[2]);     //11
	robotArea.push_back(upLeftWheel[1]);     //12

	//polylines(ioImage, m_robotArea, true, scalarLine);
	

	////Треугольник
	//Point2f triangle[3];

	//triangle[0].x = 200;
	//triangle[0].y = 200;

	//triangle[1].x = 300;
	//triangle[1].y = 200;

	//triangle[2].x = 200;
	//triangle[2].y = 300;




	//Линии робота
	line(ioImage, upLeftCorner, upRightCorner, scalarLine, 1, 8, 0);
	line(ioImage, upRightCorner, downRightCorner, scalarLine, 1, 8, 0);
	line(ioImage, downRightCorner, downLeftCorner, scalarLine, 1, 8, 0);
	line(ioImage, downLeftCorner, upLeftCorner, scalarLine, 1, 8, 0);

	//Линии колес

	//Левое верхнее колесо
	line(ioImage, upLeftCorner, upLeftWheel[0], scalarLine, 1, 8, 0);
	line(ioImage, upLeftWheel[0], upLeftWheel[1], scalarLine, 1, 8, 0);
	line(ioImage, upLeftWheel[1], upLeftWheel[2], scalarLine, 1, 8, 0);

	//Правое верхнее колесо
	line(ioImage, upRightCorner, upRightWheel[0], scalarLine, 1, 8, 0);
	line(ioImage, upRightWheel[0], upRightWheel[1], scalarLine, 1, 8, 0);
	line(ioImage, upRightWheel[1], upRightWheel[2], scalarLine, 1, 8, 0);

	//Левое нижнее колесо
	line(ioImage, downLeftCorner, downLeftWheel[0], scalarLine, 1, 8, 0);
	line(ioImage, downLeftWheel[0], downLeftWheel[1], scalarLine, 1, 8, 0);
	line(ioImage, downLeftWheel[1], downLeftWheel[2], scalarLine, 1, 8, 0);

	//Правое нижнее колесо
	line(ioImage, downRightCorner, downRightWheel[0], scalarLine, 1, 8, 0);
	line(ioImage, downRightWheel[0], downRightWheel[1], scalarLine, 1, 8, 0);
	line(ioImage, downRightWheel[1], downRightWheel[2], scalarLine, 1, 8, 0);

	////Линии треугольника
	//line(ioImage, triangle[0], triangle[1], scalarLine, 1, 8, 0);
	//line(ioImage, triangle[1], triangle[2], scalarLine, 1, 8, 0);
	//line(ioImage, triangle[2], triangle[0], scalarLine, 1, 8, 0);

	vector<cv::Point> triangleArea;

	triangleArea.push_back(Point2f(200, 200));
	triangleArea.push_back(Point2f(300, 200));
	triangleArea.push_back(Point2f(200, 300));

	polylines(ioImage, triangleArea, true, scalarLine);

	switch (m_key)
	{
	case 'w':
		if (checkBorders(upLeftWheel[0], triangleArea, robotArea) || checkBorders(upRightWheel[0], triangleArea, robotArea))
		{
			move(0, 1);
		}

		break;
	case 'a':
		if (checkBorders(upLeftWheel[0], triangleArea, robotArea) || checkBorders(downLeftWheel[0], triangleArea, robotArea))
		{
			move(1, 0);
		}
		break;
	case 's':
		if (checkBorders(downLeftWheel[0], triangleArea, robotArea) || checkBorders(downRightWheel[2], triangleArea, robotArea))
		{
			move(0, -1);
		}
		break;
	case 'd':
		if (checkBorders(downRightWheel[2], triangleArea, robotArea) || checkBorders(upRightWheel[0], triangleArea, robotArea))
		{
			move(-1, 0);
		}
		break;
	case 'y':
		if (checkBorders(downRightWheel[2], triangleArea, robotArea) || checkBorders(upRightWheel[0], triangleArea, robotArea) ||
			checkBorders(downLeftWheel[0], triangleArea, robotArea) || checkBorders(downRightWheel[2], triangleArea, robotArea))
		{
			rotate(1);
		}
		break;
	case 'u':
		if (checkBorders(downRightWheel[2], triangleArea, robotArea) || checkBorders(upRightWheel[0], triangleArea, robotArea) ||
			checkBorders(downLeftWheel[0], triangleArea, robotArea) || checkBorders(downRightWheel[2], triangleArea, robotArea))
		{
			rotate(-1);
		}
		break;
	default:
		break;
	}

	return 0;
}

char key = 0;

int main()
{
	MyRobot robot(50,100,10,20,10,10);
	Size size(800, 600);
	robot.setArea(size);
	robot.setCenter(400, 300);

	Scalar scalar(255, 255, 255);

	while (waitKey(1) != 27)
	{	
		Mat image(size, CV_8UC3, scalar);

		key = waitKey(10);

		switch(key)
		{
		case 'w':
			robot.move(0, -1);
			break;
		case 'a':
			robot.move(-1, 0);
			break;
		case 's':
			robot.move(0, 1);
			break;
		case 'd':
			robot.move(1, 0);
			break;
		case 'y':
			robot.rotate(-1);
			break;
		case 'u':
			robot.rotate(1);
			break;
		default:
			break;
		}
		robot.draw(image, key);
		imshow("Robot", image);
	}

	return 0;
}
