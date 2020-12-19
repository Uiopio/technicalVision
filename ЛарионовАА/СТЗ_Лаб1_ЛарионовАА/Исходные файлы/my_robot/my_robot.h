#pragma once

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "windows.h"

using namespace cv;
using namespace std;

class MyRobot
{
public:
    MyRobot();
    MyRobot(const float width, const float heignt, const float trackWidth, float speed, float angularSpeed);
    ~MyRobot();
private:
    float m_width;
    float m_height;
    float m_trackWidth;

    float m_speed;
    float m_angularSpeed;

    Size2i m_area;

    Point2f m_center;
    Point2f cornerDots[8];
    float m_angle;
public:
    void setSpeed(float speed);
    void setAngularSpeed(float angularSpeed);
    void setArea(const Size2i area);
    int setArea(const Mat image);
    int setCenter(Mat image);
    int setCenter(float x, float y);
    void setAngle(float angle);
    
    float getSpeed();
    float getAngularSpeed();
    Point2f getCenter();
    float getAngle();

    int move(int straight, int sideways);
    int rotate(bool isClockDirection);

    int draw(Mat& iImage, Mat& oImage);

    void figure(int a, int b, Mat& iImage, Mat& oImage);
};