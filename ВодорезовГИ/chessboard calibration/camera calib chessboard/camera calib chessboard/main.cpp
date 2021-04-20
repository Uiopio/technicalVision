#include "arucoProcessing.h"
#include "calibration.h"

#include "iostream"

using namespace std;
using namespace cv;


int main() 
{
	const int mode = 1;  //0 - калибровка, 1 - отрисовка куба

	if (mode == 0)
	{
		////////////////////// КАЛИБРОВКА КАМЕРЫ //////////////////////

		//загруждаем 17 изображений с шахматной доской
		vector<Mat> chessboardImages;

		loadImages(17, "chessboard/chess", chessboardImages);

		Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
		Mat distanceCoef;

		//калибруем (по шахматной доске)и сохраняем параметры
		cameraCalibration(chessboardImages, chessboardDimension, calibrationSquareLenght, cameraMatrix, distanceCoef);
		saveCalibParameters("calibParam.txt", cameraMatrix, distanceCoef);
	}
	else if (mode == 1)
	{
		////////////////////// ОТРИСОВКА КУБА НАД МАРКЕРОМ //////////////////////
		
		Mat cameraMatrix;
		Mat distanceCoef;
		vector<vector<Point2f>> markerCorners;
		vector<int> markerIds;
		vector<Vec3d> rotationVector, translationVector;

		//загружаем картинки для сторон куба
		vector<Mat> cubImages = { imread("cubs images/img5.jpg"), imread("cubs images/img2.jpg"), imread("cubs images/img3.jpg"),
								  imread("cubs images/img4.jpg"), imread("cubs images/img1.jpg") };

		//загружаем картинки игральных костей для сторон куба
		vector<Mat> diceImages = { imread("dice images/1.jpg"), imread("dice images/2.jpg"), imread("dice images/3.jpg"),
								  imread("dice images/4.jpg"), imread("dice images/5.jpg") };

		//загружаем параметры калибровки
		loadCalibParameters("calibParam.txt", cameraMatrix, distanceCoef);

		Mat frame;
		VideoCapture vid(0);
		if (!vid.isOpened())
		{
			return 0;
		}

		const int FPS = 30;
		namedWindow("Webcam", WINDOW_AUTOSIZE);

		while (true)
		{
			if (!vid.read(frame))
				break;

			//находим маркеры и матрицы преобразования до них
			markersDetection(frame, markerCorners, markerIds, rotationVector,
				translationVector, cameraMatrix, distanceCoef);

			//рисуем КуБиК
			for (int i = 0; i < markerIds.size(); i++)
			{
				vector <Point2f> cubeImagesPoints;
				drawCube(frame, rotationVector[i], translationVector[i], cameraMatrix, distanceCoef, markerIds[i], cubeImagesPoints);
				drawImagesOnCube(frame, cubeImagesPoints, diceImages);
			}

			imshow("Webcam", frame);

			int key = waitKey(1000/FPS);
			if (key == 27)
				break;
		}



		





		//aruco::drawAxis(image, cameraMatrix, distanceCoef,
		//				rotationVector, translationVector, 0.1);

		//cout << "rotationVector:" << endl;
		//for (int i = 0; i < markerIds.size(); i++)
		//	cout << rotationVector[i] << "\n" << endl;

		//cout << "translationVector:" << endl;
		//for (int i = 0; i < markerIds.size(); i++)
		//	cout << translationVector[i] << "\n" << endl;

		////выводим id маркеров и их углы
		//cout << "marker id:" << endl;
		//for(int i=0; i< markerIds.size(); i++)
		//	cout << markerIds[i] << "\n" << endl;

		//cout << "marker corners:" << endl;
		//for (int i = 0; i < markerCorners.size(); i++)
		//	cout << markerCorners[i] << "\n" << endl;

	}
	else cout << "mode error";

	waitKey(0);
	return 0;

	/*
	vector<vector<Point2f>> corners;
	//getChessboardCorners(chessboardImages, corners, true);


	Mat frame;
	Mat drawToFrame;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	Mat distanceCoef;

	vector<Mat> savedImages;
	
	vector<vector<Point2f>> markerCorners, rejectedCandidates;

	VideoCapture vid(0);

	if (!vid.isOpened())
	{
		return 0;
	}

	const int framePerSecond = 30;

	namedWindow("Webcam", WINDOW_AUTOSIZE);

	while (true)
	{
		if (!vid.read(frame))
			break;

		vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimension, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessboardDimension, foundPoints, found);

		if (found)
		{
			imshow("Webcam", drawToFrame);
		}
		else
		{
			imshow("Webcam", frame);
		}

		char key = waitKey(1000/framePerSecond);
	}
	*/
}

