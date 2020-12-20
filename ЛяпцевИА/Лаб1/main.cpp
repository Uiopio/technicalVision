// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "my_robot.h"

using namespace cv;
using namespace std;

int main()
{
	my_robot new_robo(100, 100, 5, 10, 15, 10);
	const Mat background(Size(1366, 768), CV_8UC3, Scalar(255, 255, 255));
	new_robo.setCenter(background);
	while (waitKey(1) != 27)
	{
		int errorCode = 0;
		Mat outputimage = background.clone();

		switch (waitKey(1))
		{
		case 'w': //вверх
			errorCode = new_robo.move(1);
			break;
		case 's': //вниз
			errorCode = new_robo.move(2);
			break;
		case 'a': //влево
			errorCode = new_robo.move(3);
			break;
		case 'd': //вправо
			errorCode = new_robo.move(4);
			break;
		case 'q': //по часовой
			errorCode = new_robo.rotate(-1);
			break;
		case 'e': //против часовой
			errorCode = new_robo.rotate(1);
			break;
		default:;
		}

		new_robo.setArea(outputimage);
		new_robo.draw(outputimage);

		if (errorCode == -2 || errorCode == -1)
		{
			putText(outputimage, to_string(errorCode), cv::Point(outputimage.cols / 2, outputimage.rows / 2),
				FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
			imshow("YoyHaveAnError!!!", Mat(Size(1366, 768), CV_8UC3, Scalar(255, 0, 0)));
			while (1);
		}
		imshow("YourRobot", outputimage);
	}
	return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.