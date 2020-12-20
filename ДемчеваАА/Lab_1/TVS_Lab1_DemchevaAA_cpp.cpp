#pragma once
#include "my_robot.h"
#include <math.h>


MyRobot::MyRobot(int8_t width, int8_t height,
	float wheelWidth, float wheelDiameter):
	m_width(width),
	m_height(height),
	m_wheelWidth(),
	m_wheelDiameter()
{
	;
};

MyRobot::~MyRobot()
{
	;
};

//Сеттеры
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

void MyRobot::setBorders(int16_t xBorder, int16_t yBorder)
{
	m_topBorder = yBorder;
	m_bottomBorder = m_area.height - yBorder;

	m_leftBorder = xBorder;
	m_rightBorder = m_area.width - xBorder;

	return;
}

int MyRobot::setStartPoint(Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	if ((image.cols / 2 > m_area.width) || (image.rows / 2 > m_area.height))
	{
		return -2;
	}

	m_center.x = (image.cols)/2;
	m_center.y = (image.rows)/2;

	m_pt1_loc.x = - m_halfWidth;
	m_pt1_loc.y = m_halfHeight;

	m_pt2_loc.x = m_halfWidth;
	m_pt2_loc.y = m_halfHeight;

	m_pt3_loc.x = m_halfWidth;
	m_pt3_loc.y = - m_halfHeight;

	m_pt4_loc.x = -m_halfWidth;
	m_pt4_loc.y = -m_halfHeight;

	m_angle = 0;

	//Точки для колес:

//m_pt5_loc.x = - m_halfWidth - m_wheelWidth;
//	//m_pt5_loc.x = -m_halfWidth - 20;
//	m_pt5_loc.y = m_halfHeight;
//
//	m_pt6_loc.x = - m_halfWidth - m_wheelWidth;
//	m_pt6_loc.y = m_halfHeight - m_wheelDiameter;
//
//	m_pt7_loc.x = -m_halfWidth;
//	m_pt7_loc.y = m_halfHeight - m_wheelDiameter;


	return 0;
}

int MyRobot::setStartPoint(float x, float y)
{
	if ((x > m_area.width) || (y > m_area.height))
	{
		return -2;
	}

	m_center.x = x;
	m_center.y = y;

	m_pt1_loc.x = -m_halfWidth;
	m_pt1_loc.y = m_halfHeight;

	m_pt2_loc.x = m_halfWidth;
	m_pt2_loc.y = m_halfHeight;

	m_pt3_loc.x = m_halfWidth;
	m_pt3_loc.y = -m_halfHeight;

	m_pt4_loc.x = -m_halfWidth;
	m_pt4_loc.y = -m_halfHeight;

	m_angle = 0;

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

void MyRobot::move(char key)
{

	if (key == 56) //перемещение вверх 8
	{
		if (m_isTBcrossed == true)
		{
			cout << "Пересечение верхней границы" << endl;
		}
		else
		{
			m_center.y = m_center.y - m_speed;
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isBBcrossed = false;
		}
	}

	if (key == 50) //перемещение вниз 2
	{
		if (m_isBBcrossed == true)
		{
			cout << "Пересечение нижней границы" << endl;
		}
		else
		{
			m_center.y = m_center.y + m_speed;
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isTBcrossed = false;
		}
	}

	if (key == 52) //перемещение влево 4
	{
		if (m_isLBcrossed == true)
		{
			cout << "Пересечение левой границы" << endl;
		}
		else
		{
			m_center.x -= m_speed;
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isRBcrossed = false;
		}
		
	}

	if (key == 54) //перемещение вправо 6
    {
		if (m_isRBcrossed == true)
		{
			cout << "Пересечение правой границы" << endl;
		}
		else
		{
			m_center.x += m_speed;
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isLBcrossed = false;
		} 
    }

	if (key == 51) // вращение против чс 3
    {
    	 m_angle += m_angularSpeed;
		 if (m_angle >= 6.28)
		 {
			 m_angle = 0;
		 }
		 cout << "Угол, рад = " << m_angle << endl;
    }

	if (key == 49) //вращение по чс  1
    {
    	 m_angle -= m_angularSpeed;
		 if (m_angle <= -6.28)
		 {
			 m_angle = 0;
		 }
		 cout << "Угол, рад = " << m_angle << endl;
    }
    
	m_pt1.x = m_pt1_loc.x * cos(m_angle) - m_pt1_loc.y * sin(m_angle) + m_center.x;
	m_pt1.y = m_pt1_loc.x * sin(m_angle) + m_pt1_loc.y * cos(m_angle) + m_center.y;

	m_pt2.x = m_pt2_loc.x * cos(m_angle) - m_pt2_loc.y * sin(m_angle) + m_center.x;
	m_pt2.y = m_pt2_loc.x * sin(m_angle) + m_pt2_loc.y * cos(m_angle) + m_center.y;

	m_pt3.x = m_pt3_loc.x * cos(m_angle) - m_pt3_loc.y * sin(m_angle) + m_center.x;
	m_pt3.y = m_pt3_loc.x * sin(m_angle) + m_pt3_loc.y * cos(m_angle) + m_center.y;

	m_pt4.x = m_pt4_loc.x * cos(m_angle) - m_pt4_loc.y * sin(m_angle) + m_center.x;
	m_pt4.y = m_pt4_loc.x * sin(m_angle) + m_pt4_loc.y * cos(m_angle) + m_center.y;


	m_pt5.x = m_pt5_loc.x * cos(m_angle) - m_pt5_loc.y * sin(m_angle) + m_center.x;
	m_pt5.y = m_pt5_loc.x * sin(m_angle) + m_pt5_loc.y * cos(m_angle) + m_center.y;

	m_pt6.x = m_pt6_loc.x * cos(m_angle) - m_pt6_loc.y * sin(m_angle) + m_center.x;
	m_pt6.y = m_pt6_loc.x * sin(m_angle) + m_pt6_loc.y * cos(m_angle) + m_center.y;

	m_pt7.x = m_pt7_loc.x * cos(m_angle) - m_pt7_loc.y * sin(m_angle) + m_center.x;
	m_pt7.y = m_pt7_loc.x * sin(m_angle) + m_pt7_loc.y * cos(m_angle) + m_center.y;

	

	//Проверка пересечения верхней границы
	if ((m_pt1.y < m_topBorder) || (m_pt2.y < m_topBorder) ||
		(m_pt3.y < m_topBorder) || (m_pt4.y < m_topBorder))
	{
		m_isTBcrossed = true;
	}

	//Проверка пересечения нижней границы
	if ((m_pt1.y > m_bottomBorder) || (m_pt2.y > m_bottomBorder) ||
		(m_pt3.y > m_bottomBorder) || (m_pt4.y > m_bottomBorder))
	{
		m_isBBcrossed = true;
	}

	//Проверка пересечения правой границы
	if ((m_pt1.x > m_rightBorder) || (m_pt2.x > m_rightBorder) ||
		(m_pt3.x > m_rightBorder) || (m_pt4.x > m_rightBorder))
	{
		m_isRBcrossed = true;
	}

	//Проверка пересечения левой границы
	if ((m_pt1.x < m_leftBorder) || (m_pt2.x < m_leftBorder) ||
		(m_pt3.x < m_leftBorder) || (m_pt4.x < m_leftBorder))
	{
		m_isLBcrossed = true;
	}

	return;
}

void MyRobot::moveSyn(char key)
{
	if (key == 54) //перемещение вправо 6
	{
		if (m_isRBcrossed == true)
		{
			cout << "Пересечение правой границы" << endl;
		}
		else
		{
			m_center.x += m_synStep;
			m_center.y = m_area.height*0.5 - m_synAmpl * sin(m_synAngFreq * m_center.x);
			m_angle = atan(-m_synAmpl * m_synAngFreq * cos(m_synAngFreq * m_center.x));
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isLBcrossed = false;
		}
	}

	if (key == 52) //перемещение влево 4
	{
		if (m_isLBcrossed == true)
		{
			cout << "Пересечение левой границы" << endl;
		}
		else
		{
			m_center.x -= m_synStep;
			m_center.y = m_area.height * 0.5 - m_synAmpl * sin(m_synAngFreq * m_center.x);
			m_angle = atan(-m_synAmpl * m_synAngFreq * cos(m_synAngFreq * m_center.x));
			cout << "Y = " << m_center.y << endl;
			cout << "X = " << m_center.x << endl << endl;
			m_isRBcrossed = false;
		}

	}

	
	m_pt1.x = m_pt1_loc.x * cos(m_angle) - m_pt1_loc.y * sin(m_angle) + m_center.x;
	m_pt1.y = m_pt1_loc.x * sin(m_angle) + m_pt1_loc.y * cos(m_angle) + m_center.y;

	m_pt2.x = m_pt2_loc.x * cos(m_angle) - m_pt2_loc.y * sin(m_angle) + m_center.x;
	m_pt2.y = m_pt2_loc.x * sin(m_angle) + m_pt2_loc.y * cos(m_angle) + m_center.y;

	m_pt3.x = m_pt3_loc.x * cos(m_angle) - m_pt3_loc.y * sin(m_angle) + m_center.x;
	m_pt3.y = m_pt3_loc.x * sin(m_angle) + m_pt3_loc.y * cos(m_angle) + m_center.y;

	m_pt4.x = m_pt4_loc.x * cos(m_angle) - m_pt4_loc.y * sin(m_angle) + m_center.x;
	m_pt4.y = m_pt4_loc.x * sin(m_angle) + m_pt4_loc.y * cos(m_angle) + m_center.y;


	/*m_pt5.x = m_pt5_loc.x * cos(m_angle) - m_pt5_loc.y * sin(m_angle) + m_center.x;
	m_pt5.y = m_pt5_loc.x * sin(m_angle) + m_pt5_loc.y * cos(m_angle) + m_center.y;

	m_pt6.x = m_pt6_loc.x * cos(m_angle) - m_pt6_loc.y * sin(m_angle) + m_center.x;
	m_pt6.y = m_pt6_loc.x * sin(m_angle) + m_pt6_loc.y * cos(m_angle) + m_center.y;

	m_pt7.x = m_pt7_loc.x * cos(m_angle) - m_pt7_loc.y * sin(m_angle) + m_center.x;
	m_pt7.y = m_pt7_loc.x * sin(m_angle) + m_pt7_loc.y * cos(m_angle) + m_center.y;*/


	//Проверка пересечения правой границы
	if ((m_pt1.x > m_rightBorder) || (m_pt2.x > m_rightBorder) ||
		(m_pt3.x > m_rightBorder) || (m_pt4.x > m_rightBorder))
	{
		m_isRBcrossed = true;
	}

	//Проверка пересечения левой границы
	if ((m_pt1.x < m_leftBorder) || (m_pt2.x < m_leftBorder) ||
		(m_pt3.x < m_leftBorder) || (m_pt4.x < m_leftBorder))
	{
		m_isLBcrossed = true;
	}

	return;
}

//int MyRobot::rotate()
//{
//
//}

int MyRobot::draw(Mat& ioImage)
{

	Scalar lineColor(0, 0, 0);

	line(ioImage, m_pt1, m_pt2, lineColor, 2, 8, 0);
	line(ioImage, m_pt2, m_pt3, lineColor, 2, 8, 0);
	line(ioImage, m_pt3, m_pt4, lineColor, 2, 8, 0);
	line(ioImage, m_pt4, m_pt1, lineColor, 2, 8, 0);

	/*line(ioImage, m_pt1, m_pt5, lineColor, 2, 8, 0);
	line(ioImage, m_pt5, m_pt6, lineColor, 2, 8, 0);
	line(ioImage, m_pt6, m_pt7, lineColor, 2, 8, 0);*/

	return 0;
}

