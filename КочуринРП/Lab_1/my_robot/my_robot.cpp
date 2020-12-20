//in the process		
//ready					-	need to check performance
//correctly

#include "my_robot.h"

#define SPEED  10

static const double PI = 3.141592653589793;
static float sinus = 0;
static float cosinus = 0;

static const Scalar white(255, 255, 255);
static const Scalar black(0, 0, 0);

//конструктор
MyRobot::MyRobot() :
    m_width(1),
    m_height(1),
    m_wheelWidth(1),
    m_wheelDiameter(1),
    m_speedAlong(0),
    m_speedAcross(0),
    m_angularSpeed(0)
{
    ;
}
MyRobot::MyRobot(float width, float height,
    float wheelWidth, float wheelDiameter,
    float speedAlong, float speedAcross,
    float angularSpeed) :
    m_width(width),
    m_height(height),
    m_wheelWidth(wheelWidth),
    m_wheelDiameter(wheelDiameter),
    m_speedAlong(speedAlong),
    m_speedAcross(speedAcross),
    m_angularSpeed(angularSpeed)
{
    ;
}
//десструктор
MyRobot::~MyRobot()
{
    ;
}

//”правление движением
    //задание движени€ с клавиатуры
//correctly
void MyRobot::Influence()
{
    Mat image(720, 1280, CV_8UC3, white);
    setArea(image);
    setCenter(image);
    setAngle(0);
    m_Motion(image);
    return;
}
//correctly
void MyRobot::Influence(Mat& image)
{
    setArea(image);
    setCenter(image);
    setAngle(0);
    m_Motion(image);
    return;
}

//задание движени€ по эвольвенте
//correctly
void MyRobot::Evolvent(float stepAngle, float radius, int direction)
{
    Mat image(720, 720, CV_8UC3, white);
    m_Evolvent(stepAngle, radius, direction, image);
    return;
}
//correctly
int MyRobot::Evolvent(float stepAngle, float radius, int direction, Mat& image)
{
    if (image.empty())
    {
        return -1;
    }
    Mat newImage = image.clone();
    m_Evolvent(stepAngle, radius, direction, newImage);
    return 0;
}

//correctly
void MyRobot::m_Evolvent(float stepAngle, float radius, int direction, Mat& image)
{
    m_Zeroing();
    float deltaAngle = PI / 2;
    if (direction < 0)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
        deltaAngle += PI;
    }
    setArea(image);
    Point2f dot(image.cols / 2, image.rows / 2);
    float angle = (float)direction * (-stepAngle) * 3.14 / 180.0;
    int error = 0;
    int thickness = 2;
    while ((waitKey(10) != 27) && (error == 0) &&
        (dot.x > 0.0) && (dot.x < (float)(m_area.width)) && (dot.y > 0.0) && (dot.y < (float)(m_area.height)))
    {
        for (int i = 0; i < thickness; i++)
        {
            for (int j = 0; j < thickness; j++)
            {
                int y = m_area.height - (dot.y + i);
                int x = dot.x + j;
                image.at<Vec3b>(y, x)[0] = 0;
                image.at<Vec3b>(y, x)[1] = 0;
                image.at<Vec3b>(y, x)[2] = 0;
            }
        }
        angle += (float)direction * stepAngle * 3.14 / 180.0;
        dot.x = image.cols / 2 + radius * (cos(angle) + angle * sin(angle));
        dot.y = image.rows / 2 + radius * (sin(angle) - angle * cos(angle));
        setAngle(angle + deltaAngle);
        setCenter(dot);
        error = m_Positioning();
        m_Drawing(image);
    }
    while (waitKey(10) != 27)
    {
        ;
    }
}
//correctly
    //основна€ функци€ управлени€ с клавиатуры
int MyRobot::m_Motion(Mat& image)
{
    while (waitKey(10) != 27)
    {
        int along = 0;
        int across = 0;
        float angular = 0;
        char key = waitKey(30);
        if ((key == 'w') || (key == 'W'))
            along = SPEED;
        if ((key == 's') || (key == 'S'))
            along = -SPEED;
        if ((key == 'd') || (key == 'D'))
            across = SPEED;
        if ((key == 'a') || (key == 'A'))
            across = -SPEED;
        if ((key == 'q') || (key == 'Q'))
            angular = SPEED * PI / 180;
        if ((key == 'e') || (key == 'E'))
            angular = -SPEED * PI / 180;
        if (key == 32)
            m_Zeroing();
        m_Move(along, across);
        m_Rotate(angular);
        m_Positioning();
        m_Drawing(image);
    }
    return 0;
}

//correctly
    //задание продольной и поперечной скорости
int MyRobot::m_Move(int speedAlong, int speedAcross)
{
    setSpeedAlong(speedAlong);
    setSpeedAcross(speedAcross);
    return 0;
}

//correctly
    //задание вращательной скорости
int MyRobot::m_Rotate(float angularSpeed)
{
    setAngularSpeed(/*getAngularSpeed()*/ +angularSpeed);
    return 0;
}

//correctly
    //обнуление всех скоростей
void MyRobot::m_Zeroing()
{
    setAngularSpeed(0);
    setSpeedAcross(0);
    setSpeedAlong(0);
    return;
}

//ѕостроение
//correctly
    //–исование самого робота
int MyRobot::m_Drawing(Mat& imageOrigin)
{
    if (m_area.empty())
    {
        return -1;
    }

    Mat image = imageOrigin.clone();

    int thickness = 1;
    line(image, m_body.UpLeft, m_body.UpRight, black, thickness);
    line(image, m_body.UpRight, m_body.DownRight, black, thickness);
    line(image, m_body.DownRight, m_body.DownLeft, black, thickness);
    line(image, m_body.DownLeft, m_body.UpLeft, black, thickness);

    line(image, m_wheel[0].UpRight, m_wheel[0].UpLeft, black, thickness);
    line(image, m_wheel[0].UpLeft, m_wheel[0].DownLeft, black, thickness);
    line(image, m_wheel[0].DownLeft, m_wheel[0].DownRight, black, thickness);

    line(image, m_wheel[1].UpLeft, m_wheel[1].UpRight, black, thickness);
    line(image, m_wheel[1].UpRight, m_wheel[1].DownRight, black, thickness);
    line(image, m_wheel[1].DownRight, m_wheel[1].DownLeft, black, thickness);

    line(image, m_wheel[2].UpRight, m_wheel[2].UpLeft, black, thickness);
    line(image, m_wheel[2].UpLeft, m_wheel[2].DownLeft, black, thickness);
    line(image, m_wheel[2].DownLeft, m_wheel[2].DownRight, black, thickness);

    line(image, m_wheel[3].UpLeft, m_wheel[3].UpRight, black, thickness);
    line(image, m_wheel[3].UpRight, m_wheel[3].DownRight, black, thickness);
    line(image, m_wheel[3].DownRight, m_wheel[3].DownLeft, black, thickness);

    imshow("Robot", image);

    return 0;
}

//correctly
    //определение координат точек робота
int MyRobot::m_Positioning()
{
    //копирование на случай врезани€ в рамку
    float tempCenterX = m_center.x;
    float tempCenterY = m_center.y;
    float tempAngle = m_angle;

    sinus = sin(m_angle);
    cosinus = cos(m_angle);

    m_center.x = m_center.x + getSpeedAcross() * cosinus - getSpeedAlong() * sinus;
    m_center.y = m_center.y + getSpeedAlong() * cosinus + getSpeedAcross() * sinus;
    m_angle = m_angle + getAngularSpeed();

    struct rectangle newBody;
    newBody.UpLeft = m_Positioning(-m_width / 2, m_height / 2);
    newBody.UpRight = m_Positioning(m_width / 2, m_height / 2);
    newBody.DownLeft = m_Positioning(-m_width / 2, -m_height / 2);
    newBody.DownRight = m_Positioning(m_width / 2, -m_height / 2);
    struct rectangle newWheel[4];   //0 - UpLeft;       1 - UpRight;
                                    //2 - DownLeft;     3- DownRight
    //0 - UpLeft
    newWheel[0].UpLeft = m_Positioning(-m_width / 2 - m_wheelWidth, m_height / 2);
    newWheel[0].UpRight = newBody.UpLeft;
    newWheel[0].DownLeft = m_Positioning(-m_width / 2 - m_wheelWidth, m_height / 2 - m_wheelDiameter);
    newWheel[0].DownRight = m_Positioning(-m_width / 2, m_height / 2 - m_wheelDiameter);
    //1 - UpRight
    newWheel[1].UpLeft = newBody.UpRight;
    newWheel[1].UpRight = m_Positioning(m_width / 2 + m_wheelWidth, m_height / 2);
    newWheel[1].DownLeft = m_Positioning(m_width / 2, m_height / 2 - m_wheelDiameter);
    newWheel[1].DownRight = m_Positioning(m_width / 2 + m_wheelWidth, m_height / 2 - m_wheelDiameter);
    //2 - DownLeft
    newWheel[2].UpLeft = m_Positioning(-m_width / 2 - m_wheelWidth, -m_height / 2 + m_wheelDiameter);
    newWheel[2].UpRight = m_Positioning(-m_width / 2, -m_height / 2 + m_wheelDiameter);
    newWheel[2].DownLeft = m_Positioning(-m_width / 2 - m_wheelWidth, -m_height / 2);
    newWheel[2].DownRight = newBody.DownLeft;
    //3- DownRight
    newWheel[3].UpLeft = m_Positioning(m_width / 2, -m_height / 2 + m_wheelDiameter);
    newWheel[3].UpRight = m_Positioning(m_width / 2 + m_wheelWidth, -m_height / 2 + m_wheelDiameter);
    newWheel[3].DownLeft = newBody.DownRight;
    newWheel[3].DownRight = m_Positioning(m_width / 2 + m_wheelWidth, -m_height / 2);

    //проверка на корректность координат
    if ((newBody.UpLeft.x < 0) || (newBody.UpLeft.x > m_area.width) || (newBody.UpLeft.y < 0) || (newBody.UpLeft.y > m_area.height) ||
        (newBody.UpRight.x < 0) || (newBody.UpRight.x > m_area.width) || (newBody.UpRight.y < 0) || (newBody.UpRight.y > m_area.height) ||
        (newBody.DownLeft.x < 0) || (newBody.DownLeft.x > m_area.width) || (newBody.DownLeft.y < 0) || (newBody.DownLeft.y > m_area.height) ||
        (newBody.DownRight.x < 0) || (newBody.DownRight.x > m_area.width) || (newBody.DownRight.y < 0) || (newBody.DownRight.y > m_area.height))
    {
        //m_Zeroing();
        m_center.x = tempCenterX;
        m_center.y = tempCenterY;
        m_angle = tempAngle;
        return -4;
    }
    for (int i = 0; i < 4; i++)
    {
        if ((newWheel[i].UpLeft.x < 0) || (newWheel[i].UpLeft.x > m_area.width) || (newWheel[i].UpLeft.y < 0) || (newWheel[i].UpLeft.y > m_area.height) ||
            (newWheel[i].UpRight.x < 0) || (newWheel[i].UpRight.x > m_area.width) || (newWheel[i].UpRight.y < 0) || (newWheel[i].UpRight.y > m_area.height) ||
            (newWheel[i].DownLeft.x < 0) || (newWheel[i].DownLeft.x > m_area.width) || (newWheel[i].DownLeft.y < 0) || (newWheel[i].DownLeft.y > m_area.height) ||
            (newWheel[i].DownRight.x < 0) || (newWheel[i].DownRight.x > m_area.width) || (newWheel[i].DownRight.y < 0) || (newWheel[i].DownRight.y > m_area.height))
        {
            //m_Zeroing();
            m_center.x = tempCenterX;
            m_center.y = tempCenterY;
            m_angle = tempAngle;
            return -4;
        }
    }
    m_body = newBody;
    for (int i = 0; i < 4; i++)
    {
        m_wheel[i] = newWheel[i];
    }
    return 0;
}

//correctly
    //непосредственное определение координат каждой точки робота
Point2f MyRobot::m_Positioning(float x, float y)
{
    Point2f point;
    point.x = x * cosinus - y * sinus + m_center.x;
    point.y = -x * sinus - y * cosinus - m_center.y + m_area.height;
    return point;
}

//—етеры
//correctly
void MyRobot::setSpeedAlong(float speedAlong)
{
    m_speedAlong = speedAlong;
}

//correctly
void MyRobot::setSpeedAcross(float speedAcross)
{
    m_speedAcross = speedAcross;
}

//correctly
void MyRobot::setAngularSpeed(float angularSpeed)
{
    m_angularSpeed = angularSpeed;
}

//correctly
void MyRobot::setArea(Size2i area)
{
    m_area = area;
}

//correctly
int MyRobot::setArea(Mat& image)
{
    if (image.empty())
    {
        return -1;
    }

    m_area.width = image.cols;
    m_area.height = image.rows;
    return 0;
}

//correctly
int MyRobot::setCenter(Mat& image)
{
    if (image.empty())
    {
        return -1;
    }
    if ((image.cols < m_area.width) ||
        (image.rows < m_area.height))
    {
        return -2;
    }
    m_center.x = image.cols / 2;
    m_center.y = image.rows / 2;
    return 0;
}

//correctly
int MyRobot::setCenter(Point2f dot)
{
    if ((dot.x > m_area.width) &
        (dot.y > m_area.height))
    {
        return -3;
    }
    m_center.x = dot.x;
    m_center.y = dot.y;
    return 0;
}

//correctly
int MyRobot::setCenter(int x, int y)
{
    if ((x > m_area.width) &
        (y > m_area.height))
    {
        return -3;
    }
    m_center.x = x;
    m_center.y = y;
    return 0;
}

//correctly
void MyRobot::setAngle(float angle)
{
    m_angle = angle;
}

//√етеры
//correctly
float MyRobot::getSpeedAlong()
{
    return m_speedAlong;
}

//correctly
float MyRobot::getSpeedAcross()
{
    return m_speedAcross;
}

//correctly
float MyRobot::getAngularSpeed()
{
    return m_angularSpeed;
}
