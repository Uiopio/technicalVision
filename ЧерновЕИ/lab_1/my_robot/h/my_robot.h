#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


class MyRobot
{
private:
    Point2f m_center = Point2f();
    float m_angle = 0;
    float m_width = 1;
    float m_height = 4;
    float m_wheelWidth = 1;
    float m_wheelDiameter = 1;
    float m_globalWidth = 3;
    float m_heightWithoutWheels = 2;
    float m_motionStep = 10;
    float m_turnStep = 0.1;
    vector<Point2f> m_basePointsArr = {};

public:
    MyRobot();
    MyRobot(float width, float height,
            float wheelWidth, float wheelDiameter,
            float motionStep = 10.0f, float turnStep = 0.1f);
    ~MyRobot();

public:
    bool setRobotState(Mat ioImage);
    bool goForth(Mat ioImage, Mat background);
    bool goBack(Mat ioImage, Mat background);
    bool turnClockwise(Mat ioImage);
    bool turnCounterclockwise(Mat ioImage);

    //Getters
    Point2f getCenter();
    float getAngle();
    float getWidth();
    float getHeight();
    float getWheelWidth();
    float getWheelDiameter();
    float getGlobalWidth();
    float getHeightWithoutWheels();
    float getMotionStep();
    float getTurnStep();
    vector<Point2f> getBasePointsArr();

    // Setters
    void setCenter(Point2f center);
    void setAngle(float angle);
    void setWidth(float width);
    void setHeight(float height);
    void setWheelWidth(float wheelWidth);
    void setWheelDiameter(float wheelDiameter);
    void setMotionStep(float motionStep);
    void setTurnStep(float turnStep);

private:
    bool edgesTest(Mat ioImage);
    void drowTrack(Point2f oldCenterPoint, Mat background);
};