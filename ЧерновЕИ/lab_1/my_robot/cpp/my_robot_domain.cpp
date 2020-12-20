#pragma once

#include "../h/my_robot.h"

MyRobot::MyRobot()
{
}

MyRobot::MyRobot(float width, float height, 
				float wheelWidth, float wheelDiameter, 
				float motionStep, float turnStep) :
	m_width(width), m_height(height),
	m_wheelWidth(wheelWidth), m_wheelDiameter(wheelDiameter),
	m_motionStep(motionStep), m_turnStep(turnStep)
{
	m_center = Point2f((wheelWidth + (width / 2)), (height / 2));
	m_angle = 0;
	m_globalWidth = width + (wheelWidth * 2);
	m_heightWithoutWheels = height - (wheelDiameter * 2);
}

MyRobot::~MyRobot()
{
}

Point2f MyRobot::getCenter()
{
	return m_center;
}
float MyRobot::getAngle()
{
	return m_angle;
}
float MyRobot::getWidth()
{
	return m_width;
}
float MyRobot::getHeight()
{
	return m_height;
}
float MyRobot::getWheelWidth()
{
	return m_wheelWidth;
}
float MyRobot::getWheelDiameter()
{
	return m_wheelDiameter;
}
float MyRobot::getGlobalWidth()
{
	return m_globalWidth;
}
float MyRobot::getHeightWithoutWheels()
{
	return m_heightWithoutWheels;
}
float MyRobot::getMotionStep()
{
	return m_motionStep;
}
float MyRobot::getTurnStep()
{
	return m_turnStep;
}

vector<Point2f> MyRobot::getBasePointsArr()
{
	return m_basePointsArr;
}

void MyRobot::setMotionStep(float motionStep)
{
	m_motionStep = motionStep;
}
void MyRobot::setTurnStep(float turnStep)
{
	m_turnStep = turnStep;
}
void MyRobot::setCenter(Point2f center) // TODO: clone?
{
	m_center = center;
}

void MyRobot::setAngle(float angle)
{
	m_angle = angle;
}

void MyRobot::setWidth(float width)
{
	m_width = width;
}

void MyRobot::setHeight(float height)
{
	m_height = height;
}

void MyRobot::setWheelWidth(float wheelWidth)
{
	m_wheelWidth = wheelWidth;
}

void MyRobot::setWheelDiameter(float wheelDiameter)
{
	m_wheelDiameter = wheelDiameter;
}
