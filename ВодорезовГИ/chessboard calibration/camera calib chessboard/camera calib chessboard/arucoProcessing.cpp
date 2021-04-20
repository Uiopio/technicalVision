#include "arucoProcessing.h"


void markersDetection(Mat image, vector<vector<Point2f>>& markerCorners, vector<int>& markerIds, vector<Vec3d>& rotationVector, vector<Vec3d>& translationVector, Mat cameraMatrix, Mat distanceCoef)
{
	aruco::detectMarkers(image, arucoDictionary, markerCorners, markerIds);
	aruco::estimatePoseSingleMarkers(markerCorners, arucoMarkerLenght, cameraMatrix,
		distanceCoef, rotationVector, translationVector);
}

void drawCube(Mat &image, Vec3d &rotationVector, Vec3d &translationVector,
	Mat cameraMatrix, Mat distanceCoef, int id, vector<Point2f> &imagePoints)
{
	const float len = arucoMarkerLenght;
	//vector<Point2f> imagePoints;
	vector<Point3f> pointWorld(8, cv::Point3d(0, 0, 0));

	//создаем куб в пространстве
	pointWorld[0] = Point3d(len / 2, len / 2, 0);
	pointWorld[1] = Point3d(len / 2, -len / 2, 0);
	pointWorld[2] = Point3d(-len / 2, -len / 2, 0);
	pointWorld[3] = Point3d(-len / 2, len / 2, 0);
	pointWorld[4] = Point3d(len / 2, len / 2, len);
	pointWorld[5] = Point3d(len / 2, -len / 2, len);
	pointWorld[6] = Point3d(-len / 2, -len / 2, len);
	pointWorld[7] = Point3d(-len / 2, len / 2, len);

	//переводим из пространства на плоскость маркера и координаты изображения
	projectPoints(pointWorld, rotationVector, translationVector,
				  cameraMatrix, distanceCoef, imagePoints);

	//отрисовываем
	Scalar nodeColor = Scalar(0,0, 255);
	Scalar edgeColor = Scalar(255, 0, 100);
	int edgeWidth = 2;
	int nodeWidth = 2;

	//грани
	for (int i = 1; i < 4; i++)
	{
		line(image, imagePoints[i - 1], imagePoints[i], edgeColor, edgeWidth);
	}
	line(image, imagePoints[0], imagePoints[3], edgeColor, edgeWidth);

	for (int i = 5; i < 8; i++)
	{
		line(image, imagePoints[i - 1], imagePoints[i], edgeColor, edgeWidth);
	}
	line(image, imagePoints[4], imagePoints[7], edgeColor, edgeWidth);

	line(image, imagePoints[0], imagePoints[4], edgeColor, edgeWidth);
	line(image, imagePoints[1], imagePoints[5], edgeColor, edgeWidth);
	line(image, imagePoints[2], imagePoints[6], edgeColor, edgeWidth);
	line(image, imagePoints[3], imagePoints[7], edgeColor, edgeWidth);

	//вершины-кружочки
	for (int i = 0; i < 8; i++)
	{
		circle(image, imagePoints[i], 2, nodeColor, nodeWidth);
		//cout << imagePoints[i] << endl;
	}

	//текст ID
	/*
	putText(image, "id = " + to_string(id), (imagePoints[7] + imagePoints[5]) / 2,
			FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
	*/
}

void addImageOnImageHomography(Mat src, Mat & dst, vector<Point2f> dstPoints)
{
	Mat homographyBuffer;
	vector <Point2f> srcPoints = { Point2f(0,0),   Point2f(src.rows, 0),  Point2f(src.rows, src.cols), Point2f(0, src.cols) };

	//находим матрицу гомографии и делаем деформацию изображения
	Mat H = findHomography(srcPoints, dstPoints);
	warpPerspective(src, homographyBuffer, H, dst.size());

	//создаем маску для наложения и заполняем черным
	Mat mask = Mat(dst.size(), CV_8UC3, Scalar(0, 0, 0));

	//костылим вектор на размерность больше для функции fillPoly() и заполняем белым область где будет наложено изобр.
	//тут кароче х*рота, все точки в Point2f, а fillPoly() просит Point2i, делаем ЛИЧНО ДЛЯ НЕЕ  
	vector<Point2i> dstPoints_temp;
	for (int i = 0; i < dstPoints.size(); i++)
		dstPoints_temp.push_back(Point2i((int) dstPoints[i].x, (int)dstPoints[i].y));

	vector<vector<Point2i>> dstPoints_wide = { dstPoints_temp };
	fillPoly(mask, dstPoints_wide, Scalar(255, 255, 255));

	//колдуем с маской и с итоговым изображением
	bitwise_not(mask, mask);
	bitwise_and(dst, mask, dst);
	bitwise_or(dst, homographyBuffer, dst);
}

void drawImagesOnCube(Mat & src, vector<Point2f> cubPoints, vector<Mat> cubImages)
{
	//распределяем точки по граням
	vector<Point2f> topSidePoints = { cubPoints[4], cubPoints[5], cubPoints[6], cubPoints[7]};
	vector <vector<Point2f>> SidePoints;

	SidePoints.push_back({cubPoints[0], cubPoints[1], cubPoints[5], cubPoints[4]});
	SidePoints.push_back({cubPoints[1], cubPoints[2], cubPoints[6], cubPoints[5] });
	SidePoints.push_back({ cubPoints[2], cubPoints[3], cubPoints[7], cubPoints[6] });
	SidePoints.push_back({ cubPoints[3], cubPoints[0], cubPoints[4], cubPoints[7]});

	//добавляем картинки на грани
	//вычисляем площадь граней
	int area[4];
	area[0] = (int)contourArea(SidePoints[0]);
	area[1] = (int)contourArea(SidePoints[1]);
	area[2] = (int)contourArea(SidePoints[2]);
	area[3] = (int)contourArea(SidePoints[3]);

	//вычисляем индексы сторон с наибольшей и наименьшей площадью из напротив стоящих сторон
	int max1Index, max2Index, min1Index, min2Index;

	if (area[0] > area[2])
	{
		max1Index = 0;
		min1Index = 2;
	}
	else
	{
		max1Index = 2;
		min1Index = 0;
	}

	if (area[1] > area[3])
	{
		max2Index = 1;
		min2Index = 3;
	}
	else
	{
		max2Index = 3;
		min2Index = 1;
	}

	//отображаем грани в магическом порядке, грань с самой большой площадью отображаем последней
	if (area[max1Index] > area[max2Index])
	{
		addImageOnImageHomography(cubImages[min1Index], src, SidePoints[min1Index]);
		addImageOnImageHomography(cubImages[min2Index], src, SidePoints[min2Index]);
		
		addImageOnImageHomography(cubImages[max2Index], src, SidePoints[max2Index]);
		addImageOnImageHomography(cubImages[max1Index], src, SidePoints[max1Index]);
	}
	else
	{
		addImageOnImageHomography(cubImages[min2Index], src, SidePoints[min2Index]);
		addImageOnImageHomography(cubImages[min1Index], src, SidePoints[min1Index]);
		
		addImageOnImageHomography(cubImages[max1Index], src, SidePoints[max1Index]);
		addImageOnImageHomography(cubImages[max2Index], src, SidePoints[max2Index]);
	}

	//крышу добавляем самой последней
	addImageOnImageHomography(cubImages[4], src, topSidePoints);
}

