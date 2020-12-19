#pragma once

#include "my_robot.h"

MyRobot::MyRobot(const float width, const float height, const float trackWidth, float speed, float angularSpeed) :
    m_width(width), 
    m_height(height), 
    m_trackWidth(trackWidth),
    m_speed(speed), 
    m_angularSpeed(angularSpeed)
{
    m_angle = 0;
}

MyRobot::~MyRobot()
{
    ;
}

void MyRobot::setSpeed(float speed)
{
    if (speed < 0)
    {
        m_speed = -speed;
    }
    else
    {
        m_speed = speed;
    }
    
    return;
}

void MyRobot::setAngularSpeed(float angularSpeed)
{
    if (angularSpeed < 0)
    {
        m_angularSpeed = -angularSpeed;
    }
    else
    {
        m_angularSpeed = angularSpeed;
    }
    
    return;
}

void MyRobot::setArea(const Size2i area)
{
    m_area = area;

    return;
}

int MyRobot::setArea(const Mat image)
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

    if (((image.cols / 2) > m_area.width) || ((image.rows / 2) > m_area.height))
    {
        return -2;
    }

    m_center.x = (float)image.cols / 2.0;
    m_center.y = (float)image.rows / 2.0;

    cornerDots[0].x = (m_center.x - (m_width / 2));
    cornerDots[0].y = (m_center.y - (m_height / 2));
    cornerDots[1].x = (m_center.x + (m_width / 2));
    cornerDots[1].y = (m_center.y - (m_height / 2));
    cornerDots[2].x = (m_center.x + (m_width / 2));
    cornerDots[2].y = (m_center.y + (m_height / 2));
    cornerDots[3].x = (m_center.x - (m_width / 2));
    cornerDots[3].y = (m_center.y + (m_height / 2));

    cornerDots[4].x = (m_center.x - (m_width / 2)) - m_trackWidth;
    cornerDots[4].y = (m_center.y - (m_height / 2));
    cornerDots[5].x = (m_center.x + (m_width / 2)) + m_trackWidth;
    cornerDots[5].y = (m_center.y - (m_height / 2));
    cornerDots[6].x = (m_center.x + (m_width / 2)) + m_trackWidth;
    cornerDots[6].y = (m_center.y + (m_height / 2));
    cornerDots[7].x = (m_center.x - (m_width / 2)) - m_trackWidth;
    cornerDots[7].y = (m_center.y + (m_height / 2));

    return 0;
}

int MyRobot::setCenter(float x, float y)
{
    if ((x < 0) || (y < 0))
    {
        return -1;
    }

    if ((x > m_area.width) || (y > m_area.height))
    {
        return -1;
    }

    m_center.x = x;
    m_center.y = y;

    cornerDots[0].x = (m_center.x - (m_width / 2));
    cornerDots[0].y = (m_center.y - (m_height / 2));
    cornerDots[1].x = (m_center.x + (m_width / 2));
    cornerDots[1].y = (m_center.y - (m_height / 2));
    cornerDots[2].x = (m_center.x + (m_width / 2));
    cornerDots[2].y = (m_center.y + (m_height / 2));
    cornerDots[3].x = (m_center.x - (m_width / 2));
    cornerDots[3].y = (m_center.y + (m_height / 2));

    cornerDots[4].x = (m_center.x - (m_width / 2)) - m_trackWidth;
    cornerDots[4].y = (m_center.y - (m_height / 2));
    cornerDots[5].x = (m_center.x + (m_width / 2)) + m_trackWidth;
    cornerDots[5].y = (m_center.y - (m_height / 2));
    cornerDots[6].x = (m_center.x + (m_width / 2)) + m_trackWidth;
    cornerDots[6].y = (m_center.y + (m_height / 2));
    cornerDots[7].x = (m_center.x - (m_width / 2)) - m_trackWidth;
    cornerDots[7].y = (m_center.y + (m_height / 2));

    return 0;
}

void MyRobot::setAngle(float angle)
{
    m_angle = ((angle / (float)CV_2PI)- (int)(angle / (float)CV_2PI)) * (float)CV_2PI;

    Mat rCoordinates = (Mat_<float>(3, 3) << cos(-m_angle), -sin(-m_angle), m_center.x,
                                             sin(-m_angle), cos(-m_angle), m_center.y,
                                             0, 0, 1);

    Point2f defaultCornerDots[8] = { Point2f((m_center.x - (m_width / 2)), (m_center.y - (m_height / 2))),
                                     Point2f((m_center.x + (m_width / 2)), (m_center.y - (m_height / 2))),
                                     Point2f((m_center.x + (m_width / 2)), (m_center.y + (m_height / 2))),
                                     Point2f((m_center.x - (m_width / 2)), (m_center.y + (m_height / 2))),
                                     Point2f(((m_center.x - (m_width / 2)) - m_trackWidth), (m_center.y - (m_height / 2))),
                                     Point2f(((m_center.x + (m_width / 2)) + m_trackWidth), (m_center.y - (m_height / 2))),
                                     Point2f(((m_center.x + (m_width / 2)) + m_trackWidth), (m_center.y + (m_height / 2))),
                                     Point2f(((m_center.x - (m_width / 2)) - m_trackWidth), (m_center.y + (m_height / 2))) };
    
    for (int i = 0; i < 8; i++)
    {
        Mat localDefaultCoordinates = (Mat_<float>(3, 3) << (defaultCornerDots[i].x - m_center.x), 0, 0,
                                                            (defaultCornerDots[i].y - m_center.y), 0, 0,
                                                            1, 0, 0);

        Mat result;
        result = rCoordinates * localDefaultCoordinates;

        cornerDots[i].x = result.at<float>(0, 0);
        cornerDots[i].y = result.at<float>(1, 0);
    }

    return;
}

float MyRobot::getSpeed()
{
    return m_speed;
}

float MyRobot::getAngularSpeed()
{
    return m_angularSpeed;
}

Point2f MyRobot::getCenter()
{
    return m_center;
}

float MyRobot::getAngle()
{
    return m_angle;
}

int MyRobot::move(int straight, int sideways)
{
    Mat rSpeed = (Mat_<float>(3, 3) << cos(m_angle), -sin(m_angle), 0,
                                       sin(m_angle), cos(m_angle), 0,
                                       0, 0, 1);

    float signS = (float)(sideways > 0) - (float)(sideways < 0);
    float signF = (float)(straight > 0) - (float)(straight < 0);

    Mat vecSpeed = (Mat_<float>(3, 3) << signS * m_speed * (float)(sideways != 0), 0, 0,
                                         signF * m_speed * (float)(straight != 0), 0, 0,
                                         1, 0, 0);

    Mat result;
    result = rSpeed * vecSpeed;

    float incrementX = result.at<float>(0, 0);
    float incrementY = (-1) * result.at<float>(1, 0);

    float previousX = incrementX;
    float previousY = incrementY;

    if ((m_center.x + incrementX) < 0)
    {
        incrementX -= (m_center.x + incrementX);
        incrementY = previousY * (incrementX / previousX);
    }
    if ((m_center.x + incrementX) > m_area.width)
    {
        incrementX -= (m_center.x + incrementX) - m_area.width;
        incrementY = previousY * (incrementX / previousX);
    }

    if ((m_center.y + incrementY) < 0)
    {
        incrementY -= (m_center.y + incrementY);
        incrementX = previousX * (incrementY / previousY);
    }
    if ((m_center.y + incrementY) > m_area.height)
    {
        incrementY -= (m_center.y + incrementY) - m_area.height;
        incrementX = previousX * (incrementY / previousY);
    }

    
    m_center.x += incrementX;
    m_center.y += incrementY;
    cornerDots[0].x += incrementX;
    cornerDots[0].y += incrementY;
    cornerDots[1].x += incrementX;
    cornerDots[1].y += incrementY;
    cornerDots[2].x += incrementX;
    cornerDots[2].y += incrementY;
    cornerDots[3].x += incrementX;
    cornerDots[3].y += incrementY;

    cornerDots[4].x += incrementX;
    cornerDots[4].y += incrementY;
    cornerDots[5].x += incrementX;
    cornerDots[5].y += incrementY;
    cornerDots[6].x += incrementX;
    cornerDots[6].y += incrementY;
    cornerDots[7].x += incrementX;
    cornerDots[7].y += incrementY;
    
    return 0;
}

int MyRobot::rotate(bool isClockDirection)
{
    if (isClockDirection == true)
    {
        m_angle -= m_angularSpeed;
    }
    else
    {
        m_angle += m_angularSpeed;
    }
    
    if (m_angle > (float)CV_2PI)
    {
        m_angle -= (float)CV_2PI;
    }
    if (m_angle < (float)(-CV_2PI))
    {
        m_angle += (float)CV_2PI;
    }

    Mat rCoordinates = (Mat_<float>(3, 3) << cos(-m_angle), -sin(-m_angle), m_center.x,
                                             sin(-m_angle), cos(-m_angle), m_center.y,
                                             0, 0, 1);

    Point2f defaultCornerDots[8] = { Point2f((m_center.x - (m_width / 2)), (m_center.y - (m_height / 2))),
                                     Point2f((m_center.x + (m_width / 2)), (m_center.y - (m_height / 2))),
                                     Point2f((m_center.x + (m_width / 2)), (m_center.y + (m_height / 2))),
                                     Point2f((m_center.x - (m_width / 2)), (m_center.y + (m_height / 2))),
                                     Point2f(((m_center.x - (m_width / 2)) - m_trackWidth), (m_center.y - (m_height / 2))),
                                     Point2f(((m_center.x + (m_width / 2)) + m_trackWidth), (m_center.y - (m_height / 2))),
                                     Point2f(((m_center.x + (m_width / 2)) + m_trackWidth), (m_center.y + (m_height / 2))),
                                     Point2f(((m_center.x - (m_width / 2)) - m_trackWidth), (m_center.y + (m_height / 2))) };

    for (int i = 0; i < 8; i++)
    {
        Mat localDefaultCoordinates = (Mat_<float>(3, 3) << (defaultCornerDots[i].x - m_center.x), 0, 0,
                                                            (defaultCornerDots[i].y - m_center.y), 0, 0,
                                                            1, 0, 0);

        Mat result;
        result = rCoordinates * localDefaultCoordinates;

        cornerDots[i].x = result.at<float>(0, 0);
        cornerDots[i].y = result.at<float>(1, 0);
    }

    return 0;
}

int MyRobot::draw(Mat& iImage, Mat& oImage)
{
    if (iImage.empty())
    {
        return -1;
    }

    if ((iImage.cols != m_area.width) || (iImage.rows != m_area.height))
    {
        return -2;
    }

    oImage = iImage.clone();

    line(oImage, cornerDots[0], cornerDots[1], 0, 2, 8, 0);
    line(oImage, cornerDots[1], cornerDots[2], 0, 2, 8, 0);
    line(oImage, cornerDots[2], cornerDots[3], 0, 2, 8, 0);
    line(oImage, cornerDots[3], cornerDots[0], 0, 2, 8, 0);

    line(oImage, cornerDots[4], cornerDots[0], 0, 2, 8, 0);
    line(oImage, cornerDots[5], cornerDots[1], 0, 2, 8, 0);
    line(oImage, cornerDots[6], cornerDots[2], 0, 2, 8, 0);
    line(oImage, cornerDots[7], cornerDots[3], 0, 2, 8, 0);
    line(oImage, cornerDots[4], cornerDots[7], 0, 2, 8, 0);
    line(oImage, cornerDots[5], cornerDots[6], 0, 2, 8, 0);

    Point2f head(((cornerDots[0].x + cornerDots[1].x) / 2), ((cornerDots[0].y + cornerDots[1].y) / 2));
    line(oImage, head, cornerDots[2], 0, 1, 8, 0);
    line(oImage, head, cornerDots[3], 0, 1, 8, 0);

    putText(oImage, "X center:", Point2i(20, 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, to_string(getCenter().x), Point2i(220, 20), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, "Y center:", Point2i(20, 40), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, to_string(getCenter().y), Point2i(220, 40), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, "Angle, radian:", Point2i(20, 60), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, to_string(getAngle()), Point2i(220, 60), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, "Speed:", Point2i(20, 80), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, to_string(getSpeed()), Point2i(220, 80), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, "Angular speed, radian:", Point2i(20, 100), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    putText(oImage, to_string(getAngularSpeed()), Point2i(220, 100), FONT_HERSHEY_COMPLEX, 0.5, Scalar(125));
    
    //cout << "   ON SCREEN DISPLAY   " << endl;
    //cout << "X center:              " << getCenter().x << endl;
    //cout << "Y center:              " << getCenter().y << endl;
    //cout << "Angle, radian:         " << getAngle() << endl;
    //cout << "Speed:                 " << getSpeed() << endl;
    //cout << "Angular speed, radian: " << getAngularSpeed() << endl;
    //cout << endl;

    return 0;
}

void MyRobot::figure(int a, int b, Mat& iImage, Mat& oImage)
{
    float f = CV_2PI / 200;

    float x = 0;
    float y = 0;

    static int i = 0;

    if (i < 200)
    {
        x = 300 + a * cos(-(i + 1) * f);
        y = 300 + b * sin(-(i + 1) * f);
        iImage.at<Vec<uint8_t, 1>>(y, x)[0] = 0;
        setCenter(x, y);
        setAngle(m_angle);
        i++;
    }
    else
    {
        i = 0;
    }

    return;
}
