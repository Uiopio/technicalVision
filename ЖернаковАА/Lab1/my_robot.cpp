#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "my_robot.h"


#define pi 3.14

using namespace cv;
using namespace std;

MyRobot::MyRobot()
{

}

MyRobot::MyRobot(float m_width, float m_hight,
				 float m_wheelWidth, float m_wheelDiameter,
				 float m_speed, float m_angularSpeed, float m_angle) :

	m_width(m_width),
	m_height(m_hight),
	m_wheelWidth(m_wheelWidth),
	m_wheelDiameter(m_wheelDiameter),
	m_speed(m_speed),
	m_angularSpeed(m_angularSpeed),
	m_angle(m_angle)
{
	;
}

MyRobot::~MyRobot()
{
	;
}

void MyRobot::setSpeed(const float speed)
{
	m_speed = speed;
}

void MyRobot::setAngularSpeed(const float angularSpeed)
{
	m_angularSpeed = angularSpeed;
}

void MyRobot::setArea(const Size2i area)
{
	m_area = area;
}

int MyRobot::setArea(Mat image)
{
	if (image.empty())
	{
		return -1;
	}
	m_area.width = image.cols;
	m_area.height = image.rows;
	return 0;
}

int MyRobot::setCenter(Mat image)
{
	if (image.empty())
	{
		return -1;
	}
	if ((image.cols / 2 > m_area.width) ||
		image.rows / 2 > m_area.height)
	{
		return -2;
	}
	m_center.x = image.cols / 2;
	m_center.y = image.rows / 2;
	return 0;
}

int MyRobot::setCenter(float x, float y)
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

float MyRobot::getAngularSpeed()
{
	return m_angularSpeed;
}

Point2f MyRobot::calculationNewCoord(float localWidth, float localHeight)
{
	Point2f pt;
	Mat MatrixRotate = (Mat_<float>(3, 3) <<
		cos(m_angle), -sin(m_angle), m_center.x,
		sin(m_angle), cos(m_angle), m_center.y,
		0, 0, 1);
	Mat A = (Mat_<float>(3, 1) <<
		localWidth / 2,
		localHeight / 2,
		1);
	Mat Apt = MatrixRotate * A;
	pt.x = Apt.at<float>(0, 0);
	pt.y = Apt.at<float>(1, 0);
	return pt;
}

void MyRobot::wayEllipse(Mat& inputImage,float width, float height, int i)
{	
	ellipse(inputImage, Point2f(inputImage.cols / 2, inputImage.rows / 2), Size2f(width / 2, height / 2), 0, 0, 360, Scalar(150, 150, 150), 1, 8, 0);
	imshow("Map", inputImage);
	m_center.x = m_area.width/2 + width/2*cos(i*pi/180);
	m_center.y = m_area.height/2 + height / 2 * sin(i*pi/180);

	while (waitKey(1) != 98 )
	{
		;
	}
}

int MyRobot::draw(Mat& inputImage, Mat& outputImage)
{
	if (inputImage.empty())
	{
		return -1;
	}
	if ((inputImage.cols != m_area.width) ||
		(inputImage.rows != m_area.height))
	{
		return -2;
	}
	//расчет координат точек прямоугольника робота
	Point2f pt1 = calculationNewCoord(-m_width, m_height);
	Point2f pt2 = calculationNewCoord(m_width, m_height);
	Point2f pt3 = calculationNewCoord(m_width, -m_height);
	Point2f pt4 = calculationNewCoord(-m_width, -m_height);
	//левое нижнее колесо
	Point2f pt5 = calculationNewCoord(-m_width - 2*m_wheelWidth, m_height);
	Point2f pt6 = calculationNewCoord(-m_width - 2*m_wheelWidth, m_height - 2*m_wheelDiameter);
	Point2f pt7 = calculationNewCoord(-m_width, m_height-2*m_wheelDiameter);
	//правое нижнее колесо
	Point2f pt8 = calculationNewCoord( m_width + 2 * m_wheelWidth, m_height);
	Point2f pt9 = calculationNewCoord(m_width + 2 * m_wheelWidth, m_height - 2 * m_wheelDiameter);
	Point2f pt10 = calculationNewCoord(m_width, m_height - 2 * m_wheelDiameter);
	//правое верхнее колесо
	Point2f pt11 = calculationNewCoord(m_width + 2 * m_wheelWidth, -m_height);
	Point2f pt12 = calculationNewCoord(m_width + 2 * m_wheelWidth, -m_height + 2 * m_wheelDiameter);
	Point2f pt13 = calculationNewCoord(m_width, -m_height + 2 * m_wheelDiameter);
	//левое верхнее колесо
	Point2f pt14 = calculationNewCoord(-m_width - 2 * m_wheelWidth, -m_height);
	Point2f pt15 = calculationNewCoord(-m_width - 2 * m_wheelWidth, -m_height + 2 * m_wheelDiameter);
	Point2f pt16 = calculationNewCoord(-m_width, -m_height + 2 * m_wheelDiameter);

	line(outputImage, pt1, pt2, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt2, pt3, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt3, pt4, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt1, pt4, Scalar(0, 0, 255), 2, 8, 0);

	line(outputImage, pt1, pt5, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt5, pt6, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt6, pt7, Scalar(0, 0, 255), 2, 8, 0);

	line(outputImage, pt2, pt8, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt8, pt9, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt9, pt10, Scalar(0, 0, 255), 2, 8, 0);

	line(outputImage, pt3, pt11, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt11, pt12, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt12, pt13, Scalar(0, 0, 255), 2, 8, 0);

	line(outputImage, pt4, pt14, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt14, pt15, Scalar(0, 0, 255), 2, 8, 0);
	line(outputImage, pt15, pt16, Scalar(0, 0, 255), 2, 8, 0);

	Rect rect13(pt5, pt11);
	Rect rect24(pt8, pt14);

	isA = true;
	isD = true;
	isW = true;
	isS = true;
	isQ = true;
	isE = true;

	//проверка пересечения правой границы
	if (m_center.x + rect13.width / 2 >= m_area.width - m_speed || m_center.x + rect24.width / 2 >= m_area.width - m_speed)
	{
		isOutOfRightBorder = true;
		if (pt1.x < pt2.x)
		{
			isA = true;
			isD = false;
		}
		else if(pt1.x > pt2.x)
		{
			isA = false;
			isD = true;
		}
		if (pt1.x < pt4.x)
		{
			isW = false;
			isS = true;
		}
		else if(pt1.x > pt4.x)
		{
			isW = true;
			isS = false;
		}
		if (isA && isW || !isA && !isW)
		{
			isE = true;
			isQ = false;
		}
		else
		{
			isE = false;
			isQ = true;
		}
	}
	else
	{
		isOutOfRightBorder = false;
	}
	//проверка пересечения левой границы
	if (m_center.x - rect13.width / 2 <= m_speed || m_center.x - rect24.width / 2 <= m_speed)
	{
		isOutOfLeftBorder = true;
		if (pt1.x < pt2.x)
		{
			isA = false;
			isD = true;
		}
		else if(pt1.x > pt2.x)
		{
			isA = true;
			isD = false;
		}
		if (pt1.x < pt4.x)
		{
			isW = true;
			isS = false;
		}
		else if(pt1.x > pt4.x)
		{
			isW = false;
			isS = true;
		}
		if (isA && isW || !isA && !isW)
		{
			isE = true;
			isQ = false;
		}
		else
		{
			isE = false;
			isQ = true;
		}
	}
	else
	{
		isOutOfLeftBorder = false;
	}
	//проверка пересечения верхней границы
	if (m_center.y - rect13.height / 2 <= m_speed || m_center.y - rect24.height / 2 <= m_speed)
	{
		isOutOfUpBorder = true;
		if (pt4.y < pt3.y)
		{
			isA = false;
			isD = true;
		}
		else if(pt4.y > pt3.y)
		{
			isA = true;
			isD = false;
		}
		if (pt1.y < pt4.y)
		{
			isW = true;
			isS = false;
		}
		else if(pt1.y > pt4.y)
		{
			isW = false;
			isS = true;
		}
		if (isA && isW || !isA && !isW)
		{
			isE = true;
			isQ = false;
		}
		else
		{
			isE = false;
			isQ = true;
		}
	}
	else
	{
		isOutOfUpBorder = false;
	}
	//проверка пересечения нижней границы
	if (m_center.y + rect13.height / 2 >= m_area.height - m_speed || m_center.y + rect24.height / 2 >= m_area.height - m_speed)
	{
		isOutOfDownBorder = true;
		if (pt4.y > pt3.y)
		{
			isA = false;
			isD = true;
		}
		else if(pt4.y < pt3.y)
		{
			isA = true;
			isD = false;
		}
		if (pt4.y > pt1.y)
		{
			isW = false;
			isS = true;
		}
		else if(pt4.y < pt1.y)
		{
			isW = true;
			isS = false;
		}
		if (isA && isW || !isA && !isW)
		{
			isE = true;
			isQ = false;
		}
		else
		{
			isE = false;
			isQ = true;
		}
	}
	else
	{
		isOutOfDownBorder = false;
	}

	return 0;
}

int MyRobot::move()
{
	char pressedKeyMove = waitKey(0);
	if (pressedKeyMove == 'a' || pressedKeyMove == 'A')
	{
		if (isA)
		{
			m_center.x = m_center.x - m_speed * cos(m_angle);
			m_center.y = m_center.y - m_speed * sin(m_angle);
		}
	}

	if (pressedKeyMove == 'd' || pressedKeyMove == 'D')
	{
		if (isD)
		{
			m_center.x = m_center.x + m_speed * cos(m_angle);
			m_center.y = m_center.y + m_speed * sin(m_angle);
		}
	}

	if (pressedKeyMove == 'w' || pressedKeyMove == 'W')
	{
		if (isW)
		{
			m_center.x = m_center.x + m_speed * sin(m_angle);
			m_center.y = m_center.y - m_speed * cos(m_angle);
		}
	}

	if (pressedKeyMove == 's' || pressedKeyMove == 'S')
	{
		if (isS)
		{
			m_center.x = m_center.x - m_speed * sin(m_angle);
			m_center.y = m_center.y + m_speed * cos(m_angle);
		}
	}
	return 0;
}

int MyRobot::rotate()
{
	char pressedKeyRotate = waitKey(0);
	if (pressedKeyRotate == 'e' || pressedKeyRotate == 'E')
	{
		if(isE)
			m_angle = m_angle + m_angularSpeed * pi / 180;
	}
	if (pressedKeyRotate == 'q' || pressedKeyRotate == 'Q')
	{
		if(isQ)
			m_angle = m_angle - m_angularSpeed * pi / 180;
	}
	return 0;
}