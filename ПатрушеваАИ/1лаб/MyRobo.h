#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

class MyRobo
{
 public:
     MyRobo() = default;
     MyRobo(float width, float height, float wheelWidth, float wheelDiameter, float speed, float angularSpeed);
     ~MyRobo() = default;

     //Геттеры
     float getSpeed(float speed);
     float getAngularSpeed(float angularspeed);

     //Сеттеры
     void setSpeed(float speed);
     void setAngularSpeed(float angularspeed);
     void setArea(cv::Size2i area);
     int setArea(cv::Mat image);
     int setCenter(cv::Mat image);
     int setCenter(float x, float y);

     //доп задание
     int setCenterOfBlackSquare(float x, float y);
     

     //Управление движением
     int move(int8_t number);
     int rotate(int8_t number);

     //Drawing
     int checkBorders(cv::Point2f point);
     cv::Point2i calculateGlobalCoordinates(cv::Point2f point, float angle);
     int draw(cv::Mat& outputImage);
     int drawRestrictedZone(cv::Mat& outputImage);


 private:
     cv::Size2i m_area;
     float m_speed;
     float m_angularSpeed;
     cv::Point2f m_center;
     float m_angle;
     float m_width;
     float m_height;
     float m_wheelWidth;
     float m_wheelDiameter;

     cv::Size2i m_restrictedArea;
     cv::Size2i m_barrier;

     //Доп задание
     cv::Point2f m_centerOfBlackSquare;
     int m_sideOfSquare;
   
     cv::Point2f m_start;
     cv::Point2f m_cornerOfBigZone;
     cv::Size2i m_bigZone;
};


    


    




