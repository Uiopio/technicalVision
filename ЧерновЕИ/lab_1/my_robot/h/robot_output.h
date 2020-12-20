#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "my_robot.h"

using namespace cv;

class RobotOutput
{
private:
    MyRobot m_robot = MyRobot();
    Size2i m_area = Size2i();
    Point2f m_areaCenter = Point2f();
    Mat m_resImg = Mat();
    Mat m_background = Mat(); //TODO g&s

public:
    RobotOutput();
    RobotOutput(MyRobot robot, Size2i area);
    ~RobotOutput();

public:
    int draw(cv::String winName = "win");

    // Setters
    void setRobot(MyRobot robot);
    int setArea(Size2i area);
    int setArea(Mat area);
    int setResImg(Mat image);
    void setAreaCenter(Point2f center);

    // Getters
    MyRobot getRobot();
    Size2i getArea();
    Mat getResImg();
    Point2f getAreaCenter();

private:
    int listenPress(cv::String winName);
    int goOnSquare(cv::String winName);
};