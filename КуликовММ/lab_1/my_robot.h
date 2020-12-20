#pragma once

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

class MyRobot
{
public:
	MyRobot();

	MyRobot(float width, float height,
			float wheelWidth, float wheelDiameter,
			float speed = 0, float angularSpeed = 0);

	~MyRobot();

	//Сеттеры
	void setArea(cv::Size2i area);
	int setArea(cv::Mat image);
	int setCenter(cv::Mat image);
	int setCenter(int x, int y);

	//Геттеры
	float getSpeed();
	float getAngle();
	float getAngularSpeed();

	//Проверка границ
	bool checkBorders(Point2f point, std::vector<cv::Point> vectorTriangleArea, std::vector<cv::Point> vectorRobotArea);

	//Управление движением
	void move(int x, int y);
	int rotate(float angle);

	//Отрисовка
	int draw(cv::Mat &ioImage, char key);

private:
	cv::Point2f m_center;
	cv::Size2i m_area;
	float m_angle = 0;
	float m_width;
	float m_height;
	float m_wheelWidth;
	float m_wheelDiameter;
	float m_speed;
	float m_rotation;
	float m_angularSpeed;
	char m_key;
	cv::Rect m_rect;
};