#pragma once

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


using namespace cv;
using namespace std;

class MyRobot
{   
    public:

        MyRobot();
        MyRobot(float width, float height, float wheelWidth, float wheelDiameter, float speed, float angularSpeed);
        ~MyRobot();

        // Сеттеры
        void setSpeed(float speed);
        void setAngularSpeed(float angularSpeed);
        void setArea(Size2i area);
        int setArea(Mat image);
        int setCenter(Mat image);
        int setCircleCenter(Mat image);
        int setCenter(float x, float y);

        // Геттеры
        float getSpeed(float speed);
        float getAngularSpeed(float angularSpeed);

        // Управление движением
        int motion(float x, float y);
        int move(int8_t number);
        int rotate(int8_t number);

        // Отрисовка
        int draw(const Mat& inputImage, Mat& outputImage);

    private:
        float m_speed;
        float m_angularSpeed;
        Point2f m_center;
        Point2f m_circleCenter;
        Size2i m_area;
        float m_angle;
        float m_width;
        float m_height;
        float m_wheelWidth;
        float m_wheelDiameter;
};