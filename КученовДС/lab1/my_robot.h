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
    MyRobot(float width, float height, float wheelWidth,
        float wheelDiameter,
        float speed = 0, float angularSpeed = 0);
    ~MyRobot();

    //�������
    float getSpeed();
    float getAngularSpeed();
    float getAngle();
    int getCenterX();
    int getCenterY();

    //�������
    void setSpeed(float speed);
    void setAngularSpeed(float angularSpeed);
    void setAngle(float angle);
    void setArea(cv::Size2i area);
    int setArea(cv::Mat image);
    int setCenter(cv::Mat image);
    int setCenter(float x, float y);


    //���������� ���������
    void move(float x, float y);
    int rotate(float angle);
    void spiralMove(float x, int i);

    //���������
    int draw(cv::Mat &ioImage);

    //�������� ������
    bool checkBorders(Point2f point, vector<Point> vectorArea);
    void borderCheck();


private:
    float m_speed;
    float m_angularSpeed;
    cv::Point2f m_center;
    cv::Size2i m_area;
    float m_angle;
    float m_width;
    float m_height;
    float m_wheelWidth;
    float m_wheelDiameter;
    float m_rotation;
    cv::Rect m_rect;
};
