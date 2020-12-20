#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

//основная функция обработки изображения
int OtsuMethod(const Mat& image);

int OtsuMethod(const Mat& image, Mat& dst);

int Comparision(const Mat& originImage);
