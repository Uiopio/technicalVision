#include "gauss.h"

current_image::current_image(float sigma)
{
	m_defaultImage = 0;
	m_processingImage = 0;
	m_sigma = sigma;
}

int current_image::setImage(const string imageName) //получает изображение
{
	m_defaultImage = imread(imageName);
	m_processingImage = m_defaultImage;
	return 0;
}

Mat current_image::getImage() //возвращает изображение
{
	return m_processingImage;
}

int current_image::processImage() // обрабатывает изображение
{
	expandImage(m_processingImage);
	corePass(m_processingImage);
	return cropImage(m_processingImage);
}

int current_image::expandImage(Mat& image) //расширение границ, заполнение "пустоты" константным черным цветом
{
	if (image.empty())
	{
		return -1;
	}
	Mat biggerImage = Mat((image.rows + 6), (image.cols + 6), CV_8UC3, Scalar(0, 0, 0));
	for (int i = 0; i < image.rows; i++)
	{
		for (int k = 0; k < image.cols; k++)
		{
			for (int g = 0; g < 3; g++)
				biggerImage.at<Vec3b>(i + 6, k)[g] = image.at<Vec3b>(i, k)[g];
		}
	}
	image = biggerImage.clone();
	return 0;
}

int current_image::cropImage(Mat& image) //обрезаем картинку обратно
{
	if (image.empty())
		return -1;

	Mat croppedImage = Mat((image.rows - 6), (image.cols - 6), CV_8UC3, Scalar(255, 255, 255));

	for (int i = 6; i < image.rows; i++)
	{
		for (int k = 0; k < image.cols - 6; k++)
		{
			for (int g = 0; g < 3; g++)
				croppedImage.at<Vec3b>(i - 6, k)[g] = image.at<Vec3b>(i, k)[g];
		}
	}
	image = croppedImage.clone();
	return 0;
}

int current_image::corePass(Mat& image)  //сам проход ядром
{
	if (image.empty())
		return -1;

	double makeGauss[7][7] = {};
	double gSum = 0;
	for (int x = -3; x <= 3; x++)
	{
		for (int y = -3; y <= 3; y++)
		{
			makeGauss[x + 3][y + 3] = (1 / (2 * CV_PI * m_sigma * m_sigma)) * exp(-(x * x + y * y) / (2 * m_sigma * m_sigma));
			gSum += makeGauss[x + 3][y + 3];
		}
	}

	/*for (int i = 6; i < image.rows; i++)
	{
		for (int k = 0; k < image.cols - 6; k++)
		{
			for (int g = 0; g < 3; g++)
			{
				bool isAllPixelSame = true;
				const double firstPixel = image.at<Vec3b>(i, k)[g];
				double pixSum = 0;
				for (int r = 0; r <= 6; r++)
				{
					for (int c = 0; c <= 6; c++)
					{
						const double currentPixel = image.at<Vec3b>(i - r, k + c)[g];
						if (currentPixel != firstPixel)
							isAllPixelSame = false;
						pixSum += currentPixel * makeGauss[r][c];
					}
				}
				if (isAllPixelSame)
					continue;

				pixSum /= gSum;

				if (pixSum < 0) pixSum = 0;
				if (pixSum > 255) pixSum = 255;

				image.at<Vec3b>(i, k)[g] = pixSum;
			}
		}
	}*/

	// Цикл для проверки дефолтной библиотечной функции, не забудь раскомментить мэйн!
	for (int i = 6; i < image.rows-6; i++)
	{
		for (int k = 6; k < image.cols - 6; k++)
		{
			for (int g = 0; g < 3; g++)
			{
				bool isAllPixelSame = true;
				const double firstPixel = image.at<Vec3b>(i, k)[g];
				double pixSum = 0;
				for (int r = 0; r <= 6; r++)
				{
					for (int c = 0; c <= 6; c++)
					{
						const double currentPixel = image.at<Vec3b>(i + r - 3, k + c - 3)[g];
						if(currentPixel != firstPixel)
							isAllPixelSame = false;
						pixSum += currentPixel * makeGauss[r][c];
					}
				}
				if(isAllPixelSame)
					continue;

				pixSum /= gSum;

				if (pixSum < 0) pixSum = 0;
				if (pixSum > 255) pixSum = 255;

				image.at<Vec3b>(i, k)[g] = pixSum;
			}
		}
	}
	
	return 0;
}