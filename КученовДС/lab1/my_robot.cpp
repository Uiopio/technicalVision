#pragma once
#include "my_robot.h"


const double Pi{ 3.14159265 };
char key = 0;

MyRobot::MyRobot()
{
    ;
}

MyRobot::MyRobot(float width, float height, float wheelWidth,
                 float wheelDiameter,
                 float speed, float angularSpeed) :

    m_speed(speed),
    m_angularSpeed(angularSpeed),
    m_width(width),
    m_height(height),
    m_wheelWidth(wheelWidth),
    m_wheelDiameter(wheelDiameter)
{
    ;
}

MyRobot::~MyRobot()
{
    ;
}


//Сеттеры
void MyRobot::setSpeed(const float speed)
{
    m_speed = speed;
}

void MyRobot::setAngularSpeed(const float angularSpeed)
{
    m_angularSpeed = angularSpeed;
}

void MyRobot::setAngle(const float angle)
{
    m_angle = angle;
}

void MyRobot::setArea(const cv::Size2i area)
{
    m_area = area;
    m_area.width = area.width;
    m_area.height = area.height;
    m_rect.x = 0;
    m_rect.y = 0;
    m_rect.width = m_area.width;
    m_rect.height = m_area.height;
}

int MyRobot::setArea(cv::Mat image)
{
    if (image.empty())
    {
        return -1;
    }
    m_area.width = image.cols;
    m_area.height = image.rows;

    return 0;
}

int MyRobot::setCenter(cv::Mat image)
{
    if (image.empty())
    {
        return -1;
    }

    if ((image.cols / 2 > m_area.width) ||
        (image.rows / 2 > m_area.height))
    {
        return -2;
    }

    m_center.x = image.cols / 2;
    m_center.y = image.rows / 2;

    return 0;

}


int MyRobot::setCenter(float x, float y)
{
    if ((x / 2 > m_area.width) ||
        (y / 2 > m_area.height))
    {
        return -1;
    }

    m_center.x = x;
    m_center.y = y;

    return 0;
}

//Геттеры
float MyRobot::getSpeed()
{
    return m_speed;
}

float MyRobot::getAngularSpeed()
{
    return m_angularSpeed;
}

float MyRobot::getAngle()
{
    return m_angle * Pi / 180;
}

int MyRobot::getCenterX()
{
    return m_center.x;
}

int MyRobot::getCenterY()
{
    return m_center.y;
}


//Управление движением
void MyRobot::move(float x, float y)
{
    m_center.x = m_center.x + (x*cos(m_angle*Pi / 180) - y * sin(m_angle*Pi / 180))*m_speed;
    m_center.y = m_center.y + (x*sin(m_angle*Pi / 180) + y * cos(m_angle*Pi / 180))*m_speed;
}
int MyRobot::rotate(float angle)
{
    m_angle = angle;
    //m_angle = m_angle + angle * m_angularSpeed;
    return 0;
}


void MyRobot::borderCheck()
{
    //Проверка границ
    if (m_center.y < 0)
    {
        setCenter(m_center.x, m_center.y + m_area.height);
    }

    if (m_center.x < 0)
    {
        setCenter(m_center.x + m_area.width, m_center.y);
    }

    if (m_center.y > m_area.height)
    {
        setCenter(m_center.x, m_center.y - m_area.height);
    }

    if (m_center.x > m_area.width)
    {
        setCenter(m_center.x - m_area.width, m_center.y);
    }
}

//Отрисовка
int MyRobot::draw(cv::Mat &ioImage)
{
    if (ioImage.empty())
    {
        return -1;
    }
    if ((ioImage.cols != m_area.width) ||
        (ioImage.rows != m_area.height))
    {
        return -2;
    }

    Scalar lineColor(0, 0, 0);

    ////Корпус
    //Point2f corpus[4];
    //
    ////Левая верхняя точка
    //corpus[0].x = m_center.x - (m_width / 2 * cos(m_angle*Pi / 180) + m_height / 2 * -sin(m_angle*Pi / 180));
    //corpus[0].y = m_center.y - (m_width / 2 * sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Правая верхняя точка
    //corpus[1].x = m_center.x + (m_width / 2 * cos(m_angle*Pi / 180) + m_height / 2 * sin(m_angle*Pi / 180));
    //corpus[1].y = m_center.y - (m_width / 2 * -sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Левая нижняя точка
    //corpus[2].x = m_center.x - (m_width / 2 * cos(m_angle*Pi / 180) + m_height / 2 * sin(m_angle*Pi / 180));
    //corpus[2].y = m_center.y + (m_width / 2 * -sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Правая нижняя точка
    //corpus[3].x = m_center.x + (m_width / 2 * cos(m_angle*Pi / 180) + m_height / 2 * -sin(m_angle*Pi / 180));
    //corpus[3].y = m_center.y + (m_width / 2 * sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));

    //line(ioImage, corpus[0], corpus[1], lineColor, 2, 8, 0);
    //line(ioImage, corpus[1], corpus[3], lineColor, 2, 8, 0);
    //line(ioImage, corpus[3], corpus[2], lineColor, 2, 8, 0);
    //line(ioImage, corpus[2], corpus[0], lineColor, 2, 8, 0);

    ////Колеса

    ////Левое верхнее колесо
    //Point2f topLeftWheel[3];

    ////Левая верхняя точка
    //topLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + m_height / 2 * -sin(m_angle*Pi / 180));
    //topLeftWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Левая нижняя точка
    //topLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*Pi / 180));
    //topLeftWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));
    ////Правая нижняя точка
    //topLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*Pi / 180));
    //topLeftWheel[2].y = m_center.y - (m_width / 2 * sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));

    //line(ioImage, corpus[0], topLeftWheel[0], lineColor, 1, 8, 0);
    //line(ioImage, topLeftWheel[0], topLeftWheel[1], lineColor, 1, 8, 0);
    //line(ioImage, topLeftWheel[1], topLeftWheel[2], lineColor, 1, 8, 0);

    ////Правое верхнее колесо
    //Point2f topRightWheel[3];

    ////Правая верхняя точка
    //topRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + m_height / 2 * sin(m_angle*Pi / 180));
    //topRightWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Правая нижняя точка
    //topRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*Pi / 180));
    //topRightWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));
    ////Левая нижняя точка
    //topRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*Pi / 180));
    //topRightWheel[2].y = m_center.y - (m_width / 2 * -sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));

    //line(ioImage, corpus[1], topRightWheel[0], lineColor, 1, 8, 0);
    //line(ioImage, topRightWheel[0], topRightWheel[1], lineColor, 1, 8, 0);
    //line(ioImage, topRightWheel[1], topRightWheel[2], lineColor, 1, 8, 0);

    ////Левое нижнее колесо
    //Point2f lowLeftWheel[3];

    ////Левая нижняя точка
    //lowLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + m_height / 2 * sin(m_angle*Pi / 180));
    //lowLeftWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Левая верхняя точка
    //lowLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*Pi / 180));
    //lowLeftWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));
    ////Правая верхняя точка
    //lowLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * sin(m_angle*Pi / 180));
    //lowLeftWheel[2].y = m_center.y + (m_width / 2 * -sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));

    //line(ioImage, corpus[2], lowLeftWheel[0], lineColor, 1, 8, 0);
    //line(ioImage, lowLeftWheel[0], lowLeftWheel[1], lineColor, 1, 8, 0);
    //line(ioImage, lowLeftWheel[1], lowLeftWheel[2], lineColor, 1, 8, 0);

    ////Правое нижнее колесо
    //Point2f lowRightWheel[3];
    ////Левая верхняя точка
    //lowRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + m_height / 2 * -sin(m_angle*Pi / 180));
    //lowRightWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle*Pi / 180) + m_height / 2 * cos(m_angle*Pi / 180));
    ////Левая нижняя точка
    //lowRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*Pi / 180));
    //lowRightWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));
    ////Правая нижняя точка
    //lowRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle*Pi / 180));
    //lowRightWheel[2].y = m_center.y + (m_width / 2 * sin(m_angle*Pi / 180) + (m_height / 2 - m_wheelDiameter) * cos(m_angle*Pi / 180));

    //line(ioImage, corpus[3], lowRightWheel[0], lineColor, 1, 8, 0);
    //line(ioImage, lowRightWheel[0], lowRightWheel[1], lineColor, 1, 8, 0);
    //line(ioImage, lowRightWheel[1], lowRightWheel[2], lineColor, 1, 8, 0);



       //Корпус
    Point2f corpus[4];

    //Левая верхняя точка
    corpus[0].x = m_center.x - (m_width / 2 * cos(m_angle) + m_height / 2 * -sin(m_angle));
    corpus[0].y = m_center.y - (m_width / 2 * sin(m_angle) + m_height / 2 * cos(m_angle));
    //Правая верхняя точка
    corpus[1].x = m_center.x + (m_width / 2 * cos(m_angle) + m_height / 2 * sin(m_angle));
    corpus[1].y = m_center.y - (m_width / 2 * -sin(m_angle) + m_height / 2 * cos(m_angle));
    //Левая нижняя точка
    corpus[2].x = m_center.x - (m_width / 2 * cos(m_angle) + m_height / 2 * sin(m_angle));
    corpus[2].y = m_center.y + (m_width / 2 * -sin(m_angle) + m_height / 2 * cos(m_angle));
    //Правая нижняя точка
    corpus[3].x = m_center.x + (m_width / 2 * cos(m_angle) + m_height / 2 * -sin(m_angle));
    corpus[3].y = m_center.y + (m_width / 2 * sin(m_angle) + m_height / 2 * cos(m_angle));

    line(ioImage, corpus[0], corpus[1], lineColor, 2, 8, 0);
    line(ioImage, corpus[1], corpus[3], lineColor, 2, 8, 0);
    line(ioImage, corpus[3], corpus[2], lineColor, 2, 8, 0);
    line(ioImage, corpus[2], corpus[0], lineColor, 2, 8, 0);

    //Колеса

    //Левое верхнее колесо
    Point2f topLeftWheel[3];

    //Левая верхняя точка
    topLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle) + m_height / 2 * -sin(m_angle));
    topLeftWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle) + m_height / 2 * cos(m_angle));
    //Левая нижняя точка
    topLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle));
    topLeftWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));
    //Правая нижняя точка
    topLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle));
    topLeftWheel[2].y = m_center.y - (m_width / 2 * sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));

    line(ioImage, corpus[0], topLeftWheel[0], lineColor, 1, 8, 0);
    line(ioImage, topLeftWheel[0], topLeftWheel[1], lineColor, 1, 8, 0);
    line(ioImage, topLeftWheel[1], topLeftWheel[2], lineColor, 1, 8, 0);

    //Правое верхнее колесо
    Point2f topRightWheel[3];

    //Правая верхняя точка
    topRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle) + m_height / 2 * sin(m_angle));
    topRightWheel[0].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle) + m_height / 2 * cos(m_angle));
    //Правая нижняя точка
    topRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * sin(m_angle));
    topRightWheel[1].y = m_center.y - ((m_width / 2 + m_wheelWidth) * -sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));
    //Левая нижняя точка
    topRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * sin(m_angle));
    topRightWheel[2].y = m_center.y - (m_width / 2 * -sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));

    line(ioImage, corpus[1], topRightWheel[0], lineColor, 1, 8, 0);
    line(ioImage, topRightWheel[0], topRightWheel[1], lineColor, 1, 8, 0);
    line(ioImage, topRightWheel[1], topRightWheel[2], lineColor, 1, 8, 0);

    //Левое нижнее колесо
    Point2f lowLeftWheel[3];

    //Левая нижняя точка
    lowLeftWheel[0].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle) + m_height / 2 * sin(m_angle));
    lowLeftWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle) + m_height / 2 * cos(m_angle));
    //Левая верхняя точка
    lowLeftWheel[1].x = m_center.x - ((m_width / 2 + m_wheelWidth) * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * sin(m_angle));
    lowLeftWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * -sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));
    //Правая верхняя точка
    lowLeftWheel[2].x = m_center.x - (m_width / 2 * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * sin(m_angle));
    lowLeftWheel[2].y = m_center.y + (m_width / 2 * -sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));

    line(ioImage, corpus[2], lowLeftWheel[0], lineColor, 1, 8, 0);
    line(ioImage, lowLeftWheel[0], lowLeftWheel[1], lineColor, 1, 8, 0);
    line(ioImage, lowLeftWheel[1], lowLeftWheel[2], lineColor, 1, 8, 0);

    //Правое нижнее колесо
    Point2f lowRightWheel[3];
    //Левая верхняя точка
    lowRightWheel[0].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle) + m_height / 2 * -sin(m_angle));
    lowRightWheel[0].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle) + m_height / 2 * cos(m_angle));
    //Левая нижняя точка
    lowRightWheel[1].x = m_center.x + ((m_width / 2 + m_wheelWidth) * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle));
    lowRightWheel[1].y = m_center.y + ((m_width / 2 + m_wheelWidth) * sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));
    //Правая нижняя точка
    lowRightWheel[2].x = m_center.x + (m_width / 2 * cos(m_angle) + (m_height / 2 - m_wheelDiameter) * -sin(m_angle));
    lowRightWheel[2].y = m_center.y + (m_width / 2 * sin(m_angle) + (m_height / 2 - m_wheelDiameter) * cos(m_angle));

    line(ioImage, corpus[3], lowRightWheel[0], lineColor, 1, 8, 0);
    line(ioImage, lowRightWheel[0], lowRightWheel[1], lineColor, 1, 8, 0);
    line(ioImage, lowRightWheel[1], lowRightWheel[2], lineColor, 1, 8, 0);




    ////Проверка границ
    //if (m_center.y < 0)
    //{
    //    setCenter(m_center.x, m_center.y + m_area.height);
    //}

    //if (m_center.x < 0)
    //{
    //    setCenter(m_center.x + m_area.width, m_center.y);
    //}

    //if (m_center.y > m_area.height)
    //{
    //    setCenter(m_center.x, m_center.y - m_area.height);
    //}

    //if (m_center.x > m_area.width)
    //{
    //    setCenter(m_center.x - m_area.width, m_center.y);
    //}


    return 0;
}

int main()
{
    const int k = 400;
    float x[k], y[k];
    const float A = 0, B = 10;

    for (int i = 0; i < k; i++)
    {
        float angle = 0.1 * i;
        x[i] = (A + B * angle) * cos(angle) + 400;
        y[i] = (A + B * angle) * sin(angle) + 300;
        
    }

    MyRobot robot(50, 80, 10, 10, 0, 0);

    Size size(800, 600);

    robot.setArea(size);
    robot.setCenter(400, 300);
    robot.setAngle(0);
    robot.setSpeed(10);
    robot.setAngularSpeed(1);

    Scalar background(255, 255, 255);
    Mat image(size, CV_8UC3, background);
    Mat land;

    for (int i = 0; i < k-1; i++)
        cv::line(image, Point2f(x[i], y[i]), Point2f(x[i + 1], y[i + 1]), Scalar(255, 0, 0), 2);

    while (waitKey(1) != 27)
    {
        int i = waitKey(50);
        image.copyTo(land);

        for (int i = 0; i < k; i++)
        {
            int key = waitKey(50);

            robot.setCenter(x[i], y[i]);
            //robot.rotate(0.1*i);

            image.copyTo(land);
            robot.borderCheck();
            robot.draw(land);
            imshow("Land", land);

            

        /*    robot.draw(image);

            imshow("Robot", image);*/
        }
     /*   switch (key)
        {
        case 'w':
            robot.move(0, -1);
            break;
        case 'a':
            robot.move(-1, 0);
            break;
        case 's':
            robot.move(0, 1);
            break;
        case 'd':
            robot.move(1, 0);
            break;
        case 'q':
            robot.rotate(-1);
            break;
        case 'e':
            robot.rotate(1);
            break;*/
        //case 'h':
        //    //for (int i = 0; i < 400; i++)
        //    robot.spiralMove(2, 2);
        //    break;
        //default:
        //    break;
        //}
    }
}
