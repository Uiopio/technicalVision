#include "combat_module.h"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

CombatModule::CombatModule(
	const float width, 
	const float length,
	const cv::Point2f center,
	const float angle, 
	const float angularSpeed,
	const Border border
) :
	m_width(width),
	m_length(length),
	m_center(center),
	m_angle(angle),
	m_angularSpeed(angularSpeed),
	m_border(border)
{
	m_boundaryPoints = boundaryPoints();
}

void CombatModule::setAngularSpeed(const float angularSpeed)
{
	m_angularSpeed = angularSpeed;
}

float CombatModule::angularSpeed() const
{
	return m_angularSpeed;
}

int32_t CombatModule::rotate(Rotation rotation)
{
	auto rotate = [this](Point2f& point, const float angle)
	{
		auto bufferPoint = point;

		point.x = bufferPoint.x * cosf(angle) - bufferPoint.y * sinf(angle);
		point.y = bufferPoint.x * sinf(angle) + bufferPoint.y * cosf(angle);
	};

	float angle = calculateAngularDisplacement(rotation);

	switch (rotation)
	{
		case Rotation::CLOCKWISE:
		{
			m_angle -= angle;
			break;
		}
		case Rotation::COUNTER_CLOCKWISE:
		{
			m_angle += angle;
			break;
		}
		default:
		{
			return -1;
		}
	}

	for (auto& point : m_boundaryPoints)
	{
		rotate(point, (static_cast<int32_t>(rotation) * 2 - 1) * angle);
	}

	if (angle < m_angularSpeed)
	{
		return -2;
	}

	return 0;
}

void CombatModule::rotateBoundaryPoints(const float angle)
{
	auto rotate = [this](Point2f& point, const float angle)
	{
		auto bufferPoint = point;

		point.x = bufferPoint.x * cosf(angle) - bufferPoint.y * sinf(angle);
		point.y = bufferPoint.x * sinf(angle) + bufferPoint.y * cosf(angle);
	};

	m_boundaryPoints = boundaryPoints();

	for (auto& point : m_boundaryPoints)
	{
		rotate(point, angle);
	}
}

vector<cv::Point2f> CombatModule::towerPoints()
{
	auto point = [this](const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = x * cosf(angle()) - y * sinf(angle());
		point.y = x * sinf(angle()) + y * cosf(angle());
		return point;
	};

	vector<Point2f> points =
	{
		point( m_length / 2.0f,  m_width / 4.0f),
		point( 0.0            ,  m_width / 2.0f),
		point(-m_length / 2.0f,  m_width / 4.0f),
		point(-m_length / 2.0f, -m_width / 4.0f),
		point( 0.0            , -m_width / 2.0f),	
		point( m_length / 2.0f, -m_width / 4.0f)
	};

	return points;
}

vector<cv::Point2f> CombatModule::gunPoints()
{
	auto point = [this](const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = (x + length()) * cosf(angle()) - y * sinf(angle());
		point.y = (x + length()) * sinf(angle()) + y * cosf(angle());
		return point;
	};

	vector<Point2f> points =
	{
		point( m_length / 2.0f,  m_width / 12.0f),
		point(-m_length / 2.0f,  m_width / 12.0f),
		point(-m_length / 2.0f, -m_width / 12.0f),
		point( m_length / 2.0f, -m_width / 12.0f)
	};

	return points;
}

void CombatModule::setCenter(const cv::Point2f center)
{
	m_center = center;
}

cv::Point2f CombatModule::center() const
{
	return m_center;
}

void CombatModule::setAngle(const float angle)
{
	m_angle = angle;
}

float CombatModule::angle() const
{
	return m_angle;
}

void CombatModule::setBorder(const Border border)
{
	m_border = border;
}

Border CombatModule::border() const
{
	return m_border;
}

float CombatModule::width() const
{
	return m_width;
}

float CombatModule::length() const
{
	return m_length;
}

float CombatModule::calculateAngularDisplacement(Rotation rotation)
{
	float angle = m_angularSpeed;

	for (auto& point : m_boundaryPoints)
	{
		float radius = hypotf(point.x - m_center.x, point.y - m_center.y);

		auto distance = [this](Quadrant quadrant)
		{
			switch (quadrant)
			{
			case Quadrant::QUADRANT_I:
				return fabs(center().x - border().left);
			case Quadrant::QUADRANT_II:
				return fabs(center().y - border().bottom);
			case Quadrant::QUADRANT_III:
				return fabs(center().x - border().right);
			case Quadrant::QUADRANT_IV:
				return fabs(center().y - border().top);
			default:
				return FLT_MAX;
			}
		};

		auto realAngle = [this, distance, &radius, &angle](Point2f point, Rotation rotation, Quadrant quadrant)
		{
			float realAngle = angle;
			if (distance(quadrant) < radius)
			{
				float alpha = static_cast<int32_t>(quadrant) * M_PI_2;
				float phi = atan2f((point.y - m_center.y) * cosf(alpha) - (point.x - m_center.x) * sinf(alpha),
					               (point.y - m_center.y) * sinf(alpha) + (point.x - m_center.x) * cosf(alpha));
				float dPhi = acosf(distance(quadrant) / radius);
				realAngle = M_PI - dPhi - (static_cast<int32_t>(rotation) * 2 - 1) * phi;
			}
			if (angle > realAngle)
			{
				return realAngle;
			}
			return angle;
		};

		angle = realAngle(point, rotation, Quadrant::QUADRANT_I  );
		angle = realAngle(point, rotation, Quadrant::QUADRANT_II );
		angle = realAngle(point, rotation, Quadrant::QUADRANT_III);
		angle = realAngle(point, rotation, Quadrant::QUADRANT_IV );
	}

	return angle;
}

vector<Point2f> CombatModule::boundaryPoints()
{
	vector<Point2f> points;

	auto tower = towerPoints();
	points.insert(points.end(), tower.begin(), tower.end());
	
	auto gun = gunPoints();
	points.insert(points.end(), gun.begin(), gun.end());

	return points;
}
