#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>

using namespace cv;

static struct rectangle
{
    //лобовая часть смотрит вверх
    Point2f UpLeft;
    Point2f UpRight;
    Point2f DownLeft;
    Point2f DownRight;
};

class MyRobot
{
public:
    MyRobot();
    MyRobot(float width, float height,
        float wheelWidth, float wheelDeameter,
        float speedAlong = 0, float speedAcross = 0,
        float angularSpeed = 0);
    ~MyRobot();


    //Управление движением
public:
    //задание движения с клавиатуры
    void Influence();
    void Influence(Mat& image);

    //задание движения по эвольвенте
    void Evolvent(float stepAngle, float radius, int direction);
    int Evolvent(float stepAngle, float radius, int direction, Mat& image);

private:
    int m_Motion(Mat& image);
    int m_Move(int speedAlong, int speedAcross);
    int m_Rotate(float angularSpeed);
    void m_Zeroing();
    void m_Evolvent(float stepAngle, float radius, int direction, Mat& image);

    //Построение
public:

private:
    int m_Drawing(Mat& Image);
    int m_Positioning();
    Point2f m_Positioning(float a, float b);

public:
    //Сетеры
    void setSpeedAlong(float speedAlong);
    void setSpeedAcross(float speedAcross);
    void setAngularSpeed(float angularSpeed);
    void setArea(Size2i area);
    int setArea(Mat& image);
    int setCenter(Mat& image);
    int setCenter(Point2f dot);
    int setCenter(int x, int y);
    void setAngle(float angle);

    //Гетеры
    float getSpeedAlong();
    float getSpeedAcross();
    float getAngularSpeed();

private:
    float m_speedAlong;
    float m_speedAcross;
    float m_angularSpeed;
    Point2f m_center;
    float m_angle;
    const float m_width;
    const float m_height;
    const float m_wheelWidth;
    const float m_wheelDiameter;
    Size2i m_area;

    //точки робота
    //лобовая сторона смотрит вверх
    struct rectangle m_body;
    struct rectangle m_wheel[4];    //0 - UpLeft;       1 - UpRight;
                                    //2 - DownLeft;     3 - DownRight

};
