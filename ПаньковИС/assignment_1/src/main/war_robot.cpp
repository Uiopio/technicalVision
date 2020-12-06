#include "war_robot.h"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

WarRobot::WarRobot(
	const float width, 
	const float length, 
	const Wheel wheel,
	const CombatModule combatModule,
	const cv::Point2f center,
	const float angle,
	const float speed, 
	const float angularSpeed
) : 
	Robot(width, length, wheel, center, angle, speed, angularSpeed),
	m_combatModule(combatModule)
{
	
}

CombatModule& WarRobot::combatModule(void)
{
	return m_combatModule;
}

int32_t WarRobot::draw(cv::Mat& image)
{
	if (image.empty() == true)
	{
		return -1;
	}

	if (image.cols != area().width || image.rows != area().height)
	{
		return -2;
	}

	auto point = [this](const Point2f poligonCenter, const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = center().x + (x + poligonCenter.x) * cosf(angle()) - (y + poligonCenter.y) * sinf(angle());
		point.y = center().y + (x + poligonCenter.x) * sinf(angle()) + (y + poligonCenter.y) * cosf(angle());

		point.y = static_cast<float>(area().height) - 1.0f - point.y;
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
		point(Point2f(),  length() / 2.0f,  width() / 2.0f),
		point(Point2f(), -length() / 2.0f,  width() / 2.0f),
		point(Point2f(), -length() / 2.0f, -width() / 2.0f),
		point(Point2f(),  length() / 2.0f, -width() / 2.0f)
	};

	poligon(image, hull, black);

	vector<Point2f> wheelCenter =
	{
		Point2f( (length() - wheel().diameter) / 2.0f,  (width() / 2.0f + wheel().width)),
		Point2f(-(length() - wheel().diameter) / 2.0f,  (width() / 2.0f + wheel().width)),
		Point2f(-(length() - wheel().diameter) / 2.0f, -(width() / 2.0f + wheel().width)),
		Point2f( (length() - wheel().diameter) / 2.0f, -(width() / 2.0f + wheel().width))
	};

	for (auto currentWheelCenter : wheelCenter)
	{
		vector<Point2f> currentWheel =
		{
			point(currentWheelCenter, -wheel().diameter / 2.0f,  wheel().width / 2.0f),
			point(currentWheelCenter, -wheel().diameter / 2.0f, -wheel().width / 2.0f),
			point(currentWheelCenter,  wheel().diameter / 2.0f, -wheel().width / 2.0f),
			point(currentWheelCenter,  wheel().diameter / 2.0f,  wheel().width / 2.0f)
		};

		poligon(image, currentWheel, black);
	}

	auto towerPoints = m_combatModule.towerPoints();

	vector<Point2f> tower;
	for (auto currentPoint : towerPoints)
	{
		tower.push_back(point(combatModule().center(), currentPoint.x, currentPoint.y));
	}

	poligon(image, tower, black);

	auto gunPoints = m_combatModule.gunPoints();

	vector<Point2f> gun;
	for (auto currentPoint : gunPoints)
	{
		gun.push_back(point(combatModule().center(), currentPoint.x, currentPoint.y));
	}

	poligon(image, gun, black);

	return 0;
}

vector<Point2f> WarRobot::boundaryPoints()
{
	auto point = [this](const float x, const float y)
	{
		auto point = cv::Point2f();
		point.x = center().x + x * cosf(angle()) - y * sinf(angle());
		point.y = center().y + x * sinf(angle()) + y * cosf(angle());

		return point;
	};

	vector<Point2f> points =
	{
		point( length() / 2.0f,  (width() + 3.0f * wheel().width) / 2.0f),
		point(-length() / 2.0f,  (width() + 3.0f * wheel().width) / 2.0f),
		point(-length() / 2.0f, -(width() + 3.0f * wheel().width) / 2.0f),
		point( length() / 2.0f, -(width() + 3.0f * wheel().width) / 2.0f),
	};

	auto gunPoints = m_combatModule.gunPoints();
	for (auto& currentPoint : gunPoints)
	{
		currentPoint = point(combatModule().center().x + currentPoint.x, 
			                 combatModule().center().y + currentPoint.y);
	}
	points.insert(points.end(), gunPoints.begin(), gunPoints.end());

	auto towerPoint = point(combatModule().center().x, combatModule().center().y);
	Border border =
	{
		-(towerPoint.y - this->border().top   ),
		 (towerPoint.x - this->border().left  ),
		-(towerPoint.y - this->border().bottom),
		 (towerPoint.x - this->border().right )
	};
	m_combatModule.setBorder(border);
	m_combatModule.rotateBoundaryPoints(angle() - M_PI_2);

	return points;
}

void WarRobot::doSomething(const char key)
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
	case ']':
	case '}':
	{
		combatModule().rotate(Rotation::CLOCKWISE);
		break;
	}
	case '[':
	case '{':
		combatModule().rotate(Rotation::COUNTER_CLOCKWISE);
		break;
	default:
		break;
	}
}
