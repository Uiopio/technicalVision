#pragma once

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;

class MyRobot
{
private:
    Point2f m_center;
    float m_angle;
    float m_width;
    float m_height;
    float m_wheelWidth;
    float m_wheelDiameter;

    float m_speed;
    float m_angularSpeed;

    Size2i m_area;
public:
    //int motion();
    int move();
    int rotate();

    int draw(Mat& ioImage);
    
    void setSpeed(const float speed);
    void setAngularSpeed(const float angularSpeed);
    void setArea(const Size2i area);
    int setArea(Mat image);
    int setCenter(Mat image);
    int setCenter(float x, float y);

    float getSpeed();
    float getAngularSpeed();
public:
    MyRobot();
    MyRobot(float width, float heignt, float wheelWidth, float wheelDiameter, float speed, float angularSpeed);
    ~MyRobot();
};