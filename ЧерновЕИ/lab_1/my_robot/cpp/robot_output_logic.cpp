#pragma once

#include "../h/robot_output.h"

int RobotOutput::draw(cv::String winName)
{
	namedWindow(winName, WINDOW_AUTOSIZE);
	imshow(winName, m_resImg);
	
	//return listenPress(winName);

	return goOnSquare(winName);
}

int RobotOutput::listenPress(cv::String winName)
{
	while (true)
	{
		// Clear image
		m_background.copyTo(m_resImg);

		bool pressFlag = false;
		while (pressFlag == false)
		{
			pressFlag = true;
			switch ((char)waitKey(0))
			{
			case (char)'s':
				pressFlag = m_robot.goBack(m_resImg, m_background) ? true : false;
				break;
			case (char)'w':
				pressFlag = m_robot.goForth(m_resImg, m_background) ? true : false;
				break;
			case (char)'d':
				pressFlag = m_robot.turnClockwise(m_resImg) ? true : false;
				break;
			case (char)'a':
				pressFlag = m_robot.turnCounterclockwise(m_resImg) ? true : false;
				break;
			case (char)27:
				return 0;
			default:
				pressFlag = false;
				break;
			}
		}
		imshow(winName, m_resImg);
	}
}

int RobotOutput::goOnSquare(cv::String winName)
{
	for (int i = 0; i < 100; i++)
	{
		m_background.copyTo(m_resImg);
		m_robot.goForth(m_resImg, m_background);
		imshow(winName, m_resImg);
		waitKey(1);
	}

	int l = 100;
	while (true)
	{
		for (float a = 0; a < 6; a += (3.14 / 2))
		{
			m_background.copyTo(m_resImg);
			m_robot.setAngle(a + (3.14 / 2));
			m_robot.setRobotState(m_resImg);
			imshow(winName, m_resImg);
			waitKey(1);

			for (int i = 0; i < l; i++)
			{
				m_background.copyTo(m_resImg);
				m_robot.goForth(m_resImg, m_background);
				imshow(winName, m_resImg);
				char key = waitKey(1);
				if (key == 'w')
				{
					l += 30;
				}
				if (key == 's')
				{
					l -= 30;
				}
			}
		}
	}

	return 0;
}
