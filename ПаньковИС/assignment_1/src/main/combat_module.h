#pragma once

#include "robot.h"

class CombatModule
{
public:
	CombatModule(
		const float width = 40, 
		const float length = 60, 
		const cv::Point2f center = cv::Point2f(0, 0), 
		const float angle = 0, 
		const float angularSpeed = SPEED,
		const Border border = { FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX });
	~CombatModule() = default;

	int32_t rotate(Rotation rotation);
	void rotateBoundaryPoints(const float angle);

	std::vector<cv::Point2f> towerPoints();
	std::vector<cv::Point2f> gunPoints();

	void setAngularSpeed(const float speed);
	float angularSpeed() const;

	void setCenter(const cv::Point2f center);
	cv::Point2f center() const;

	void setAngle(const float angle);
	float angle() const;

	void setBorder(const Border border);
	Border border() const;

	float width() const;
	float length() const;

	float calculateAngularDisplacement(Rotation rotation);
	std::vector<cv::Point2f> boundaryPoints();

private:
	cv::Point2f m_center;
	const float m_width;
	const float m_length;
	float m_angle;
	float m_angularSpeed;
	std::vector<cv::Point2f> m_boundaryPoints;
	Border m_border;
};
