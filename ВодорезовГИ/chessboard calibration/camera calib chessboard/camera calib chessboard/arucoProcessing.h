#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/aruco.hpp"

using namespace std;
using namespace cv;

static const float arucoMarkerLenght = 0.123f; //в метрах 
static Ptr <aruco::Dictionary> arucoDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_50);

//находим все маркеры, углы для каждого и матрицы поворота
void markersDetection(Mat image, vector<vector<Point2f>> &markerCorners, vector<int> &markerIds,
					  vector<Vec3d> &rotationVector, vector<Vec3d> &translationVector, Mat cameraMatrix, Mat distanceCoef);

//нахожденее вершин куба в мировых и экранных координатах и отрисовка куба
void drawCube(Mat &image, Vec3d &rotationVector, Vec3d &translationVector,
			  Mat cameraMatrix, Mat distanceCoef, int id, vector<Point2f> &imagePoints);

//добавление картинки на картинку в нужную область по точкам с гомографией
void addImageOnImageHomography(Mat src, Mat &dst, vector<Point2f> dstPoints);

//добавление изображений на куб по его координатам
void drawImagesOnCube(Mat &src, vector<Point2f> cubPoints, vector<Mat> cubImages);