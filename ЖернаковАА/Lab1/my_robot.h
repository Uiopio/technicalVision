#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>

using namespace cv;
using namespace std;

class MyRobot
{
public:
	MyRobot();
	MyRobot(float m_width, float m_hight,
			float m_wheelWidth, float m_wheelDiameter,
			float m_speed, float m_angularSpeed, float m_angle);
	~MyRobot();

	bool isOutOfLeftBorder = false;
	bool isOutOfRightBorder = false;
	bool isOutOfUpBorder = false;
	bool isOutOfDownBorder = false;
	bool isW = true;
	bool isS = true;
	bool isD = true;
	bool isA = true;
	bool isQ = true;
	bool isE = true;

	void setSpeed(float speed);
	void setAngularSpeed(float angularSpeed);
	void setArea(Size2i area);
	int setArea(Mat image);
	int setCenter(Mat image);
	int setCenter(float x, float y);
	Point2f calculationNewCoord(float localWidth, float localHeight);
	float getSpeed();
	float getAngularSpeed();
	int move();
	int rotate();
	int draw(Mat& inputImage, Mat& outputImage);
	void wayEllipse(Mat& inputImage, float width, float height, int i);

private:
	Point2f m_center; // координаты центра
	float m_angle; // угол поворота
	float m_width; // ширина корпуса
	float m_height; // высота корпуса
	float m_wheelWidth; // ширина колеса
	float m_wheelDiameter; // диаметр колеса
	float m_speed; // скорость движения
	float m_angularSpeed; // угловая скорость
	Size2i m_area; // область движения
};