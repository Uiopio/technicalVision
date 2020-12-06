#pragma once

#include "robot.h"
#include "combat_module.h"

class WarRobot : public Robot
{
public:
	WarRobot(
		const float width = 60,
		const float length = 120,
		const Wheel wheel = { 10, 40 },
		const CombatModule combatModule = CombatModule(),
		const cv::Point2f center = cv::Point2f(0, 0),
		const float angle = M_PI_2, 
		const float speed = SPEED, 
		const float angularSpeed = ANGULAR_SPEED
	);
	~WarRobot() = default;

	CombatModule& combatModule();

	int32_t draw(cv::Mat& image);

	void doSomething(const char key);

	std::vector<cv::Point2f> boundaryPoints();

private:
	CombatModule m_combatModule;
};
