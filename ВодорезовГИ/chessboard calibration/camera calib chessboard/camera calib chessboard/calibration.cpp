#include "calibration.h"


//создание идеальной шахматной доски
void createKnownBoardPosition(Size boardSize, float squareEngeLenght, vector<Point3f>& corners)
{
	for (int i = 0; i < boardSize.height; i++)
	{
		for (int j = 0; j < boardSize.width; j++)
		{
			corners.push_back(Point3f(j*squareEngeLenght, i*squareEngeLenght, 0.0f));
		}
	}
}

//возваращает координаты углов шахматной доски (для вектора изображений)
void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResult)
{
	for (vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++)
	{
		vector<Point2f> pointBuf;
		bool found = findChessboardCorners(*iter, chessboardDimension, pointBuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		if (found)
		{
			allFoundCorners.push_back(pointBuf);
		}

		if (showResult)
		{
			drawChessboardCorners(*iter, chessboardDimension, pointBuf, found);
			imshow("chessboard  with corners", *iter);
			while (waitKey(0) != 27);
		}
	}
}

//функция калибровки камеры
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLenght, Mat &cameraMatrix, Mat &distanceCoef)
{
	vector<vector<Point2f>> chessboardImagePoints;
	getChessboardCorners(calibrationImages, chessboardImagePoints, false);

	vector<vector<Point3f>> worldSpaceCornerPoints(1);

	createKnownBoardPosition(boardSize, squareEdgeLenght, worldSpaceCornerPoints[0]);
	worldSpaceCornerPoints.resize(chessboardImagePoints.size(), worldSpaceCornerPoints[0]);

	vector<Mat> rVector, tVector;
	distanceCoef = Mat::zeros(8, 1, CV_64F);

	calibrateCamera(worldSpaceCornerPoints, chessboardImagePoints, boardSize,
		cameraMatrix, distanceCoef, rVector, tVector);

}

void loadImages(int number, String path, vector<Mat>& imageVector)
{
	for (int i = 0; i < number; i++)
	{
		String tempPath = path +  to_string(i) + ".jpg";
		Mat tempImg = imread(tempPath);
		imageVector.push_back(tempImg);
	}

}

void saveCalibParameters(String name, Mat cameraMatrix, Mat distCoef)
{
	//2 строки - кол-вло строк и столбцов, потом cameraMatrix значения
	//2 строки - кол-вло строк и столбцов, потом distCoef значения

	//открываем текстовый файл name
	//если файла не сущ. - будет создан, если что-то уже записано - удалено и перезаписано
	ofstream outStream(name);
	if (!outStream)
	{
		cout << "stream error";
		return;
	}

	//записываем коэффиценты cameraMatrix
	int rows = cameraMatrix.rows;
	int cols = cameraMatrix.cols;

	outStream << rows << endl;
	outStream << cols << endl;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double value = cameraMatrix.at<double>(r,c);
			outStream << value << endl;
		}
	}

	//записываем коэффиценты distCoef
	rows = distCoef.rows;
	cols = distCoef.cols;

	outStream << rows << endl;
	outStream << cols << endl;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double value = distCoef.at<double>(r, c);
			outStream << value << endl;
		}
	}

	//закрываем запись
	outStream.close();
}

void loadCalibParameters(String name, Mat & cameraMatrix, Mat & distCoef)
{
	//2 строки - кол-вло строк и столбцов, потом cameraMatrix значения
	//2 строки - кол-вло строк и столбцов, потом distCoef значения

	//открываем текстовый файл name
	ifstream inStream(name);
	if (!inStream)
	{
		cout << "stream error";
		return;
	}

	//считываем коэффиценты cameraMatrix
	int rows;
	int cols;

	inStream >> rows;
	inStream >> cols;

	cameraMatrix = Mat(Size(cols, rows), CV_64F);

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double value = 0.0f;
			inStream >> value;
			cameraMatrix.at<double>(r, c) = value;
		}
	}

	//считываем коэффиценты distCoef
	inStream >> rows;
	inStream >> cols;

	distCoef = Mat(Size(cols, rows), CV_64F);

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			double value;
			inStream >> value;
			distCoef.at<double>(r, c) = value;
		}
	}

	//закрываем запись
	inStream.close();
}

