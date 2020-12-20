#pragma once
#include "my_robot.h"

const double pi = 3.141592653589793238463;

MyRobot::MyRobot(float width, float height, float wheelWidth, float wheelDiameter, float speed = 0, float angularSpeed = 0):
m_width (width), m_height (height), m_wheelWidth (wheelWidth), m_wheelDiameter (wheelDiameter), m_speed (speed), m_angularSpeed (angularSpeed)
{
	m_angle = 0;
	;
}
MyRobot::~MyRobot()
{
	;
}

// Сеттеры 
void MyRobot::setSpeed(const float  speed)
{
	m_speed = speed;
}

void MyRobot::setAngularSpeed(const float  angularSpeed)
{
	m_angularSpeed = angularSpeed;
}

void MyRobot::setArea(cv::Size2i area)
{
	m_area = area;
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
	m_center.x = image.cols/4;
	m_center.y = image.rows/2;
	return 0;
}

int MyRobot::setCircleCenter(Mat image)
{
	if (image.empty())
	{
		return -1;
	}

	if ((image.cols / 2 > m_area.width) || (image.rows / 2 > m_area.height))
	{
		return -2;
	}
	m_circleCenter.x = image.cols / 2;
	m_circleCenter.y = image.rows / 2;
	return 0;
}

int MyRobot::setCenter(float x, float y)
{
	if ((x > m_area.width) || (y > m_area.height))
	{
		return -2;
	}

	m_center.x = x;
	m_center.y = y;

	return 0;
};

// Геттеры 
float MyRobot::getSpeed(float speed)
{
	return m_speed;
};
float MyRobot::getAngularSpeed(float angularSpeed)
{
	return m_angularSpeed;
};

int MyRobot::motion(float x, float y)
{
	return 0;
}

int MyRobot::move(int8_t number)
{
	switch (number)
	{
		// вверх
	case 1:
		m_center.x = m_center.x - m_speed * sin(m_angle);
		m_center.y = m_center.y + m_speed * cos(m_angle);

		break;

		// влево
	case 2:
		m_center.x = m_center.x - m_speed * cos(m_angle);
		m_center.y = m_center.y - m_speed * sin(m_angle);

		break;

		// вниз
	case 3:
		m_center.x = m_center.x + m_speed * sin(m_angle);
		m_center.y = m_center.y - m_speed * cos(m_angle);

		break;

		// вправо
	case 4:
		m_center.x = m_center.x + m_speed * cos(m_angle);
		m_center.y = m_center.y + m_speed * sin(m_angle);

		break;
		break;
	}
	return 0;
};

int MyRobot::rotate(int8_t number)
{
	switch (number)
	{
		// для поворота по часовой
	case 1:
		m_angle = m_angle - m_angularSpeed * pi / 180;
		break;

		// для поворота против часовой
	case 2:
		m_angle = m_angle + m_angularSpeed * pi / 180;
		break;

		break;
	}

	return 0;
};


// Отрисовка
int MyRobot::draw(const Mat& inputImage, Mat& outputImage)
{
	if (inputImage.empty())
	{
		return -1;
	}
	outputImage = inputImage.clone();

	if ((inputImage.cols != m_area.width) || (inputImage.rows != m_area.height))
	{
		return -2;
	}

	// задание точки центра окружностей
	Point2i circleCenter;

	circleCenter.x = m_circleCenter.x;
	circleCenter.y = m_circleCenter.y;

	int radius1 = 50;
	int radius2 = 300;

	// задание точек углов робота
	Point2i pointTopLeft;
	Point2i pointTopRight;
	Point2i pointBotLeft;
	Point2i pointBotRight;

	// тело

	pointTopLeft.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (m_height / 2);
	pointTopLeft.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (m_height / 2);

	pointTopRight.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (m_height / 2);
	pointTopRight.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (m_height / 2);

	pointBotLeft.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (-m_height / 2);
	pointBotLeft.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (-m_height / 2);

	pointBotRight.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (-m_height / 2);
	pointBotRight.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (-m_height / 2);


	// задание точек для колёс
	Point2i pointTopLeftWheelLT;
	Point2i pointTopRightWheelLT;
	Point2i pointBotLeftWheelLT;
	Point2i pointBotRightWheelLT;

	Point2i pointTopLeftWheelLB;
	Point2i pointTopRightWheelLB;
	Point2i pointBotLeftWheelLB;
	Point2i pointBotRightWheelLB;

	Point2i pointTopLeftWheelRT;
	Point2i pointTopRightWheelRT;
	Point2i pointBotLeftWheelRT;
	Point2i pointBotRightWheelRT;

	Point2i pointTopLeftWheelRB;
	Point2i pointTopRightWheelRB;
	Point2i pointBotLeftWheelRB;
	Point2i pointBotRightWheelRB;

	// левое верхнее колесо

	pointTopLeftWheelLT.x = m_center.x + cos(m_angle) * ( - m_width / 2 - m_wheelWidth) - sin(m_angle) * ( m_height / 2);
	pointTopLeftWheelLT.y = m_area.height - m_center.y - sin(m_angle) * ( - m_width / 2 - m_wheelWidth) - cos(m_angle) * ( m_height / 2);

	pointTopRightWheelLT.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (m_height / 2);
	pointTopRightWheelLT.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (m_height / 2);

	pointBotLeftWheelLT.x = m_center.x + cos(m_angle) * (-m_width / 2 - m_wheelWidth) - sin(m_angle) * (m_height / 2 - m_wheelDiameter);
	pointBotLeftWheelLT.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2 - m_wheelWidth) - cos(m_angle) * (m_height / 2 - m_wheelDiameter);

	pointBotRightWheelLT.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (m_height / 2 - m_wheelDiameter);
	pointBotRightWheelLT.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (m_height / 2 - m_wheelDiameter);

	// левое нижнее колесо

	pointBotLeftWheelLB.x = m_center.x + cos(m_angle) * (-m_width / 2 - m_wheelWidth) - sin(m_angle) * (-m_height / 2);
	pointBotLeftWheelLB.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2 - m_wheelWidth) - cos(m_angle) * (-m_height / 2);

	pointBotRightWheelLB.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (-m_height / 2);
	pointBotRightWheelLB.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (-m_height / 2);

	pointTopLeftWheelLB.x = m_center.x + cos(m_angle) * (-m_width / 2 - m_wheelWidth) - sin(m_angle) * (-m_height / 2 + m_wheelDiameter);
	pointTopLeftWheelLB.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2 - m_wheelWidth) - cos(m_angle) * (- m_height / 2 + m_wheelDiameter);

	pointTopRightWheelLB.x = m_center.x + cos(m_angle) * (-m_width / 2) - sin(m_angle) * (-m_height / 2 + m_wheelDiameter);
	pointTopRightWheelLB.y = m_area.height - m_center.y - sin(m_angle) * (-m_width / 2) - cos(m_angle) * (-m_height / 2 + m_wheelDiameter);

	// правое верхнее колесо

	pointTopLeftWheelRT.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (m_height / 2);
	pointTopLeftWheelRT.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (m_height / 2);

	pointTopRightWheelRT.x = m_center.x + cos(m_angle) * (m_width / 2 + m_wheelWidth) - sin(m_angle) * (m_height / 2);
	pointTopRightWheelRT.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2 + m_wheelWidth) - cos(m_angle) * (m_height / 2);

	pointBotLeftWheelRT.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (m_height / 2 - m_wheelDiameter);
	pointBotLeftWheelRT.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (m_height / 2 - m_wheelDiameter);

	pointBotRightWheelRT.x = m_center.x + cos(m_angle) * (m_width / 2 + m_wheelWidth) - sin(m_angle) * (m_height / 2 - m_wheelDiameter);
	pointBotRightWheelRT.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2 + m_wheelWidth) - cos(m_angle) * (m_height / 2 - m_wheelDiameter);

	// правое нижнее колесо

	pointBotLeftWheelRB.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (-m_height / 2);
	pointBotLeftWheelRB.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (-m_height / 2);

	pointBotRightWheelRB.x = m_center.x + cos(m_angle) * (m_width / 2 + m_wheelWidth) - sin(m_angle) * (-m_height / 2);
	pointBotRightWheelRB.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2 + m_wheelWidth) - cos(m_angle) * (-m_height / 2);

	pointTopLeftWheelRB.x = m_center.x + cos(m_angle) * (m_width / 2) - sin(m_angle) * (-m_height / 2 + m_wheelDiameter);
	pointTopLeftWheelRB.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2) - cos(m_angle) * (-m_height / 2 + m_wheelDiameter);

	pointTopRightWheelRB.x = m_center.x + cos(m_angle) * (m_width / 2 + m_wheelWidth) - sin(m_angle) * (-m_height / 2 + m_wheelDiameter);
	pointTopRightWheelRB.y = m_area.height - m_center.y - sin(m_angle) * (m_width / 2 + m_wheelWidth) - cos(m_angle) * (-m_height / 2 + m_wheelDiameter);

	static float tempCenterX = m_center.x;
	static float tempCenterY = m_center.y;
	static float tempAngle = m_angle;

	static float tempPointTopLeftX = pointTopLeft.x;
	static float tempPointTopLeftY = pointTopLeft.y;
	static float tempPointTopRightX = pointTopRight.x;
	static float tempPointTopRightY = pointTopRight.y;
	static float tempPointBotLeftX = pointBotLeft.x;
	static float tempPointBotLeftY = pointBotLeft.y;
	static float tempPointBotRightX = pointBotRight.x;
	static float tempPointBotRightY = pointBotRight.y;

	static float tempPointTopLeftWheelLTX = pointTopLeftWheelLT.x;
	static float tempPointTopLeftWheelLTY = pointTopLeftWheelLT.y;
	static float tempPointTopRightWheelLTX = pointTopRightWheelLT.x;
	static float tempPointTopRightWheelLTY = pointTopRightWheelLT.y;
	static float tempPointBotLeftWheelLTX = pointBotLeftWheelLT.x;
	static float tempPointBotLeftWheelLTY = pointBotLeftWheelLT.y;
	static float tempPointBotRightWheelLTX = pointBotRightWheelLT.x;
	static float tempPointBotRightWheelLTY = pointBotRightWheelLT.y;

	static float tempPointTopLeftWheelRTX = pointTopLeftWheelRT.x;
	static float tempPointTopLeftWheelRTY = pointTopLeftWheelRT.y;
	static float tempPointTopRightWheelRTX = pointTopRightWheelRT.x;
	static float tempPointTopRightWheelRTY = pointTopRightWheelRT.y;
	static float tempPointBotLeftWheelRTX = pointBotLeftWheelRT.x;
	static float tempPointBotLeftWheelRTY = pointBotLeftWheelRT.y;
	static float tempPointBotRightWheelRTX = pointBotRightWheelRT.x;
	static float tempPointBotRightWheelRTY = pointBotRightWheelRT.y;

	static float tempPointTopLeftWheelLBX = pointTopLeftWheelLB.x;
	static float tempPointTopLeftWheelLBY = pointTopLeftWheelLB.y;
	static float tempPointTopRightWheelLBX = pointTopRightWheelLB.x;
	static float tempPointTopRightWheelLBY = pointTopRightWheelLB.y;
	static float tempPointBotLeftWheelLBX = pointBotLeftWheelLB.x;
	static float tempPointBotLeftWheelLBY = pointBotLeftWheelLB.y;
	static float tempPointBotRightWheelLBX = pointBotRightWheelLB.x;
	static float tempPointBotRightWheelLBY = pointBotRightWheelLB.y;

	static float tempPointTopLeftWheelRBX = pointTopLeftWheelRB.x;
	static float tempPointTopLeftWheelRBY = pointTopLeftWheelRB.y;
	static float tempPointTopRightWheelRBX = pointTopRightWheelRB.x;
	static float tempPointTopRightWheelRBY = pointTopRightWheelRB.y;
	static float tempPointBotLeftWheelRBX = pointBotLeftWheelRB.x;
	static float tempPointBotLeftWheelRBY = pointBotLeftWheelRB.y;
	static float tempPointBotRightWheelRBX = pointBotRightWheelRB.x;
	static float tempPointBotRightWheelRBY = pointBotRightWheelRB.y;

	int dX1, dX2, dX3, dX4, dY1, dY2, dY3, dY4, dXW1, dXW2, dXW3, dXW4, dXW5, dXW6, dXW7, dXW8, dYW1, dYW2, dYW3, dYW4, dYW5, dYW6, dYW7, dYW8;
	float rPoint1, rPoint2, rPoint3, rPoint4, rPointW1, rPointW2, rPointW3, rPointW4, rPointW5, rPointW6, rPointW7, rPointW8;

	dX1 = pointTopLeft.x - circleCenter.x;
	dX2 = pointBotLeft.x - circleCenter.x;
	dX3 = pointTopRight.x - circleCenter.x;
	dX4 = pointBotRight.x - circleCenter.x;
	dY1 = pointTopLeft.y - circleCenter.y;
	dY2 = pointBotLeft.y - circleCenter.y;
	dY3 = pointTopRight.y - circleCenter.y;
	dY4 = pointBotRight.y - circleCenter.y;

	dXW1 = pointTopLeftWheelLT.x - circleCenter.x;
	dXW2 = pointBotLeftWheelLB.x - circleCenter.x;
	dXW3 = pointTopRightWheelRT.x - circleCenter.x;
	dXW4 = pointBotRightWheelRB.x - circleCenter.x;
	dXW5 = pointBotRightWheelRT.x - circleCenter.x;
	dXW6 = pointTopRightWheelRB.x - circleCenter.x;
	dXW7 = pointBotLeftWheelLT.x - circleCenter.x;
	dXW8 = pointTopLeftWheelLB.x - circleCenter.x;
	dYW1 = pointTopLeftWheelLT.y - circleCenter.y;
	dYW2 = pointBotLeftWheelLB.y - circleCenter.y;
	dYW3 = pointTopRightWheelRT.y - circleCenter.y;
	dYW4 = pointBotRightWheelRB.y - circleCenter.y;
	dYW5 = pointBotRightWheelRT.y - circleCenter.y;
	dYW6 = pointTopRightWheelRB.y - circleCenter.y;
	dYW7 = pointBotLeftWheelLT.y - circleCenter.y;
	dYW8 = pointTopLeftWheelLB.y - circleCenter.y;

	rPoint1 = sqrt(dX1 * dX1 + dY1 * dY1);
	rPoint2 = sqrt(dX2 * dX2 + dY2 * dY2);
	rPoint3 = sqrt(dX3 * dX3 + dY3 * dY3);
	rPoint4 = sqrt(dX4 * dX4 + dY4 * dY4);

	rPointW1 = sqrt(dXW1 * dXW1 + dYW1 * dYW1);
	rPointW2 = sqrt(dXW2 * dXW2 + dYW2 * dYW2);
	rPointW3 = sqrt(dXW3 * dXW3 + dYW3 * dYW3);
	rPointW4 = sqrt(dXW4 * dXW4 + dYW4 * dYW4);
	rPointW5 = sqrt(dXW5 * dXW5 + dYW5 * dYW5);
	rPointW6 = sqrt(dXW6 * dXW6 + dYW6 * dYW6);
	rPointW7 = sqrt(dXW7 * dXW7 + dYW7 * dYW7);
	rPointW8 = sqrt(dXW8 * dXW8 + dYW8 * dYW8);


	if ((pointTopLeftWheelLT.x  <= 0) || 
		(pointBotLeftWheelLB.x  <= 0) || 
		(pointTopRightWheelRT.x <= 0) || 
		(pointBotRightWheelRB.x <= 0) ||
		(pointTopLeftWheelLT.x >= m_area.width) || 
		(pointBotLeftWheelLB.x >= m_area.width) || 
		(pointTopRightWheelRT.x >= m_area.width) || 
		(pointBotRightWheelRB.x >= m_area.width) ||
		(pointTopLeftWheelLT.y <= 0) || 
		(pointBotLeftWheelLB.y <= 0) || 
		(pointTopRightWheelRT.y <= 0) || 
		(pointBotRightWheelRB.y <= 0) ||
		(pointTopLeftWheelLT.y >= m_area.height) ||
		(pointBotLeftWheelLB.y >= m_area.height) || 
		(pointTopRightWheelRT.y >= m_area.height) || 
		(pointBotRightWheelRB.y >= m_area.height) ||
		(rPoint1 < radius1) ||
		(rPoint2 < radius1) ||
		(rPoint3 < radius1) ||
		(rPoint4 < radius1) ||
		(rPoint1 > radius2) ||
		(rPoint1 > radius2) ||
		(rPoint1 > radius2) ||
		(rPoint1 > radius2) ||
		(rPointW1 < (radius1+1)) ||
		(rPointW2 < (radius1+1)) ||
		(rPointW3 < (radius1+1)) ||
		(rPointW4 < (radius1+1)) ||
		(rPointW1 > (radius2-1)) ||
		(rPointW2 > (radius2-1)) ||
		(rPointW3 > (radius2-1)) ||
		(rPointW4 > (radius2-1)) ||
		(rPointW5 > (radius2-1)) ||
		(rPointW6 > (radius2-1)) ||
		(rPointW7 > (radius2-1)) ||
		(rPointW8 > (radius2-1))
		)
	{
		m_center.x = tempCenterX;
		m_center.y = tempCenterY;
		m_angle = tempAngle;

		pointTopLeft.x = tempPointTopLeftX;
		pointTopLeft.y = tempPointTopLeftY;
		pointTopRight.x = tempPointTopRightX;
		pointTopRight.y = tempPointTopRightY;
		pointBotLeft.x = tempPointBotLeftX;
		pointBotLeft.y = tempPointBotLeftY;
		pointBotRight.x = tempPointBotRightX;
		pointBotRight.y = tempPointBotRightY;

		pointTopLeftWheelLT.x = tempPointTopLeftWheelLTX;
		pointTopLeftWheelLT.y = tempPointTopLeftWheelLTY;
		pointTopRightWheelLT.x = tempPointTopRightWheelLTX;
		pointTopRightWheelLT.y = tempPointTopRightWheelLTY;
		pointBotLeftWheelLT.x = tempPointBotLeftWheelLTX;
		pointBotLeftWheelLT.y = tempPointBotLeftWheelLTY;
		pointBotRightWheelLT.x = tempPointBotRightWheelLTX;
		pointBotRightWheelLT.y = tempPointBotRightWheelLTY;

		pointTopLeftWheelRT.x = tempPointTopLeftWheelRTX;
		pointTopLeftWheelRT.y = tempPointTopLeftWheelRTY;
		pointTopRightWheelRT.x = tempPointTopRightWheelRTX;
		pointTopRightWheelRT.y = tempPointTopRightWheelRTY;
		pointBotLeftWheelRT.x = tempPointBotLeftWheelRTX;
		pointBotLeftWheelRT.y = tempPointBotLeftWheelRTY;
		pointBotRightWheelRT.x = tempPointBotRightWheelRTX;
		pointBotRightWheelRT.y = tempPointBotRightWheelRTY;

		pointTopLeftWheelLB.x = tempPointTopLeftWheelLBX;
		pointTopLeftWheelLB.y = tempPointTopLeftWheelLBY;
		pointTopRightWheelLB.x = tempPointTopRightWheelLBX;
		pointTopRightWheelLB.y = tempPointTopRightWheelLBY;
		pointBotLeftWheelLB.x = tempPointBotLeftWheelLBX;
		pointBotLeftWheelLB.y = tempPointBotLeftWheelLBY;
		pointBotRightWheelLB.x = tempPointBotRightWheelLBX;
		pointBotRightWheelLB.y = tempPointBotRightWheelLBY;

		pointTopLeftWheelRB.x = tempPointTopLeftWheelRBX;
		pointTopLeftWheelRB.y = tempPointTopLeftWheelRBY;
		pointTopRightWheelRB.x = tempPointTopRightWheelRBX;
		pointTopRightWheelRB.y = tempPointTopRightWheelRBY;
		pointBotLeftWheelRB.x = tempPointBotLeftWheelRBX;
		pointBotLeftWheelRB.y = tempPointBotLeftWheelRBY;
		pointBotRightWheelRB.x = tempPointBotRightWheelRBX;
		pointBotRightWheelRB.y = tempPointBotRightWheelRBY;

	}

	tempCenterX = m_center.x;
	tempCenterY = m_center.y;
	tempAngle = m_angle;

	tempPointTopLeftX = pointTopLeft.x;
	tempPointTopLeftY = pointTopLeft.y;
	tempPointTopRightX = pointTopRight.x;
	tempPointTopRightY = pointTopRight.y;
	tempPointBotLeftX = pointBotLeft.x;
	tempPointBotLeftY = pointBotLeft.y;
	tempPointBotRightX = pointBotRight.x;
	tempPointBotRightY = pointBotRight.y;

	tempPointTopLeftWheelLTX = pointTopLeftWheelLT.x;
	tempPointTopLeftWheelLTY = pointTopLeftWheelLT.y;
	tempPointTopRightWheelLTX = pointTopRightWheelLT.x;
	tempPointTopRightWheelLTY = pointTopRightWheelLT.y;
	tempPointBotLeftWheelLTX = pointBotLeftWheelLT.x;
	tempPointBotLeftWheelLTY = pointBotLeftWheelLT.y;
	tempPointBotRightWheelLTX = pointBotRightWheelLT.x;
	tempPointBotRightWheelLTY = pointBotRightWheelLT.y;

	tempPointTopLeftWheelRTX = pointTopLeftWheelRT.x;
	tempPointTopLeftWheelRTY = pointTopLeftWheelRT.y;
	tempPointTopRightWheelRTX = pointTopRightWheelRT.x;
	tempPointTopRightWheelRTY = pointTopRightWheelRT.y;
	tempPointBotLeftWheelRTX = pointBotLeftWheelRT.x;
	tempPointBotLeftWheelRTY = pointBotLeftWheelRT.y;
	tempPointBotRightWheelRTX = pointBotRightWheelRT.x;
	tempPointBotRightWheelRTY = pointBotRightWheelRT.y;

	tempPointTopLeftWheelLBX = pointTopLeftWheelLB.x;
	tempPointTopLeftWheelLBY = pointTopLeftWheelLB.y;
	tempPointTopRightWheelLBX = pointTopRightWheelLB.x;
	tempPointTopRightWheelLBY = pointTopRightWheelLB.y;
	tempPointBotLeftWheelLBX = pointBotLeftWheelLB.x;
	tempPointBotLeftWheelLBY = pointBotLeftWheelLB.y;
	tempPointBotRightWheelLBX = pointBotRightWheelLB.x;
	tempPointBotRightWheelLBY = pointBotRightWheelLB.y;

	tempPointTopLeftWheelRBX = pointTopLeftWheelRB.x;
	tempPointTopLeftWheelRBY = pointTopLeftWheelRB.y;
	tempPointTopRightWheelRBX = pointTopRightWheelRB.x;
	tempPointTopRightWheelRBY = pointTopRightWheelRB.y;
	tempPointBotLeftWheelRBX = pointBotLeftWheelRB.x;
	tempPointBotLeftWheelRBY = pointBotLeftWheelRB.y;
	tempPointBotRightWheelRBX = pointBotRightWheelRB.x;
	tempPointBotRightWheelRBY = pointBotRightWheelRB.y;


	// характеристики линий
	Scalar black = (0, 0, 0);
	int8_t thickness = 2;
	int8_t lineType = 1;
	int8_t shift = 0;
	

	// рисование
	line(outputImage, pointTopLeft, pointTopRight, black, thickness, lineType, shift); // верхняя линия
	line(outputImage, pointTopLeft, pointBotLeft, black, thickness, lineType, shift); // левая линия
	line(outputImage, pointTopRight, pointBotRight, black, thickness, lineType, shift); // правая линия
	line(outputImage, pointBotLeft, pointBotRight, black, thickness, lineType, shift); // нижняя линяя
	// левое верхнее колесо
	line(outputImage, pointTopLeftWheelLT, pointTopRightWheelLT, black, thickness, lineType, shift); 
	line(outputImage, pointTopLeftWheelLT, pointBotLeftWheelLT, black, thickness, lineType, shift);
	line(outputImage, pointBotLeftWheelLT, pointBotRightWheelLT, black, thickness, lineType, shift);
	// левое нижнее колесо
	line(outputImage, pointTopLeftWheelLB, pointTopRightWheelLB, black, thickness, lineType, shift); 
	line(outputImage, pointTopLeftWheelLB, pointBotLeftWheelLB, black, thickness, lineType, shift);
	line(outputImage, pointBotLeftWheelLB, pointBotRightWheelLB, black, thickness, lineType, shift);
	// правое верхнее колесо
	line(outputImage, pointTopLeftWheelRT, pointTopRightWheelRT, black, thickness, lineType, shift);
	line(outputImage, pointTopRightWheelRT, pointBotRightWheelRT, black, thickness, lineType, shift);
	line(outputImage, pointBotRightWheelRT, pointBotLeftWheelRT, black, thickness, lineType, shift);
	// правое нижнее колесо
	line(outputImage, pointTopLeftWheelRB, pointTopRightWheelRB, black, thickness, lineType, shift);
	line(outputImage, pointTopRightWheelRB, pointBotRightWheelRB, black, thickness, lineType, shift);
	line(outputImage, pointBotRightWheelRB, pointBotLeftWheelRB, black, thickness, lineType, shift);

	// окружности
	circle(outputImage, circleCenter, radius1, black, thickness, lineType, shift);
	circle(outputImage, circleCenter, radius2, black, thickness, lineType, shift);

	return 0;
}