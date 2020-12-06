#include "Recognition.h"
int Counter = 0;

void Task1(string fileName)
{
	/*
	  Задание 1, тут надо поместить прицел в центр белого пятна
	  1. Пороговая фильтрация, все что темнее порога шлем нафиг
	  2. Создаем и находим контур (п. 6 задания)
	  3. Находим координаты центра масс контура (п. 8, через моменты)
	  4. Рисуем в этой точке на исходном изображении красивый прицел
	 */

	const string fullFillName = "C://Users//Ilya//source//repos//Lab4_ComputerVision//Lab4_ComputerVision//Задание//Аллахбабах//ig_" + fileName + ".jpg";
	const Mat inputImage = imread(fullFillName, IMREAD_GRAYSCALE);
	double thresh = 220;
	Mat ImageToWork;

	//1
	threshold(inputImage, ImageToWork, thresh, 255, THRESH_BINARY);
	erode(ImageToWork, ImageToWork, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dilate(ImageToWork, ImageToWork, getStructuringElement(MORPH_RECT, Size(3, 3)));

	//2
	Mat edges;
	Canny(ImageToWork, edges, 0, 0, 3, false);

	const Mat imageWithContours = edges.clone();
	vector<vector<Point>> contours;
	findContours(imageWithContours, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//3
	Moments mnts = moments(contours[0]);
	double m00 = mnts.m00;
	double m10 = mnts.m10;
	double m01 = mnts.m01;

	Point center(0, 0);
	center.x = m10 / m00;
	center.y = m01 / m00;

	//4
	Mat outputImage;
	cvtColor(inputImage, outputImage, COLOR_GRAY2BGR);

	line(outputImage, Point(center.x - 15, center.y), Point(center.x + 15, center.y), Scalar(0, 0, 255), 2);
	line(outputImage, Point(center.x, center.y - 15), Point(center.x, center.y + 15), Scalar(0, 0, 255), 2);

	string imageName = std::to_string(Counter) + "Result";
	imshow(imageName, outputImage);
	Counter++;
	while (waitKey(1) != 27)
	{
	}
}

void Task2(string fileName)
{
	/*
		Задание 2, тут чисто то же, что и по заданию 1
		Одно отличие (сноска 1`)- светлых мест много, поэтому мы находим место с наибольшим по периметру контуром
	 */

	const string fullFillName = "C://Users//Ilya//source//repos//Lab4_ComputerVision//Lab4_ComputerVision//Задание//Тепловизор//" + fileName;
	const Mat inputImage = imread(fullFillName);
	cvtColor(inputImage, inputImage, COLOR_BGR2HSV);

	Mat ImageToWork;
	inRange(inputImage, Scalar(34, 101, 0), Scalar(164, 255, 255), ImageToWork);
	dilate(ImageToWork, ImageToWork, getStructuringElement(MORPH_RECT, Size(3, 3)));
	erode(ImageToWork, ImageToWork, getStructuringElement(MORPH_RECT, Size(3, 3)));

	Mat edges;
	Canny(ImageToWork, edges, 0, 0, 3, false);

	Mat imageWithContours = edges.clone();
	vector<vector<Point>> contours;
	findContours(imageWithContours, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<Moments> mnts;
	for (int i = 0; i < contours.size(); i++)
	{
		mnts.push_back(moments(contours[i]));
	}

	int i_max = 0;
	double tempArea = mnts[0].m00;

	for (int i = 0; i < contours.size(); i++)
	{
		if (mnts[i].m00 >= tempArea)
		{
			i_max = i;
			tempArea = mnts[i].m00;
		}
	}

	Moments biggestMoment = mnts[i_max];
	double m00 = biggestMoment.m00;
	double m10 = biggestMoment.m10;
	double m01 = biggestMoment.m01;

	Point center(0, 0);
	center.x = m10 / m00;
	center.y = m01 / m00;

	Mat outputImage = inputImage.clone();

	cvtColor(inputImage, outputImage, COLOR_HSV2BGR);
	line(outputImage, Point(center.x - 15, center.y), Point(center.x + 15, center.y), Scalar(0, 0, 255), 2);
	line(outputImage, Point(center.x, center.y - 15), Point(center.x, center.y + 15), Scalar(0, 0, 255), 2);

	string imageName = std::to_string(Counter) + "Result";
	imshow(imageName, outputImage);
	Counter++;
	while (waitKey(1) != 27)
	{
	}
}

//Создаем маску и удаляем все ненужное для каждого кейса, в которых задаем границы
Mat maskDrawing(const int flag, const Mat inputImage)
{
	Scalar low;
	Scalar high;
	int dilateSize = 1;
	int erodeSize = 1;
	string color;

	if (flag == 0) //Лампа
	{
		Scalar lampLow(107, 0, 255);
		low = lampLow;
		Scalar lampHigh(180, 255, 255);
		high = lampHigh;
		dilateSize = 10;
		erodeSize = 1;
		color = "Lamp";
	}
	if (flag == 1) //Голубой
	{
		Scalar blueLow(82, 50, 0);
		Scalar blueHigh(116, 255, 255);
		low = blueLow;
		high = blueHigh;
		dilateSize = 13;
		erodeSize = 18;
		color = "Blue";
	}
	if (flag == 2) //Зеленый
	{
		Scalar greenLow(43, 52, 113);
		Scalar greenHigh(84, 255, 255);
		low = greenLow;
		high = greenHigh;
		dilateSize = 2;
		erodeSize = 7;
		color = "Green";
	}
	if (flag == 3) //Красный
	{
		Scalar redLow(10, 0, 0);
		Scalar redHigh(116, 255, 255);
		low = redLow;
		high = redHigh;
		dilateSize = 13;
		erodeSize = 16;
		color = "Red";
	}

	Mat tempMask;
	inRange(inputImage, low, high, tempMask);
	dilate(tempMask, tempMask, getStructuringElement(MORPH_RECT, Size(dilateSize, dilateSize)));
	erode(tempMask, tempMask, getStructuringElement(MORPH_RECT, Size(erodeSize, erodeSize)));

	//imshow(color, tempMask);
	waitKey();
	return tempMask;
}

//Ну тут все просто, имея вектор значений расстояний как корней из сумм квадратов координат центра, ищем минимальную
int findNearest(vector<Point> centerRobot, Point centerLamp)
{
	vector<double> distance;
	for (int i = 0; i < centerRobot.size(); i++)
	{
		const double a1 = abs(centerRobot[i].x - centerLamp.x);
		const double a2 = abs(centerRobot[i].y - centerLamp.y);
		double temp = sqrt(a1 * a1 + a2 * a2);
		distance.push_back(temp);
	}

	double minDistance = distance[0];
	int indexMin = 0;
	for (int i = 0; i < distance.size(); i++)
	{
		if (distance[i] <= minDistance)
		{
			minDistance = distance[i];
			indexMin = i;
		}
	}
	return indexMin;
}

void Task3(string fileName)
{
	/*
	   Задание 3, тут находим всяике штуки и рисуем их
	   1. Найти цветную крышку у каждого робота и обвести
	   creationMask для соответствующего цвета

	   2. Найти лампу и обозначить
	   Все та же функция creationMask

	   3. Найти ближайшего к лампе и нарисовать его центр масс
	   searchMinDistance
	*/

	const string fullFillName = "C://Users//Ilya//source//repos//Lab4_ComputerVision//Lab4_ComputerVision//Задание//Роботы//roi_robotov_" + fileName + ".jpg";
	const Mat inputImage = imread(fullFillName);
	Mat outputImage = inputImage.clone();
	cvtColor(inputImage, inputImage, COLOR_BGR2HSV);

	//2
	Mat lampMask = maskDrawing(0, inputImage);
	Mat lampEdges;
	vector<vector<Point>> cntsLamp;
	Canny(lampMask, lampEdges, 0, 0, 3, false);
	Mat tempLamp = lampEdges.clone();
	findContours(tempLamp, cntsLamp, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Moments mntsLamp = moments(cntsLamp[0]);
	double m00Lamp = mntsLamp.m00;
	double m10Lamp = mntsLamp.m10;
	double m01Lamp = mntsLamp.m01;

	Point centerLamp(0, 0);
	centerLamp.x = m10Lamp / m00Lamp;
	centerLamp.y = m01Lamp / m00Lamp;
	circle(outputImage, centerLamp, 8, Scalar(0, 0, 0), FILLED, 8, 0);

	cvtColor(inputImage, inputImage, COLOR_HSV2BGR);

	//1
	Scalar color(inputImage.at<Vec3b>(10, 10)[0], inputImage.at<Vec3b>(10, 10)[1], inputImage.at<Vec3b>(10, 10)[2]);
	ellipse(inputImage, Point(centerLamp.x, centerLamp.y - 25), Size(40, 22), 17, 0, 360, color, FILLED, 8, 0);
	//imshow("inputImage", inputImage);
	waitKey();
	cvtColor(inputImage, inputImage, COLOR_BGR2HSV);

	//Определяем контуры для синего цвета, создав маску функцией creationMask, далее имея маску создаем по ней все контуры для нужного цвета
	Mat blueMask = maskDrawing(1, inputImage);
	Mat blueEdges;
	vector<vector<Point>> cntsBlue;

	Canny(blueMask, blueEdges, 0, 0, 3, false);
	Mat tempBlue = blueEdges.clone();
	findContours(tempBlue, cntsBlue, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//Аналогично определяем красный цвет
	Mat redMask = maskDrawing(3, inputImage);
	Mat redEdges;
	vector<vector<Point>> cntsRed;

	Canny(redMask, redEdges, 0, 0, 3, false);
	Mat tempRed = redEdges.clone();
	findContours(tempRed, cntsRed, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//Аналогично определяем зеленый цвет
	Mat greenMask = maskDrawing(2, inputImage);
	Mat greenEdges;
	vector<vector<Point>> cntsGreen;

	Canny(greenMask, greenEdges, 0, 0, 3, false);
	Mat tempGreen = greenEdges.clone();
	findContours(tempGreen, cntsGreen, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//Для красного цвета находим моменты как в предыдущих заданиях, далее для нахождения минимального расстояния создаем вектор
	vector<Moments> mntsRed;
	for (int i = 0; i < cntsRed.size(); i++)
	{
		mntsRed.push_back(moments(cntsRed[i]));
	}
	vector<Point> centerRed;
	for (int i = 0; i < cntsRed.size(); i++)
	{
		double m00 = mntsRed[i].m00;
		double m10 = mntsRed[i].m10;
		double m01 = mntsRed[i].m01;

		centerRed.push_back(Point((m10 / m00), (m01 / m00)));
	}

	//Аналогично определяем синий цвет
	vector<Moments> mntsBlue;
	for (int i = 0; i < cntsBlue.size(); i++)
	{
		mntsBlue.push_back(moments(cntsBlue[i]));
	}
	vector<Point> centerBlue;
	for (int i = 0; i < cntsBlue.size(); i++)
	{
		double m00 = mntsBlue[i].m00;
		double m10 = mntsBlue[i].m10;
		double m01 = mntsBlue[i].m01;

		centerBlue.push_back(Point((m10 / m00), (m01 / m00)));
	}

	//Аналогично определяем зеленый цвет
	vector<Moments> mntsGreen;
	for (int i = 0; i < cntsGreen.size(); i++)
	{
		mntsGreen.push_back(moments(cntsGreen[i]));
	}
	vector<Point> centerGreen;
	for (int i = 0; i < cntsGreen.size(); i++)
	{
		double m00 = mntsGreen[i].m00;
		double m10 = mntsGreen[i].m10;
		double m01 = mntsGreen[i].m01;

		centerGreen.push_back(Point((m10 / m00), (m01 / m00)));
	}

	//Находим минимальное расстояние для каждого цвета
	int indMinBlue = findNearest(centerBlue, centerLamp);
	int indMinGreen = findNearest(centerGreen, centerLamp);
	int indMinRed = findNearest(centerRed, centerLamp);

	//Обводим контуры роботов
	polylines(outputImage, cntsBlue, true, Scalar(255, 0, 0), 1, 16);
	polylines(outputImage, cntsRed, true, Scalar(0, 0, 255), 1, 16);
	polylines(outputImage, cntsGreen, true, Scalar(0, 255, 0), 1, 16);

	//Рисуем пути до лампы
	line(outputImage, centerRed[indMinRed], centerLamp, Scalar(0, 0, 255), 2);
	line(outputImage, centerBlue[indMinBlue], centerLamp, Scalar(255, 0, 0), 2);
	line(outputImage, centerGreen[indMinGreen], centerLamp, Scalar(0, 255, 0), 2);

	string imageName = std::to_string(Counter) + "Result";
	imshow(imageName, outputImage);
	Counter++;
	while (waitKey(1) != 27)
	{
	}
}

void Task4()
{
	/*
	   Задание 4, тут ищем неправильные гаечные ключи по шаблону
	   1. Создаем маски всего и вся

	   2. Находим контур шаблона и много-много контуров ключей-испытуемых

	   3. Сравниваем их функцией matchShapes
	*/

	const string fullFillName = "C://Users//Ilya//source//repos//Lab4_ComputerVision//Lab4_ComputerVision//Задание//Гаечные ключи//gk.jpg";
	const string fullFillTemplateName = "C://Users//Ilya//source//repos//Lab4_ComputerVision//Lab4_ComputerVision//Задание//Гаечные ключи//gk_tmplt.jpg";
	const Mat inputImage = imread(fullFillName, IMREAD_GRAYSCALE);
	const Mat Template = imread(fullFillTemplateName, IMREAD_GRAYSCALE);

	//Маска вообще для всех ключей
	Mat maskAll;
	threshold(inputImage, maskAll, 229, 255, THRESH_BINARY);
	dilate(maskAll, maskAll, getStructuringElement(MORPH_RECT, Size(2, 2)));
	erode(maskAll, maskAll, getStructuringElement(MORPH_RECT, Size(3, 3)));

	//Контур вообще для всех ключей
	Mat edgesAll;
	Canny(maskAll, edgesAll, 0, 0, 3, false);
	Mat temp1 = edgesAll.clone();
	vector<vector<Point>> cntsAll;
	findContours(temp1, cntsAll, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//Маска для нашего шаблона
	Mat maskTemplate;
	threshold(Template, maskTemplate, 100, 255, THRESH_BINARY);
	dilate(maskTemplate, maskTemplate, getStructuringElement(MORPH_RECT, Size(2, 2)));
	erode(maskTemplate, maskTemplate, getStructuringElement(MORPH_RECT, Size(3, 3)));

	//Контур нашего шаблона
	Mat edgesTemplate;
	Canny(maskTemplate, edgesTemplate, 0, 0, 3, false);
	Mat temp2 = edgesTemplate.clone();
	vector<vector<Point>> cntsTemplate;
	findContours(temp2, cntsTemplate, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//Ищем различия между каждым контуром и контуром шаблона, находя разность моментов через matchShapes, если больше 1, то отбраковываем
	vector<double> likeness;
	cvtColor(inputImage, inputImage, COLOR_GRAY2BGR);
	for (int i = 0; i < cntsAll.size(); i++)
	{
		likeness.push_back(matchShapes(cntsTemplate[0], cntsAll[i], ShapeMatchModes::CONTOURS_MATCH_I2, 0));
		cout << likeness[i] << endl;
		if (likeness[i] <= 1)
		{
			polylines(inputImage, cntsAll[i], true, Scalar(0, 255, 0), 1, 16);
		}
		if (likeness[i] > 1)
		{
			polylines(inputImage, cntsAll[i], true, Scalar(0, 0, 255), 1, 16);
		}
	}
	imshow("result", inputImage);
	while (waitKey(1) != 27)
	{
	}
}