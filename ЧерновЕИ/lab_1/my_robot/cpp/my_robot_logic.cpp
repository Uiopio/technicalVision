#pragma once

#include "../h/my_robot.h"

bool MyRobot::setRobotState(Mat ioImage)
{
	int wc = (m_width / 2) * cos(m_angle);
	int ws = (m_width / 2) * sin(m_angle);
	int hc = (m_height / 2) * cos(m_angle);
	int hs = (m_height / 2) * sin(m_angle);

	int wwc = (m_globalWidth / 2) * cos(m_angle);
	int wws = (m_globalWidth / 2) * sin(m_angle);
	int whc = (m_heightWithoutWheels / 2) * cos(m_angle);
	int whs = (m_heightWithoutWheels / 2) * sin(m_angle);

	Point2f pt0(((-wwc) + m_center.x - (hs)), ((-wws) + m_center.y + (hc)));
	Point2f pt01(((-wwc) + m_center.x - (whs)), ((-wws) + m_center.y + (whc)));
	Point2f pt02(((-wc) + m_center.x - (whs)), ((-ws) + m_center.y + (whc)));

	Point2f pt12(((-wc) + m_center.x - (-whs)), ((-ws) + m_center.y + (-whc)));
	Point2f pt11(((-wwc) + m_center.x - (-whs)), ((-wws) + m_center.y + (-whc)));
	Point2f pt1(((-wwc) + m_center.x - (-hs)), ((-wws) + m_center.y + (-hc)));

	Point2f pt2(((wwc) + m_center.x - (-hs)), ((wws) + m_center.y + (-hc)));
	Point2f pt21(((wwc) + m_center.x - (-whs)), ((wws) + m_center.y + (-whc)));
	Point2f pt22(((wc) + m_center.x - (-whs)), ((ws) + m_center.y + (-whc)));

	Point2f pt32(((wc) + m_center.x - (whs)), ((ws) + m_center.y + (whc)));
	Point2f pt31(((wwc) + m_center.x - (whs)), ((wws) + m_center.y + (whc)));
	Point2f pt3(((wwc) + m_center.x - (hs)), ((wws) + m_center.y + (hc)));

	m_basePointsArr.push_back(pt0);
	m_basePointsArr.push_back(pt1);
	m_basePointsArr.push_back(pt2);
	m_basePointsArr.push_back(pt3);

	if (edgesTest(ioImage) == false)
	{
		return false;
	}

	line(ioImage, pt0, pt01, Scalar(255), 5, LINE_8, 0);
	line(ioImage, pt01, pt02, Scalar(255), 5, LINE_8, 0);
	line(ioImage, pt02, pt12, Scalar(255), 5, LINE_8, 0);

	line(ioImage, pt12, pt11, Scalar(205), 5, LINE_8, 0);
	line(ioImage, pt11, pt1, Scalar(205), 5, LINE_8, 0);
	line(ioImage, pt1, pt2, Scalar(205), 5, LINE_8, 0);

	line(ioImage, pt2, pt21, Scalar(155), 5, LINE_8, 0);
	line(ioImage, pt21, pt22, Scalar(155), 5, LINE_8, 0);
	line(ioImage, pt22, pt32, Scalar(155), 5, LINE_8, 0);

	line(ioImage, pt32, pt31, Scalar(105), 5, LINE_8, 0);
	line(ioImage, pt31, pt3, Scalar(105), 5, LINE_8, 0);
	line(ioImage, pt3, pt0, Scalar(105), 5, LINE_8, 0);
	
	return true;
}

bool MyRobot::goForth(Mat ioImage, Mat background)
{
	float centerX = m_center.x;
	float centerY = m_center.y;
	m_center.x = m_center.x - m_motionStep * cos(m_angle + 3.14 / 2);
	m_center.y = m_center.y - m_motionStep * sin(m_angle + 3.14 / 2);

	if (setRobotState(ioImage) == false)
	{
		m_center.x = centerX;
		m_center.y = centerY;
		return false;
	}
	drowTrack(Point2f(centerX, centerY), background);
	return true;
}

bool MyRobot::goBack(Mat ioImage, Mat background)
{
	float centerX = m_center.x;
	float centerY = m_center.y;
	m_center.x = m_center.x + m_motionStep * cos(m_angle + 3.14 / 2);
	m_center.y = m_center.y + m_motionStep * sin(m_angle + 3.14 / 2);

	if (setRobotState(ioImage) == false)
	{
		m_center.x = centerX;
		m_center.y = centerY;
		return false;
	}
	drowTrack(Point2f(centerX, centerY), background);
	return true;
}

bool MyRobot::turnClockwise(Mat ioImage)
{
	float angle = m_angle;
	m_angle = m_angle + m_turnStep;

	if (setRobotState(ioImage) == false)
	{
		m_angle = angle;
		return false;
	}
	return true;
}

bool MyRobot::turnCounterclockwise(Mat ioImage)
{
	float angle = m_angle;
	m_angle = m_angle - m_turnStep;

	if (setRobotState(ioImage) == false)
	{
		m_angle = angle;
		return false;
	}
	return true;
}

bool MyRobot::edgesTest(Mat ioImage)
{
	for (Point2f p : m_basePointsArr)
	{
		if (
			p.x <= 0 
			|| p.x >= ioImage.cols
			|| p.y <= 0
			|| p.y >= ioImage.rows
			)
		{
			m_basePointsArr.clear();
			return false;
		}
	}
	m_basePointsArr.clear();
	return true;
}

void MyRobot::drowTrack(Point2f oldCenterPoint, Mat background)
{
	line(background, oldCenterPoint, m_center, Scalar(155), 2, LINE_8, 0);
}
