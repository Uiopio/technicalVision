#pragma once
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <math.h>


using namespace cv;
using namespace std;


class MyRobot
{
public:
	MyRobot();
	MyRobot(int8_t width, int8_t height,
		float wheelWidth, float wheelDiameter);
	~MyRobot();

//�������
	void setSpeed(const float speed);
	void setAngularSpeed(const float angularSpeed);

	void setArea(const Size2i area);
	int setArea(Mat image);

	void setBorders(int16_t xBorder, int16_t yBorder);

	int setStartPoint(Mat image);
	int setStartPoint(float x, float y);

	void setPeriod(float period)
	{
		m_synPer = period;
	}

	void setAmplitude(float amplitude)
	{
		m_synAmpl = amplitude;
	}

	void setAngularFrequency(float angularFrequency)
	{
		m_synAngFreq = angularFrequency;
	}

	void setSynStep(float step)
	{
		m_synStep = step;
	}
//�������
	float getSpeed();
	float getAngularSpeed();

//���������� ���������
	void move(char key);
	void moveSyn(char key);

//���������
	int draw(Mat& ioImage);

private:

	float m_angle; //���� �������� ��������� ������� ��������� ������

	Point2i m_center; //���������� ������ ������ 

	Point2i m_pt1; //������� ����� ������� ������ � ������� �����������
	Point2i m_pt2;
	Point2i m_pt3;
	Point2i m_pt4;

	Point2i m_pt1_loc; //������� ����� ������� ������
	Point2i m_pt2_loc;
	Point2i m_pt3_loc;
	Point2i m_pt4_loc;


	Point2i m_pt5; //������� ����� ����� ������ � ������� �����������
	Point2i m_pt6;
	Point2i m_pt7;
	

	Point2i m_pt5_loc; //������� ����� �����
	Point2i m_pt6_loc;
	Point2i m_pt7_loc;
	

	int8_t m_width; //������ �������
	int8_t m_height; //����� �������
	
	int8_t m_halfWidth = m_width / 2;
	int8_t m_halfHeight = m_height / 2;

	int8_t m_wheelWidth;//������ �����
	int8_t m_wheelDiameter; //������� �����

	float m_speed;
	float m_angularSpeed;

	Size2i m_area;

	int16_t  m_topBorder;
	int16_t  m_bottomBorder;
	int16_t  m_rightBorder;
	int16_t  m_leftBorder;

	bool m_isTBcrossed = false;
	bool m_isBBcrossed = false;
	bool m_isRBcrossed = false;
	bool m_isLBcrossed = false;

	float m_synPer;
	float m_synAmpl;
	float m_synAngFreq;
	float m_synStep;
};