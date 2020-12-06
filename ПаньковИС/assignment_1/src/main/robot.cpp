#include "robot.h"
#include "opencv2/imgproc.hpp"

#define ZERO 0.000001

using namespace std;
using namespace cv;

Robot::Robot(
	const float width, 
	const float length, 
	const Wheel wheel, 
	const cv::Point2f center, 
	const float angle, 
	const float speed, const float angularSpeed
) :
	m_width(width),
	m_length(length),
	m_wheel(wheel),
	m_center(center),
	m_angle(angle),
	m_speed(speed),
	m_angularSpeed(speed)
{
	auto white = Scalar(0xFF, 0xFF, 0xFF);
	auto size = Size(1080, 720);
	auto area = Mat(size, CV_8UC3, white);
	
	setArea(area);
	setCenter(area);

	m_boundaryPoints = boundaryPoints();
}

void Robot::setSpeed(const float speed)
{
	m_speed = speed;
}

float Robot::speed() const
{
	return m_speed;
}

void Robot::setAngularSpeed(const float angularSpeed)
{
	m_angularSpeed = angularSpeed;
}

float Robot::angularSpeed() const
{
	return m_angularSpeed;
}

void Robot::setArea(Size2i area)
{
	m_area = area;
}

int32_t Robot::setArea(Mat image)
{
	if (image.empty() == true)
	{
		return -1;
	}
	m_area.width = image.cols;
	m_area.height = image.rows;

	Border border =
	{
		static_cast<float>(area().width) - 1.0f,
		static_cast<float>(area().height) - 1.0f,
		0.0,
		0.0
	};
	setBorder(border);

	return 0;
}

Size2i Robot::area() const
{
	return m_area;
}

int32_t Robot::setCenter(float centerX, float centerY)
{
	m_center.x = centerX;
	m_center.y = centerY;

	return 0;
}

int32_t Robot::setCenter(cv::Mat image)
{
	if (image.empty() == true)
	{
		return -1;
	}

	m_center.x = static_cast<float>(image.cols / 2.0);
	m_center.y = static_cast<float>(image.rows / 2.0);

	return 0;
}

Point2f Robot::center() const
{
	return m_center;
}

void Robot::setBorder(const Border border)
{
	m_border = border;
}

Border Robot::border() const
{
	return m_border;
}

int32_t Robot::move(Direction direction)
{
	float distance = calculateDisplacement(direction);

	switch (direction)
	{
	case Direction::FORWARD:
		m_center.x += distance * cosf(m_angle);
		m_center.y += distance * sinf(m_angle);
		break;
	case Direction::BACK:
		m_center.x -= distance * cosf(m_angle);
		m_center.y -= distance * sinf(m_angle);
		break;
	case Direction::LEFT:
		m_center.x -= distance * sinf(m_angle);
		m_center.y += distance * cosf(m_angle);
		break;
	case Direction::RIGHT:
		m_center.x += distance * sinf(m_angle);
		m_center.y -= distance * cosf(m_angle);
		break;
	default:
		return -1;
	}

	m_boundaryPoints = boundaryPoints();

	if (distance < m_speed)
	{
		return -2;
	}

	return 0;
}

int32_t Robot::rotate(Rotation rotation)
{
	float angle = calculateAngularDisplacement(rotation);

	switch (rotation)
	{
	case Rotation::CLOCKWISE:
		m_angle -= angle;
		break;
	case Rotation::COUNTER_CLOCKWISE:
		m_angle += angle;
		break;
	default:
		return -1;
	}

	m_boundaryPoints = boundaryPoints();

	if (angle < m_angularSpeed)
	{
		return -2;
	}

	return 0;
}

int32_t Robot::go(Direction direction, Rotation rotation)
{
	move(direction);
	rotate(rotation);

	return 0;
}

int32_t Robot::draw(cv::Mat &image)
{
	if (image.empty() == true)
	{
		return -1;
	}

	if (image.cols != m_area.width || image.rows != m_area.height)
	{
		return -2;
	}

	auto point = [this](const Point2f poligonCenter, const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = center().x + (x + poligonCenter.x) * cosf(angle()) - (y + poligonCenter.y) * sinf(angle());
		point.y = center().y + (x + poligonCenter.x) * sinf(angle()) + (y + poligonCenter.y) * cosf(angle());

		point.y = static_cast<float>(m_area.height) - 1.0f - point.y;
		return point;
	};

	auto poligon = [](cv::Mat& image, vector<Point2f>& poligon, const Scalar& color)
	{
		line(image, poligon.front(), poligon.back(), color);
		for (int32_t index = 1; index < poligon.size(); index++)
		{
			line(image, poligon.at(index - 1), poligon.at(index), color);
		}
	};

	auto black = Scalar(0x00, 0x00, 0x00);

	vector<Point2f> hull =
	{
		point(Point2f(),  m_length / 2.0f,  m_width / 2.0f),
		point(Point2f(), -m_length / 2.0f,  m_width / 2.0f),
		point(Point2f(), -m_length / 2.0f, -m_width / 2.0f),
		point(Point2f(),  m_length / 2.0f, -m_width / 2.0f)
	};

	poligon(image, hull, black);

	vector<Point2f> wheelCenter =
	{
		Point2f( (m_length - m_wheel.diameter) / 2.0f,  (m_width / 2.0f + m_wheel.width)),
		Point2f(-(m_length - m_wheel.diameter) / 2.0f,  (m_width / 2.0f + m_wheel.width)),
		Point2f(-(m_length - m_wheel.diameter) / 2.0f, -(m_width / 2.0f + m_wheel.width)),
		Point2f( (m_length - m_wheel.diameter) / 2.0f, -(m_width / 2.0f + m_wheel.width))
	};
		
	for (auto currentWheelCenter : wheelCenter)
	{
		vector<Point2f> currentWheel =
		{
			point(currentWheelCenter, -m_wheel.diameter / 2.0f,  m_wheel.width / 2.0f),
			point(currentWheelCenter, -m_wheel.diameter / 2.0f, -m_wheel.width / 2.0f),
			point(currentWheelCenter,  m_wheel.diameter / 2.0f, -m_wheel.width / 2.0f),
			point(currentWheelCenter,  m_wheel.diameter / 2.0f,  m_wheel.width / 2.0f)
		};

		poligon(image, currentWheel, black);
	}

	return 0;
}

float Robot::angle() const
{
	return m_angle;
}

float Robot::width() const
{
	return m_width;
}

float Robot::length() const
{
	return m_length;
}

Wheel Robot::wheel() const
{
	return m_wheel;
}

float Robot::calculateDisplacement(Direction direction)
{
	auto borderPoint = [this](const float angle)
	{
		auto borderPoint = Point2f();
		if (cosf(angle) >= 0.0 && sinf(angle) >= 0.0)
		{
			borderPoint.x = border().right;
			borderPoint.y = border().top;
		}
		if (cosf(angle) <  0.0 && sinf(angle) >= 0.0)
		{
			borderPoint.x = border().left;
			borderPoint.y = border().top;
		}
		if (cosf(angle) <  0.0 && sinf(angle) <  0.0)
		{
			borderPoint.x = border().left;
			borderPoint.y = border().bottom;
		}
		if (cosf(angle) >= 0.0 && sinf(angle) <  0.0)
		{
			borderPoint.x = border().right;
			borderPoint.y = border().bottom;
		}
		return borderPoint;
	};

	float distance = m_speed;
	float angle = m_angle + static_cast<uint32_t>(direction) * M_PI_2;

	for (auto& point : m_boundaryPoints)
	{
		float realDistance = FLT_MAX;

		if (fabs(cosf(angle)) > ZERO)
		{
			realDistance = (borderPoint(angle).x - point.x) / cosf(angle);
			if (distance > realDistance)
			{
				distance = realDistance;
			}
		}

		if (fabs(sinf(angle)) > ZERO)
		{
			realDistance = (borderPoint(angle).y - point.y) / sinf(angle);
			if (distance > realDistance)
			{
				distance = realDistance;
			}
		}
	}

	return distance;
}

float Robot::calculateAngularDisplacement(Rotation rotation)
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

vector<Point2f> Robot::boundaryPoints()
{
	auto point = [this](const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = m_center.x + x * cosf(m_angle) - y * sinf(m_angle);
		point.y = m_center.y + x * sinf(m_angle) + y * cosf(m_angle);

		return point;
	};

	vector<Point2f> points =
	{
		point( m_length / 2.0f,  (m_width + 3.0f * m_wheel.width) / 2.0f),
		point(-m_length / 2.0f,  (m_width + 3.0f * m_wheel.width) / 2.0f),
		point(-m_length / 2.0f, -(m_width + 3.0f * m_wheel.width) / 2.0f),
		point( m_length / 2.0f, -(m_width + 3.0f * m_wheel.width) / 2.0f)
	};

	return points;
}

void Robot::doSomething(const char key)
{
	switch (key)
	{
	case 'w':
	case 'W':
		move(Direction::FORWARD);
		break;
	case 's':
	case 'S':
		move(Direction::BACK);
		break;
	case 'a':
	case 'A':
		move(Direction::LEFT);
		break;
	case 'd':
	case 'D':
		move(Direction::RIGHT);
		break;
	case 'q':
	case 'Q':
		go(Direction::FORWARD, Rotation::COUNTER_CLOCKWISE);
		break;
	case 'e':
	case 'E':
		go(Direction::FORWARD, Rotation::CLOCKWISE);
		break;
	case 'z':
	case 'Z':
		go(Direction::BACK, Rotation::CLOCKWISE);
		break;
	case 'x':
	case 'X':
		go(Direction::BACK, Rotation::COUNTER_CLOCKWISE);
		break;
	case '.':
	case '>':
		rotate(Rotation::CLOCKWISE);
		break;
	case ',':
	case '<':
		rotate(Rotation::COUNTER_CLOCKWISE);
		break;
	default:
		break;
	}
}
