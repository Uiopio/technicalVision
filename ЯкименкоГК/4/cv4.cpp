//=============================================================
//-------------------ADD_TO_MAIN-------------------------------
//-------------------------------------------------------------
/*
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "../OpenCV_test/cv4.h"
using namespace cv;
using namespace std;


int main()
{
    Mat image_1 = imread("C:/Users/nne_l/source/repos/OpenCV_test/images/4lab/babah/ig_0.jpg");
    Mat image_2 = imread("C:/Users/nne_l/source/repos/OpenCV_test/images/4lab/term/1.jpg");
    Mat image_3 = imread("C:/Users/nne_l/source/repos/OpenCV_test/images/4lab/robots/1.jpg");
    Mat image_4 = imread("C:/Users/nne_l/source/repos/OpenCV_test/images/4lab/kluchi/1.jpg");
    Mat shape_4 = imread("C:/Users/nne_l/source/repos/OpenCV_test/images/4lab/kluchi/2.jpg");


   Part_1 part_1(image_1);
   part_1.show();
   part_1.show_result();

    Part_2 part_2(image_2);
    part_2.show();
    part_2.show_result();

    Part_3 part_3(image_3);
    part_3.show();
    part_3.show_result();

    Part_4 part_4(image_4, shape_4);
    part_4.show();
    part_4.show_result();

}
*/
//-------------------------------------------------------------
//=============================================================
//=============================================================

#include "../OpenCV_test/cv4.h"
#include "iostream"
#include "math.h"
#include "time.h"

using namespace std;

Part_1::Part_1(cv::Mat image)
{
    m_image = image.clone();
    
    cvtColor(m_image, m_result, cv::COLOR_RGB2GRAY);
    

    bin_threshhold(m_result, search_max(m_result));

    uint8_t kernelData[9] = { 255, 255, 255,
                              255, 255, 255,
                              255, 255, 255 };

    cv::Mat kernel(3, 3, CV_8UC1, kernelData);
    cv::erode(m_result, m_result, kernel);

    search_response(m_image, m_result);
}

Part_1::~Part_1()
{
    ;
}

uint8_t Part_1::search_max(cv::Mat src)
{
    uint8_t max = 0;

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uint8_t>(i, j) > max) max = src.at<uint8_t>(i, j);
        }
    }

    return max;
}

void Part_1::bin_threshhold(cv::Mat& dst, uint8_t t)
{
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) >= t*0.9) dst.at<uint8_t>(i, j) = 255;
            else dst.at<uint8_t>(i, j) = 0;
        }
    }
}

void Part_1::search_response(cv::Mat src, cv::Mat& dst)
{
    cv::Point2i target[10];
    uint8_t number_of_t = 0;

    //Находим белые области и их центры масс
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) == 255) search_response_centr(i, j, dst);
        }
    }

    //Сохраняем координаты центров масс
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) == 254)
            {
                target[number_of_t].x = j;
                target[number_of_t].y = i;
                number_of_t++;
            }
        }
    }
    
    //Проверяем нет ли нескольких центров масс, принадлежащих одному объекту
    for (int i = 0; i < number_of_t; i++)
    {
        for (int j = 0; j < number_of_t; j++)
        {
            if (i == j) j++;
            if (j == number_of_t) break;

            if (sqrt(pow((target[i].x - target[j].x), 2) + pow((target[i].y - target[j].y), 2)) < dst.rows * 0.15)
            {
                target[i].x = (target[i].x + target[j].x) / 2;
                target[i].y = (target[i].y + target[j].y) / 2;

                for (int n = j; n < number_of_t; n++)
                {
                    if (j + 1 == number_of_t) break;
                    target[j] = target[j + 1];
                }
                number_of_t--;
                i = 0;
                j = 0;
            }
        }
    }

    //Копируем оригинал изображения
    cvtColor(src, dst, cv::COLOR_RGB2GRAY);

    //Отмечаем на оригинале цели
    for (int i = 0; i < number_of_t; i++)
    {
        /*
            a[0]
        a[1]    a[2]
            a[3]  
        */
        cv::Point2i aim[4];

        aim[0].x = target[i].x;
        aim[0].y = target[i].y + 10;
        aim[3].x = target[i].x;
        aim[3].y = target[i].y - 10;
        aim[1].x = target[i].x - 10;
        aim[1].y = target[i].y;
        aim[2].x = target[i].x + 10;
        aim[2].y = target[i].y;

        cv::line(dst, aim[0], aim[3], 0, 2, 8, 0);
        cv::line(dst, aim[1], aim[2], 0, 2, 8, 0);
    }

}

void Part_1::search_response_centr(int i, int j, cv::Mat& dst)
{
   static int tmp_i = 0; //Сумма i
   static int tmp_j = 0; //Сумма j
   static int n = 0; //Количество белых точек
   static int itr = 0; //Вложенность функции

   dst.at<uint8_t>(i, j) = 0;
   tmp_i += i;
   tmp_j += j;
   n++;
   itr++; 

   
   if (dst.at<uint8_t>(i - 1, j) == 255) search_response_centr(i - 1, j, dst);
   if (dst.at<uint8_t>(i - 1, j - 1) == 255) search_response_centr(i - 1, j - 1, dst);
   if (dst.at<uint8_t>(i - 1, j + 1) == 255) search_response_centr(i - 1, j + 1, dst);
   if (dst.at<uint8_t>(i + 1, j) == 255) search_response_centr(i + 1, j, dst);
   if (dst.at<uint8_t>(i + 1, j - 1) == 255) search_response_centr(i + 1, i - 1, dst);
   if (dst.at<uint8_t>(i + 1, j + 1) == 255) search_response_centr(i + 1, j + 1, dst);
   if (dst.at<uint8_t>(i, j - 1) == 255) search_response_centr(i, j - 1, dst);
   if (dst.at<uint8_t>(i, j + 1) == 255) search_response_centr(i, j + 1, dst);

   itr--;

   if (itr == 1)
   {
       int result_i = tmp_i / n;
       int result_j = tmp_j / n;
       dst.at<uint8_t>(result_i, result_j) = 254;

       tmp_i = 0;
       tmp_j = 0;
       n = 0;

       return;
   }

   return;
}

void Part_1::show() 
{
    cv::imshow("image", m_image);
    while (cv::waitKey(1) != 'w');
}

void Part_1::show_result()
{
    cv::imshow("result", m_result);
    while (cv::waitKey(1) != 'w');
}

Part_2::Part_2(cv::Mat image)
{
    m_image = image.clone();

    cvtColor(m_image, m_result, cv::COLOR_BGR2HSV);

    cv::Mat red_mask_1, red_mask_2;
    inRange(m_result, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), red_mask_1);
    inRange(m_result, cv::Scalar(170, 50, 50), cv::Scalar(179, 255, 255), red_mask_2);
    
    m_result = red_mask_1.clone();
    m_result |= red_mask_2;

    uint8_t kernelData[9] = { 255, 255, 255,
                              255, 255, 255,
                              255, 255, 255 };

    cv::Mat kernel(3, 3, CV_8UC1, kernelData);
    cv::erode(m_result, m_result, kernel);
    cv::dilate(m_result, m_result, kernel);

    // search_response(m_image, m_result);

    Canny(m_result, m_result, 0, 255, 3, false);

    vector <vector<cv::Point>> contours;
    cv::findContours(m_result, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    vector<cv::Point> centr;
    calculate_centr(contours, centr, false);

    draw_aim(m_image, m_result, centr);
 
}

void Part_2::search_response(cv::Mat src, cv::Mat& dst)
{
    cv::Point2i target[10];
    uint8_t number_of_t = 0;
    
    imshow("a_1", dst);
    while (cv::waitKey(1) != 'w');

    //Находим белые области и их центры масс
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) == 255) search_response_centr(i, j, dst);
        }
    }

    imshow("a", dst);
    while (cv::waitKey(1) != 'w');

    //Сохраняем координаты центров масс
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) == 254)
            {
                target[number_of_t].x = j;
                target[number_of_t].y = i;
                number_of_t++;
            }
        }
    }

    ////Проверяем нет ли нескольких центров масс, принадлежащих одному объекту
    //for (int i = 0; i < number_of_t; i++)
    //{
    //    for (int j = 0; j < number_of_t; j++)
    //    {
    //        if (i == j) j++;
    //        if (j == number_of_t) break;
    //
    //        if (sqrt(pow((target[i].x - target[j].x), 2) + pow((target[i].y - target[j].y), 2)) < dst.rows * 0.15)
    //        {
    //            target[i].x = (target[i].x + target[j].x) / 2;
    //            target[i].y = (target[i].y + target[j].y) / 2;
    //
    //            for (int n = j; n < number_of_t; n++)
    //            {
    //                if (j + 1 == number_of_t) break;
    //                target[j] = target[j + 1];
    //            }
    //            number_of_t--;
    //            i = 0;
    //            j = 0;
    //        }
    //    }
    //}
    
    m_result = src.clone();

    //Отмечаем на оригинале цели
    for (int i = 0; i < number_of_t; i++)
    {
        /*
            a[0]
        a[1]    a[2]
            a[3]
        */
        cv::Point2i aim[4];

        aim[0].x = target[i].x;
        aim[0].y = target[i].y + 10;
        aim[3].x = target[i].x;
        aim[3].y = target[i].y - 10;
        aim[1].x = target[i].x - 10;
        aim[1].y = target[i].y;
        aim[2].x = target[i].x + 10;
        aim[2].y = target[i].y;

        cv::line(dst, aim[0], aim[3], (0,0,0), 2, 8, 0);
        cv::line(dst, aim[1], aim[2], (0,0,0), 2, 8, 0);
    }
}

void Part_2::search_response_centr(int i, int j, cv::Mat& dst)
{
    static int tmp_i = 0; //Сумма i
    static int tmp_j = 0; //Сумма j
    static int n = 0; //Количество белых точек
    static int itr = 0; //Вложенность функции

    dst.at<uint8_t>(i, j) = 0;
    tmp_i += i;
    tmp_j += j;
    n++;
    itr++;


    if (dst.at<uint8_t>(i - 1, j) == 255) search_response_centr(i - 1, j, dst);
    if (dst.at<uint8_t>(i - 1, j - 1) == 255) search_response_centr(i - 1, j - 1, dst);
    if (dst.at<uint8_t>(i - 1, j + 1) == 255) search_response_centr(i - 1, j + 1, dst);
    if (dst.at<uint8_t>(i + 1, j) == 255) search_response_centr(i + 1, j, dst);
    if (dst.at<uint8_t>(i + 1, j - 1) == 255) search_response_centr(i + 1, i - 1, dst);
    if (dst.at<uint8_t>(i + 1, j + 1) == 255) search_response_centr(i + 1, j + 1, dst);
    if (dst.at<uint8_t>(i, j - 1) == 255) search_response_centr(i, j - 1, dst);
    if (dst.at<uint8_t>(i, j + 1) == 255) search_response_centr(i, j + 1, dst);

    itr--;

    if (itr == 0)
    {
        int result_i = tmp_i / n;
        int result_j = tmp_j / n;
        dst.at<uint8_t>(result_i, result_j) = 254;

        tmp_i = 0;
        tmp_j = 0;
        n = 0;
        return;
    }

    return;
}

Part_2::~Part_2()
{
    ;
}

void Part_2::calculate_centr(vector <vector<cv::Point>> contours, vector<cv::Point>& centr, bool isOne)
{
    if (isOne)
    {
        vector<cv::Point> tmp;

        for (int i = 0; i < contours.size(); i++)
        {
            cv::Moments moment = moments(contours[i]);

            float m00 = moment.m00;
            float m10 = moment.m10;
            float m01 = moment.m01;

            cv::Point result;
            result.x = m10 / m00;
            result.y = m01 / m00;

            tmp.push_back(result);
        }

        cv::Point n_res;
        
        for (int i = 0; i < tmp.size(); i++) n_res.x += tmp[i].x;
        for (int i = 0; i < tmp.size(); i++) n_res.y += tmp[i].y;
        n_res.x = n_res.x / tmp.size();
        n_res.y = n_res.y / tmp.size();

        centr.push_back(n_res);
    }
    else
    {
        for (int i = 0; i < contours.size(); i++)
        {
            cv::Moments moment = moments(contours[i]);

            float m00 = moment.m00;
            float m10 = moment.m10;
            float m01 = moment.m01;

            cv::Point result;
            result.x = m10 / m00;
            result.y = m01 / m00;

            centr.push_back(result);
        }
    }
}

void Part_2::draw_aim(cv::Mat src, cv::Mat& dst, vector<cv::Point>& centr)
{
    dst = src.clone();

    for (int i = 0; i < centr.size(); i++)
    {
        /*
            a[0]
        a[1]    a[2]
            a[3]
        */
        cv::Point2i aim[4];

        aim[0].x = centr[i].x;
        aim[0].y = centr[i].y + 10;
        aim[3].x = centr[i].x;
        aim[3].y = centr[i].y - 10;
        aim[1].x = centr[i].x - 10;
        aim[1].y = centr[i].y;
        aim[2].x = centr[i].x + 10;
        aim[2].y = centr[i].y;

        cv::line(dst, aim[0], aim[3], (0,0,0), 2, 8, 0);
        cv::line(dst, aim[1], aim[2], (0,0,0), 2, 8, 0);
    }
}

void Part_2::show()
{
    cv::imshow("image", m_image);
    while (cv::waitKey(1) != 'w');
}

void Part_2::show_result()
{
    cv::imshow("result", m_result);
    while (cv::waitKey(1) != 'w');
}

Part_3::Part_3(cv::Mat image)
{
    m_image = image.clone();
    m_result = m_image.clone();
    
    find_light(m_image, m_centr_of_light);
    m_closest_points.push_back(m_centr_of_light);
  
    robot_filter(m_image, m_red_r, RED);
    robot_filter(m_image, m_green_r, GREEN);
    robot_filter(m_image, m_blue_r, BLUE);

    vector <vector<cv::Point>> r_contours;
    vector <vector<cv::Point>> g_contours;
    vector <vector<cv::Point>> b_contours;
    cv::findContours(m_red_r, r_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::findContours(m_green_r, g_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::findContours(m_blue_r, b_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    polylines(m_result, r_contours, true, cv::Scalar(0, 0, 255), 3);
    polylines(m_result, g_contours, true, cv::Scalar(0, 255, 0), 3);
    polylines(m_result, b_contours, true, cv::Scalar(255, 0, 0), 3);

    vector <cv::Point> centres_of_r;
    vector <cv::Point> centres_of_g;
    vector <cv::Point> centres_of_b;

    calculate_centr(r_contours, centres_of_r, false);
    calculate_centr(g_contours, centres_of_g, false);
    calculate_centr(b_contours, centres_of_b, false);

    m_closest_points.push_back(find_closest(centres_of_r, m_centr_of_light));
    m_closest_points.push_back(find_closest(centres_of_g, m_centr_of_light));
    m_closest_points.push_back(find_closest(centres_of_b, m_centr_of_light));

    draw_aim(m_result, m_result, m_closest_points);
    cv::line(m_result, m_closest_points[0], m_closest_points[1], (0, 0, 0), 1, 8, 0);
    cv::line(m_result, m_closest_points[0], m_closest_points[2], (0, 0, 0), 1, 8, 0);
    cv::line(m_result, m_closest_points[0], m_closest_points[3], (0, 0, 0), 1, 8, 0);
  
}

Part_3::~Part_3()
{

}

void Part_3::find_light(cv::Mat src, cv::Point& dst) 
{
    cvtColor(src, src, cv::COLOR_RGB2GRAY);
    bin_threshhold(src, search_max(src));

    uint8_t kernelData[9] = { 255, 255, 255,
                              255, 255, 255,
                              255, 255, 255 };

    cv::Mat kernel(3, 3, CV_8UC1, kernelData);
    cv::erode(src, src, kernel);
    cv::dilate(src, src, kernel);

    Canny(src, src, 0, 255, 3, true);

    vector <vector<cv::Point>> contours;
    cv::findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    vector<cv::Point> centr;
    calculate_centr(contours, centr, false);

    dst = centr[0];
}

void Part_3::robot_filter(cv::Mat src, cv::Mat& dst, ColorOfRobot color)
{
    cv::Mat src_copy = src.clone();
    cvtColor(src, src_copy, cv::COLOR_BGR2HSV);

    switch (color)
    {
        case RED:
        {
            cv::Mat red_mask_1, red_mask_2;
            inRange(src_copy, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), red_mask_1);
            inRange(src_copy, cv::Scalar(170, 50, 50), cv::Scalar(179, 255, 255), red_mask_2);
            red_mask_1 |= red_mask_2;
            src_copy = red_mask_1.clone();
            cv::Mat kernel;
            kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
            cv::erode(src_copy, src_copy, kernel);
            cv::dilate(src_copy, src_copy, kernel);
            break;
        }
        case GREEN:
        {
            cv::Mat green_mask;
            inRange(src_copy, cv::Scalar(65, 50, 130), cv::Scalar(80, 255, 255), green_mask);
            src_copy = green_mask.clone();
            cv::Mat kernel, kernel_1;
            kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            kernel_1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            cv::erode(src_copy, src_copy, kernel_1);
            cv::dilate(src_copy, src_copy, kernel);
            

            
            break;
        }
        case BLUE:
        {
            cv::Mat blue_mask;
            inRange(src_copy, cv::Scalar(90, 50, 50), cv::Scalar(130, 255, 255), blue_mask);
            src_copy = blue_mask.clone();
            cv::Mat kernel;
            kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
            cv::erode(src_copy, src_copy, kernel);
            cv::dilate(src_copy, src_copy, kernel);
            break;
        }
    }

    Canny(src_copy, src_copy, 0, 255, 3, true);

    dst = src_copy.clone();
}

uint8_t Part_3::search_max(cv::Mat src)
{
    uint8_t max = 0;

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uint8_t>(i, j) > max) max = src.at<uint8_t>(i, j);
        }
    }

    return max;
}

void Part_3::bin_threshhold(cv::Mat& dst, uint8_t t)
{
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if (dst.at<uint8_t>(i, j) >= t*0.98) dst.at<uint8_t>(i, j) = 255;
            else dst.at<uint8_t>(i, j) = 0;
        }
    }
}

void Part_3::calculate_centr(vector <vector<cv::Point>> contours, vector<cv::Point>& centr, bool isOne)
{
    if (isOne)
    {
        vector<cv::Point> tmp;

        for (int i = 0; i < contours.size(); i++)
        {
            cv::Moments moment = moments(contours[i]);

            float m00 = moment.m00;
            float m10 = moment.m10;
            float m01 = moment.m01;

            cv::Point result;
            result.x = m10 / m00;
            result.y = m01 / m00;

            tmp.push_back(result);
        }

        cv::Point n_res;

        for (int i = 0; i < tmp.size(); i++) n_res.x += tmp[i].x;
        for (int i = 0; i < tmp.size(); i++) n_res.y += tmp[i].y;
        n_res.x = n_res.x / tmp.size();
        n_res.y = n_res.y / tmp.size();

        centr.push_back(n_res);
    }
    else
    {
        for (int i = 0; i < contours.size(); i++)
        {
            cv::Moments moment = moments(contours[i]);

            float m00 = moment.m00;
            float m10 = moment.m10;
            float m01 = moment.m01;

            cv::Point result;
            result.x = m10 / m00;
            result.y = m01 / m00;

            centr.push_back(result);
        }
    }
}

void Part_3::draw_aim(cv::Mat src, cv::Mat& dst, vector<cv::Point>& centr)
{
    dst = src.clone();

    for (int i = 0; i < centr.size(); i++)
    {
        /*
            a[0]
        a[1]    a[2]
            a[3]
        */
        cv::Point2i aim[4];

        aim[0].x = centr[i].x;
        aim[0].y = centr[i].y + 4;
        aim[3].x = centr[i].x;
        aim[3].y = centr[i].y - 4;
        aim[1].x = centr[i].x - 4;
        aim[1].y = centr[i].y;
        aim[2].x = centr[i].x + 4;
        aim[2].y = centr[i].y;

        cv::line(dst, aim[0], aim[3], (0, 0, 0), 2, 8, 0);
        cv::line(dst, aim[1], aim[2], (0, 0, 0), 2, 8, 0);
    }
}

cv::Point Part_3::find_closest(vector<cv::Point>& points, cv::Point  object)
{
    int min = INT_MAX;
    cv::Point result;

    for (int i = 0; i < points.size(); i++)
    {
        if (sqrt(pow((points[i].x - object.x), 2) + pow((points[i].y - object.y), 2)) < min)
        {
            min = sqrt(pow((points[i].x - object.x), 2) + pow((points[i].y - object.y), 2));
            result = points[i];
        }
    }
    return result;
}

void Part_3::show()
{
    cv::imshow("image", m_image);
    while (cv::waitKey(1) != 'w');
}
void Part_3::show_result()
{
    cv::imshow("result", m_result);
    while (cv::waitKey(1) != 'w');
}

Part_4::Part_4(cv::Mat image, cv::Mat shape)
{
    m_image = image.clone();
    m_result = m_image.clone();
    m_shape = shape.clone();

    cv::cvtColor(m_result, m_result, cv::COLOR_RGB2GRAY);
    cv::cvtColor(m_shape, m_shape, cv::COLOR_RGB2GRAY);

    cv::threshold(m_result, m_result, 200, 255, cv::THRESH_BINARY_INV);
    
    cv::Mat kernel;
    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::dilate(m_result, m_result, kernel);
    cv::erode(m_result, m_result, kernel);

    Canny(m_result, m_result, 0, 255, 3, true);
    Canny(m_shape, m_shape, 0, 255, 3, true);

    vector <vector<cv::Point>> img_contours;
    vector <vector<cv::Point>> shp_contours;

    cv::findContours(m_result, img_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::findContours(m_shape, shp_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    m_result = image.clone();
  
    for (int i = 0; i < img_contours.size(); i++)
    { 
        double compare = cv::matchShapes(shp_contours[0], img_contours[i], cv::CONTOURS_MATCH_I2, 0);
 
        if (compare < 0.2) cv::polylines(m_result, img_contours[i], true, cv::Scalar(0, 255, 0), 3);
        else cv::polylines(m_result, img_contours[i], true, cv::Scalar(0, 0, 255), 3);
    }
}

Part_4::~Part_4()
{
    ;
}

void Part_4::show()
{
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::resizeWindow("image", 600, 400);
    imshow("image", m_image);
    while (cv::waitKey(1) != 'w');
}
void Part_4::show_result()
{
    cv::namedWindow("result", cv::WINDOW_NORMAL);
    cv::resizeWindow("result", 600, 400);
    cv::imshow("result", m_result);
    while (cv::waitKey(1) != 'w');
}
