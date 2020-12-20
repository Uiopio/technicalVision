#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MyRobo.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/types.hpp>

using namespace std;
using namespace cv;


MyRobo::MyRobo(float width, float height, float wheelWidth,
    float wheelDiameter, float speed, float angularSpeed) :
    m_width(width),
    m_height(height),
    m_wheelWidth(wheelWidth),
    m_wheelDiameter(wheelDiameter),
    m_speed(speed),
    m_angularSpeed(angularSpeed)
    {
        m_angle = 0;
    }


//Геттеры
float MyRobo::getSpeed(float speed)
{
    return speed;
}

float MyRobo::getAngularSpeed(float angularSpeed)
{ 
    return angularSpeed;
}

//Сеттеры
void  MyRobo::setSpeed(float speed)
{
    m_speed = speed;
}

void  MyRobo::setAngularSpeed(float angularSpeed)
{
    m_angularSpeed = angularSpeed;
}

void  MyRobo::setArea(cv::Size2i area)
{
    m_area = area;
}

Point2i MyRobo::calculateGlobalCoordinates(Point2f point, float angle)
{
    Mat rotateMat = (Mat_<float>(3, 3) << cos(angle), -sin(angle), m_center.x,
                                          sin(angle),  cos(angle), m_center.y,
                                               0,          0,           1);

    Mat pointOfRobot = (Mat_<float>(3, 3) << point.x, 0, 0,
                                             point.y, 0, 0,
                                                1,    0, 0);
    Mat result33 = rotateMat * pointOfRobot;

    Point2i result12(result33.at<float>(0, 0), result33.at<float>(1, 0)); 

    return result12;
}

int  MyRobo::setArea(cv::Mat image)
{
    if (image.empty())
    {
        return -1;
    }
    
    m_area.width = image.cols;
    m_area.height = image.rows;

    return 0;
 }

int MyRobo::setCenter(cv::Mat image)
{
    if (image.empty())
    {
        return -1;
    }

    if ((image.cols/2 > m_area.width) ||
        (image.rows/2 > m_area.height))
    {
        return -2;
    }
    
    m_center.x = image.cols / 2;
    m_center.y = image.rows / 2;
    
    return 0;
}

int MyRobo::setCenter(float x, float y)
{
    if ((x > m_area.width) ||
        (y > m_area.height))
    {
        return -1;
    }
    m_center.x = x;
    m_center.y = y;

    return 0;
}

//Доп задание
int MyRobo::setCenterOfBlackSquare(float x, float y)
{
    if ((x > m_area.width) ||
        (y > m_area.height))
    {
        return -1;
    }
    m_centerOfBlackSquare.x = x;
    m_centerOfBlackSquare.y = y;

    return 0;
}


int MyRobo::rotate(int8_t number)
{
    switch (number)
    {
    case 1:
    
        m_angle = m_angle - m_angularSpeed;
        break;
    
    case 2:

        m_angle = m_angle + m_angularSpeed;
        break;

    break;
    }

    return 0;
}

//int MyRobo::checkBorders(Point2f point)
//{
//    m_cornerOfBigZone.x = 200;
//    m_cornerOfBigZone.y = 200;
//    m_restrictedArea.width = 70;
//    m_restrictedArea.height = 70;
//
//    //m_sideOfSquare;
//
//    m_restrictedArea.width = m_area.width -100;
//    m_restrictedArea.height = m_area.height - 100;
//
//    if ((point.x > (m_restrictedArea.width)) || (point.y > (m_restrictedArea.height)) || (point.x < 100) || (point.y < 100))
//    {
//        
//        return -1;
//    }
//    else
//    {
//        return 0;
//    }
//};

/*
int MyRobo::checkBorders(cv::Point2f point)
{
   

    //if (m_center.y > (m_area.height - 100))
    if (point.y > (m_area.height - 100))
    {
        return 2; //движение вверх больше невозможно
    }
    if ((point.x > (m_restrictedArea.width)) || (point.y > (m_restrictedArea.height)) || (point.x < 100) || (point.y < 100))
    {
        return -1;
    }
    else
    {
        //std::cout << point.x << '\n';
        //std::cout << point.y << '\n';
        return 0;
    }
};
*/
  

int MyRobo::checkBorders(Point2f point)
{
    cv::Point2i centerOfwheel1((-m_width / 2 - m_wheelDiameter / 2), (-m_height / 4 - m_wheelDiameter / 2));
    cv::Point2i centerOfwheel2((m_width / 2 + m_wheelDiameter / 2), (-m_height / 4 - m_wheelDiameter / 2));
    cv::Point2i centerOfwheel3((-m_width / 2 - m_wheelDiameter / 2), (m_height / 4 + m_wheelDiameter / 2));
    cv::Point2i centerOfwheel4((m_width / 2 + m_wheelDiameter / 2), (m_height / 4 + m_wheelDiameter / 2));

    //applying rotation matrix for wheels
    centerOfwheel1 = calculateGlobalCoordinates(centerOfwheel1, m_angle);
    centerOfwheel2 = calculateGlobalCoordinates(centerOfwheel2, m_angle);
    centerOfwheel3 = calculateGlobalCoordinates(centerOfwheel3, m_angle);
    centerOfwheel4 = calculateGlobalCoordinates(centerOfwheel4, m_angle);

 

//Доп задание
    if (point != m_centerOfBlackSquare)
    {
        if (m_angle!=0) 
        {
           
            if (((point.x - m_width / 2 - m_wheelDiameter) < (m_cornerOfBigZone.x + 10)) || ((point.y - m_height / 2) < (m_cornerOfBigZone.y+10))
                || ((point.x + m_width / 2 + m_wheelDiameter + 10) > (m_cornerOfBigZone.x + m_bigZone.width)) || ((point.y + m_height / 2) >= (m_cornerOfBigZone.y + m_bigZone.height - 10)))
            {
                return -1;
            }

            if ((abs(m_centerOfBlackSquare.x - point.x) < (m_sideOfSquare / 2 + m_width / 2 + m_wheelDiameter)) && (abs(point.y - m_centerOfBlackSquare.y) <= (m_sideOfSquare / 2 + m_height / 2 )))
            {
                return -1;
            }
        }

        else
        {
            if (((point.x - m_width / 2 - m_wheelDiameter) < (m_cornerOfBigZone.x - 10)) || ((point.y - m_height / 2 -2) < (m_cornerOfBigZone.y))
                || ((point.x + m_width / 2 + m_wheelDiameter) > (m_cornerOfBigZone.x + m_bigZone.width)) || ((point.y + m_height / 2 - 10) >= (m_cornerOfBigZone.y + m_bigZone.height)))
            {
                return -1;
            }

            if ((abs(m_centerOfBlackSquare.x - point.x) < (m_sideOfSquare / 2 + m_width / 2 + m_wheelDiameter-5)) && (abs(point.y - m_centerOfBlackSquare.y) <= (m_sideOfSquare / 2 + m_height / 2)))
            {
                return -1;
            }
        }
      
        
    }
    else
    {
        if (((point.x - m_sideOfSquare/2) < (m_cornerOfBigZone.x)) || ((point.y - m_sideOfSquare/2) < (m_cornerOfBigZone.y))
            || ((point.x + m_sideOfSquare/2) > (m_cornerOfBigZone.x + m_bigZone.width)) || ((point.y + m_sideOfSquare/2) > (m_cornerOfBigZone.y + m_bigZone.height)))
        {
            
            return -1;
        }
        
        
            if ((abs(m_center.x - point.x) < (m_sideOfSquare/2 + m_width/2 + m_wheelDiameter-5)) && (abs(point.y - m_center.y) <= (m_sideOfSquare/2 + m_height/2)))
            {
                return -1;
            } 
    }

};

int MyRobo::move(int8_t number)
{ 
    cv::Point2f previousValue1 = m_center;
    cv::Point2f previousValue2 = m_centerOfBlackSquare;
    float previousAngle= m_angle;

    m_sideOfSquare = 110;

    m_cornerOfBigZone.x = 100;
    m_cornerOfBigZone.y = 100;
    m_bigZone.width = 900;
    m_bigZone.height = 600;

    switch (number)
    {
    case 1: //w
        m_center.x = m_center.x + m_speed * sin(m_angle);
        m_center.y = m_center.y - m_speed * cos(m_angle);
        break;

    case 2: //a
        m_center.x = m_center.x - m_speed * cos(m_angle);
        m_center.y = m_center.y - m_speed * sin(m_angle);

        break;

    case 3: //s
        m_center.x = m_center.x - m_speed * sin(m_angle);
        m_center.y = m_center.y + m_speed * cos(m_angle);
        break;

    case 4: //d
        m_center.x = m_center.x + m_speed * cos(m_angle);
        m_center.y = m_center.y + m_speed * sin(m_angle);

        break;

    //Доп задание

    case 5: // стрелка вверх
        m_centerOfBlackSquare.x = m_centerOfBlackSquare.x;
        m_centerOfBlackSquare.y = m_centerOfBlackSquare.y - m_speed;
        break;

    case 6: //стрелка влево
        m_centerOfBlackSquare.x = m_centerOfBlackSquare.x - m_speed;
        m_centerOfBlackSquare.y = m_centerOfBlackSquare.y;

        break;

    case 7: //стрелка вниз
        m_centerOfBlackSquare.x = m_centerOfBlackSquare.x;
        m_centerOfBlackSquare.y = m_centerOfBlackSquare.y + m_speed;
        break;

    case 8: //стрелка вправо
        m_centerOfBlackSquare.x = m_centerOfBlackSquare.x + m_speed;
        m_centerOfBlackSquare.y = m_centerOfBlackSquare.y;

        break;
        
    break;

    }    

    //доп задание

    if (checkBorders(m_center) == (-1))
    {
        m_center = previousValue1;       
    }

    if (checkBorders(m_centerOfBlackSquare) == (-1))
    {
        m_centerOfBlackSquare = previousValue2;
        
    }

    //if (checkBorders(m_center) == (-1))
    //{
    //    std::cout << 1 << '/n';
    //    m_center = previousValue1;
    //}

    return 0;
}


//Drawing
int MyRobo::draw(cv::Mat& outputImage)
{
    cv::Point2i pointTopLeft ((-m_width / 2), (-m_height / 2));  
    cv::Point2i pointTopRight ((m_width / 2), (-m_height / 2));
    cv::Point2i pointBotLeft ((-m_width / 2), ( m_height / 2));
    cv::Point2i pointBotRight ((m_width / 2), ( m_height / 2));
    
    //applying rotation matrix for body's points
    pointTopLeft = calculateGlobalCoordinates(pointTopLeft, m_angle);
    pointTopRight = calculateGlobalCoordinates(pointTopRight, m_angle);
    pointBotLeft = calculateGlobalCoordinates(pointBotLeft, m_angle);
    pointBotRight = calculateGlobalCoordinates(pointBotRight, m_angle);   
       
    cv::Scalar black = (0, 0, 0);
    int8_t thickness = 3;
    int8_t lineType = 8;
    int8_t shift = 0;
    
    //drawing body
    cv::line (outputImage, pointTopLeft, pointTopRight, black, thickness, lineType, shift); //Top line
    cv::line (outputImage, pointTopLeft, pointBotLeft, black, thickness, lineType, shift); //Left line
    cv::line (outputImage, pointTopRight, pointBotRight, black, thickness, lineType, shift); //Right line
    cv::line (outputImage, pointBotLeft, pointBotRight, black, thickness, lineType, shift); //Bottom line

    //drawing black square
    cv::Point2i squareTopLeft((m_centerOfBlackSquare.x -m_sideOfSquare / 2), (m_centerOfBlackSquare.y - m_sideOfSquare / 2));
    cv::Point2i squareTopRight((m_centerOfBlackSquare.x + m_sideOfSquare / 2), (m_centerOfBlackSquare.y - m_sideOfSquare / 2));
    cv::Point2i squareBotLeft((m_centerOfBlackSquare.x - m_sideOfSquare / 2), (m_centerOfBlackSquare.y + m_sideOfSquare/ 2));
    cv::Point2i squareBotRight((m_centerOfBlackSquare.x + m_sideOfSquare / 2), (m_centerOfBlackSquare.y + m_sideOfSquare/ 2));

    cv::line(outputImage, squareTopLeft, squareTopRight, black, thickness, lineType, shift); //Top line
    cv::line(outputImage, squareTopLeft, squareBotLeft, black, thickness, lineType, shift); //Left line
    cv::line(outputImage, squareTopRight,squareBotRight, black, thickness, lineType, shift); //Right line
    cv::line(outputImage, squareBotLeft, squareBotRight, black, thickness, lineType, shift); //Bottom line


    //drawing wheels
    cv::Point2i centerOfwheel1((-m_width/2 - m_wheelDiameter / 2), (-m_height/4 - m_wheelDiameter / 2));
    cv::Point2i centerOfwheel2(( m_width/2 + m_wheelDiameter / 2), (-m_height/4 - m_wheelDiameter / 2));
    cv::Point2i centerOfwheel3((-m_width/2 - m_wheelDiameter / 2), ( m_height/4 + m_wheelDiameter / 2));
    cv::Point2i centerOfwheel4(( m_width/2 + m_wheelDiameter / 2), ( m_height/4 + m_wheelDiameter / 2));  

    //applying rotation matrix for wheels
    centerOfwheel1 = calculateGlobalCoordinates(centerOfwheel1, m_angle);
    centerOfwheel2 = calculateGlobalCoordinates(centerOfwheel2, m_angle);
    centerOfwheel3 = calculateGlobalCoordinates(centerOfwheel3, m_angle);
    centerOfwheel4 = calculateGlobalCoordinates(centerOfwheel4, m_angle);
    
    //drawing wheels
    circle(outputImage, centerOfwheel1, m_wheelDiameter/ 2, black, thickness, lineType);
    circle(outputImage, centerOfwheel2, m_wheelDiameter/ 2, black, thickness, lineType);
    circle(outputImage, centerOfwheel3, m_wheelDiameter/ 2, black, thickness, lineType);
    circle(outputImage, centerOfwheel4, m_wheelDiameter/ 2, black, thickness, lineType); 
    
    return 0;
}


int MyRobo::drawRestrictedZone(cv::Mat& outputImage)
{
    cv::Scalar black = (0, 0, 0);
    int8_t thickness = 3;
    int8_t lineType = 8;
    int8_t shift = 0;

    cv::Rect RectangleToDraw(m_cornerOfBigZone.x, m_cornerOfBigZone.y, m_bigZone.width, m_bigZone.height);
    rectangle(outputImage, RectangleToDraw.tl(), RectangleToDraw.br(), black, 2, 8, 0);

    return 0;
}
