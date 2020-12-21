#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;

class Part_1
{
private:
    cv::Mat m_image;
    cv::Mat m_result;
public:
    Part_1(cv::Mat image);
    ~Part_1();

    uint8_t search_max(cv::Mat src);
    void bin_threshhold(cv::Mat& dst, uint8_t t);
    void search_response(cv::Mat src, cv::Mat& dst);
    void search_response_centr(int i, int j, cv::Mat& dst);
    

    void show();
    void show_result();
};

class Part_2
{
private:
    cv::Mat m_image;
    cv::Mat m_result;
public:
    Part_2(cv::Mat image);
    ~Part_2();

    void calculate_centr(vector <vector<cv::Point>> contours, vector<cv::Point>& centr, bool isOne);
    void draw_aim(cv::Mat src, cv::Mat& dst, vector<cv::Point>& centr);

    void search_response(cv::Mat src, cv::Mat& dst);
    void search_response_centr(int i, int j, cv::Mat& dst);

    void show();
    void show_result();
};

class Part_3
{
private:
    cv::Mat m_image;
    cv::Mat m_result;
    cv::Mat m_red_r;
    cv::Mat m_green_r;
    cv::Mat m_blue_r;
    cv::Point m_centr_of_light;
    vector<cv::Point> m_closest_points;

public:
    Part_3(cv::Mat image);
    ~Part_3();

    enum ColorOfRobot
    {
        RED,
        GREEN,
        BLUE
    };
    
    void find_light(cv::Mat src, cv::Point& dst);
    void robot_filter(cv::Mat src, cv::Mat& dst, ColorOfRobot color);
    uint8_t search_max(cv::Mat src);
    void bin_threshhold(cv::Mat& dst, uint8_t t);
    void calculate_centr(vector <vector<cv::Point>> contours, vector<cv::Point>& centr, bool isOne);
    void draw_aim(cv::Mat src, cv::Mat& dst, vector<cv::Point>& centr);
    cv::Point find_closest(vector<cv::Point>& points, cv::Point  object);
   
    void show();
    void show_result();


};

class Part_4
{
private:
    cv::Mat m_image;
    cv::Mat m_shape;
    cv::Mat m_result;

public:
    Part_4(cv::Mat image, cv::Mat shape);
    ~Part_4();

    void show();
    void show_result();
};