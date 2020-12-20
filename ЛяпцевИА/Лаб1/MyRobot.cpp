#pragma once

#include "MyRobot.h"

MyRobot::MyRobot(float width, float height, float wheelWidth, float wheelDiameter, float speed, float angularSpeed) :
	m_width(width), m_height(height), m_wheelWidth(wheelWidth), m_wheelDiameter(wheelDiameter), m_speed(speed), m_angularSpeed(angularSpeed)
{
	;
};

MyRobot::~MyRobot()
{
	;
};

void MyRobot::setSpeed(const float speed) // Проверка знака скорости - добавить
{
	m_speed = speed;
};

void MyRobot::setAngularSpeed(const float angularSeed)
{
	m_angularSpeed = angularSeed;
};

void MyRobot::setArea(const Size2i area)
{
	m_area = area;
};

int MyRobot::setArea(Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	m_area.width = image.cols;
	m_area.height = image.rows;

	return 0;
};

int MyRobot::setCenter(Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	if ((image.cols / 2 > m_area.width) || (image.rows / 2 > m_area.height))
	{
		return -2;
	}

	m_center.x = image.cols / 2;
	m_center.y = image.rows / 2;

	return 0;
};

int MyRobot::setCenter(float x, float y) // Не факт, что было в точности так
{
	if ((x > m_area.width) || (y > m_area.height))
	{
		return -2;
	}

	m_center.x = x;
	m_center.y = y;

	return 0;
};

float MyRobot::getSpeed()
{
	return m_speed;
};
float MyRobot::getAngularSpeed()
{
	return m_angularSpeed;
};

int MyRobot::move()
{
	return 0;
};

int MyRobot::rotate()
{
	return 0;
};

int MyRobot::draw(Mat& iImage, Mat& oImage)
{
	if (iImage.empty())
	{
		return -1;
	}

	if ((iImage.cols != m_area.width) || (iImage.rows != m_area.height))
	{
		return -2;
	}

	return 0;
}
