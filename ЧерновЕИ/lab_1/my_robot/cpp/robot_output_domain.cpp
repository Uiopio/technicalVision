#pragma once

#include "../h/robot_output.h"


RobotOutput::RobotOutput()
{
}

RobotOutput::RobotOutput(MyRobot robot, Size2i area)
	: m_robot(robot), m_area(area)
{
	if (area.width < robot.getGlobalWidth())
	{
		area.width = robot.getGlobalWidth() + 20;
	}
	if (area.height < robot.getHeight())
	{
		area.height = robot.getHeight() + 20;
	}
	if (area.width % 2 == 0)
	{
		area.width++; //TODO
	}

	m_areaCenter = Point2f((area.width / 2), (area.height / 2));
	m_robot.setCenter(m_areaCenter);
	m_resImg = Mat(area, CV_8U, Scalar(0));
	m_robot.setRobotState(m_resImg);
	m_background = Mat(area, CV_8U, Scalar(0));
}

RobotOutput::~RobotOutput()
{
}

void RobotOutput::setRobot(MyRobot robot) // TODO: clone?
{
	m_robot = robot;
}

int RobotOutput::setArea(Size2i area) // TODO: clone?
{
	if (area.empty())
	{
		return -1;
	}
	m_area = area;
};

int RobotOutput::setArea(Mat image)
{
	if (image.empty())
	{
		return -1;
	}
	m_area.width = image.cols;
	m_area.height = image.rows;
}
int RobotOutput::setResImg(Mat image) // TODO: clone?
{
	if (image.empty())
	{
		return -1;
	}
	m_resImg = image;
}
void RobotOutput::setAreaCenter(Point2f center)
{
	m_areaCenter = center;
}
MyRobot RobotOutput::getRobot()
{
	return m_robot;
}

Size2i RobotOutput::getArea()
{
	return m_area;
}

Mat RobotOutput::getResImg()
{
	return m_resImg;
}

Point2f RobotOutput::getAreaCenter()
{
	return m_areaCenter;
}
