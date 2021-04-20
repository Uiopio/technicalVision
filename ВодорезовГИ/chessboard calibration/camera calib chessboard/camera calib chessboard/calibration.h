#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

static const float calibrationSquareLenght = 0.023f;
static const Size chessboardDimension = Size(6, 9);


//создание идеальной шахматной доски
void createKnownBoardPosition(Size boardSize, float squareEngeLenght, vector<Point3f>& corners);

//возваращает координаты углов шахматной доски (для вектора изображений)
void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResult = false);

//функция калибровки камеры
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLenght, Mat &cameraMatrix, Mat &distanceCoef);

//загружаем изображения
void loadImages(int number, String path, vector<Mat> &imageVector);

void saveCalibParameters(String name, Mat cameraMatrix, Mat distCoef);

void loadCalibParameters(String name, Mat &cameraMatrix, Mat &distCoef);

