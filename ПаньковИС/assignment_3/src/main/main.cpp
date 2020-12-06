#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "fast_furier_transformer.h"
#include "filter.h"

#define KERNEL_COLS 3
#define KERNEL_ROWS 3 

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{    
    /***** Задание 1. Прямое и обратное преобразования Фурье *****/

    Mat1f image_1 = imread("src/images/lena.bmp", IMREAD_GRAYSCALE);
    image_1 /= static_cast<float>(0xFF);
 
    // Реализованный класс FastFurierTransformer
    auto fft_1 = FastFurierTransformer();
    fft_1.setImage(image_1.clone());

    fft_1.directFastFurierTransform();
    fft_1.inverseFastFurierTransform();

    fft_1.showSpectrum();
    fft_1.showImage();

    // Встроенные функции
    auto cols = getOptimalDFTSize(image_1.cols);
    auto rows = getOptimalDFTSize(image_1.rows);

    auto openCVSpectrum_1 = Mat2f(Size2i(cols, rows), Vec2f(0, 0));
    auto openCVImage_1 = Mat1f(Size2i(cols, rows), 0);
    auto openCVImageROI_1 = Mat1f(image_1.clone());
    openCVImageROI_1.copyTo(openCVImage_1(Rect(0, 0, image_1.cols, image_1.rows)));

    dft(openCVImage_1, openCVSpectrum_1, DFT_COMPLEX_OUTPUT);
    idft(openCVSpectrum_1, openCVImage_1, DFT_REAL_OUTPUT);

    Mat1f openCVSpectrumComplex_1[NUM_OF_COMPLEX_PARTS];
    split(openCVSpectrum_1, openCVSpectrumComplex_1);
    auto openCVSpectrumMagnitude_1 = Mat1f();

    magnitude(openCVSpectrumComplex_1[RE], openCVSpectrumComplex_1[IM], openCVSpectrumMagnitude_1);
    shiftSpectrum(openCVSpectrumMagnitude_1, cols / 2, rows / 2);

    openCVSpectrumMagnitude_1 += Scalar::all(1);
    log(openCVSpectrumMagnitude_1, openCVSpectrumMagnitude_1);
    normalize(openCVSpectrumMagnitude_1, openCVSpectrumMagnitude_1, 0, 1, NORM_MINMAX);

    imshow("Spectrum [OpenCV]", openCVSpectrumMagnitude_1);
    imshow("Image [OpenCV]", openCVImageROI_1);

    while (waitKey() != 27);

    // Сравнение спектров и изображений
    auto diffSpectrum = Mat1f();
    auto diffImage = Mat1f();

    auto fftSpectrumMagnitude_1 = fft_1.getSpectrumMagnitude();
    auto fftImage_1 = fft_1.getImage();

    absdiff(fftSpectrumMagnitude_1, openCVSpectrumMagnitude_1, diffSpectrum);
    imshow("Spectrum Difference", diffSpectrum);

    absdiff(fftImage_1, openCVImageROI_1, diffImage);
    imshow("Image Difference", diffImage);

    while (waitKey() != 27);

    /********************** Конец задания 1 **********************/

    /********* Задание 2. Фильтры верхних и нижних частот ********/

    Mat1f image_2 = imread("src/images/lena.bmp", IMREAD_GRAYSCALE);
    image_2 /= static_cast<float>(0xFF);

    // 1) Фильтр верхних частот
    auto fft_2_hpf = FastFurierTransformer();
    fft_2_hpf.setImage(image_2.clone());

    auto hpf = Filter();
    hpf.configure(FilterType::HIGH_PASS, FilterName::BUTTERWORTH, 50, 5);

    fft_2_hpf.setFilter(hpf);
    fft_2_hpf.filtrateImage();

    fft_2_hpf.showSpectrum("Spectrum with High Pass Butterworth Filter");
    fft_2_hpf.showImage("Image with High Pass Butterworth Filter");

    while (waitKey() != 27);

    // 2) Фильтр нижних частот
    auto fft_2_lpf = FastFurierTransformer();
    fft_2_lpf.setImage(image_2.clone());

    auto lpf = Filter();
    lpf.configure(FilterType::LOW_PASS, FilterName::BUTTERWORTH, 50, 5);

    fft_2_lpf.setFilter(lpf);
    fft_2_lpf.filtrateImage();

    fft_2_lpf.showSpectrum("Spectrum with Low Pass Butterworth Filter");
    fft_2_lpf.showImage("Image with Low Pass Butterworth Filter");

    while (waitKey() != 27);

    /********************** Конец задания 2 **********************/

    /** Задание 3. Фурье-образы свёртки изображения с фильтрами **/

    Mat1f image_3 = imread("src/images/lena.bmp", IMREAD_GRAYSCALE);
    image_3 /= static_cast<float>(0xFF);

    cols = getOptimalDFTSize(image_3.cols + KERNEL_COLS - 1);
    rows = getOptimalDFTSize(image_3.rows + KERNEL_ROWS - 1);

    // 1) Фильтр Собеля

    // а) Горизонтальный оператор Собеля
    float sobelH[KERNEL_COLS][KERNEL_ROWS] =
    {
        { 1.0,  2.0,  1.0},
        { 0.0,  0.0,  0.0},
        {-1.0, -2.0, -1.0}
    };
    auto sobelOperatorH = Mat1f(KERNEL_COLS, KERNEL_ROWS, *sobelH);
    
    auto fft_sobelH = FastFurierTransformer();
    fft_sobelH.setImage(image_3.clone());
    fft_sobelH.setSpectrumSize(Size2i(cols, rows));
    fft_sobelH.directFastFurierTransform();

    auto fft_sobelFilterH = FastFurierTransformer();
    fft_sobelFilterH.setImage(sobelOperatorH);
    fft_sobelFilterH.setSpectrumSize(Size2i(cols, rows));
    fft_sobelFilterH.directFastFurierTransform();

    fft_sobelH.showSpectrum("Image Spectrum");
    fft_sobelFilterH.showSpectrum("Horizontal Sobel Operator Spectrum");

    auto sobelSpectrumH = fft_sobelH.getSpectrum();
    auto sobelOperatorSpectrumH = fft_sobelFilterH.getSpectrum();
    multiplySpectrums(sobelSpectrumH, sobelOperatorSpectrumH, sobelSpectrumH);
    
    fft_sobelH.setSpectrum(sobelSpectrumH);
    fft_sobelH.showSpectrum("Horizontal Sobel Operator Spectrum Result");

    while (waitKey() != 27);

    // б) Вертикальный оператор Собеля
    float sobelV[KERNEL_COLS][KERNEL_ROWS] =
    {
        { 1.0,  0.0, -1.0},
        { 2.0,  0.0, -2.0},
        { 1.0,  0.0, -1.0}
    };
    auto sobelOperatorV = Mat1f(KERNEL_COLS, KERNEL_ROWS, *sobelV);

    auto fft_sobelV = FastFurierTransformer();
    fft_sobelV.setImage(image_3.clone());
    fft_sobelV.setSpectrumSize(Size2i(cols, rows));
    fft_sobelV.directFastFurierTransform();

    auto fft_sobelFilterV = FastFurierTransformer();
    fft_sobelFilterV.setImage(sobelOperatorV);
    fft_sobelFilterV.setSpectrumSize(Size2i(cols, rows));
    fft_sobelFilterV.directFastFurierTransform();

    fft_sobelV.showSpectrum("Image Spectrum");
    fft_sobelFilterV.showSpectrum("Vertical Sobel Operator Spectrum");

    auto sobelSpectrumV = fft_sobelV.getSpectrum();
    auto sobelOperatorSpectrumV = fft_sobelFilterV.getSpectrum();
    multiplySpectrums(sobelSpectrumV, sobelOperatorSpectrumV, sobelSpectrumV);

    fft_sobelV.setSpectrum(sobelSpectrumV);
    fft_sobelV.showSpectrum("Vertical Sobel Operator Spectrum Result");

    while (waitKey() != 27);

    // в) Свёртка горизотального и вертикального операторов Собеля
    auto fft_sobel = FastFurierTransformer();
    fft_sobel.setImage(image_3.clone());
    fft_sobel.setSpectrumSize(Size2i(cols, rows));
    fft_sobel.directFastFurierTransform();
    
    auto sobelSpectrum = fft_sobel.getSpectrum();
    auto sobelFilterSpectrum = Mat2f();
    multiplySpectrums(sobelOperatorSpectrumH, sobelOperatorSpectrumV, sobelFilterSpectrum);

    auto fft_sobelFilter = FastFurierTransformer();
    fft_sobelFilter.setSpectrum(sobelFilterSpectrum);

    fft_sobel.showSpectrum("Image Spectrum");
    fft_sobelFilter.showSpectrum("Sobel Filter Spectrum");

    multiplySpectrums(sobelSpectrum, sobelFilterSpectrum, sobelSpectrum);

    fft_sobel.setSpectrum(sobelSpectrum);
    fft_sobel.showSpectrum("Sobel Filter Spectrum Result");

    while (waitKey() != 27);

    // 2) Усредняющий фильтр (Box Filter)
    float box[KERNEL_COLS][KERNEL_ROWS] =
    {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    auto boxFilterKernel = Mat1f(KERNEL_COLS, KERNEL_ROWS, *box);

    auto fft_box = FastFurierTransformer();
    fft_box.setImage(image_3.clone());
    fft_box.setSpectrumSize(Size2i(cols, rows));
    fft_box.directFastFurierTransform();

    auto fft_boxFilter = FastFurierTransformer();
    fft_boxFilter.setImage(boxFilterKernel);
    fft_boxFilter.setSpectrumSize(Size2i(cols, rows));
    fft_boxFilter.directFastFurierTransform();

    fft_box.showSpectrum("Image Spectrum");
    fft_boxFilter.showSpectrum("Box Filter Spectrum");

    auto boxSpectrum = fft_box.getSpectrum();
    auto boxFilterSpectrum = fft_boxFilter.getSpectrum();
    multiplySpectrums(boxSpectrum, boxFilterSpectrum, boxSpectrum);

    fft_box.setSpectrum(boxSpectrum);
    fft_box.showSpectrum("Box Filter Spectrum Result");

    while (waitKey() != 27);

    // 3) Фильтр Лапласа
    float laplace[KERNEL_COLS][KERNEL_ROWS] =
    {
        {0.0,  1.0,  0.0},
        {1.0, -4.0,  1.0},
        {0.0,  1.0,  0.0}
    };

    auto laplaceFilterKernel = Mat1f(KERNEL_COLS, KERNEL_ROWS, *laplace);

    auto fft_laplace = FastFurierTransformer();
    fft_laplace.setImage(image_3.clone());
    fft_laplace.setSpectrumSize(Size2i(cols, rows));
    fft_laplace.directFastFurierTransform();

    auto fft_laplaceFilter = FastFurierTransformer();
    fft_laplaceFilter.setImage(laplaceFilterKernel);
    fft_laplaceFilter.setSpectrumSize(Size2i(cols, rows));
    fft_laplaceFilter.directFastFurierTransform();

    fft_laplace.showSpectrum("Image Spectrum");
    fft_laplaceFilter.showSpectrum("Laplace Filter Spectrum");

    auto laplaceSpectrum = fft_laplace.getSpectrum();
    auto laplaceFilterSpectrum = fft_laplaceFilter.getSpectrum();
    multiplySpectrums(laplaceSpectrum, laplaceFilterSpectrum, laplaceSpectrum);

    fft_laplace.setSpectrum(laplaceSpectrum);
    fft_laplace.showSpectrum("Laplace Filter Spectrum Result");

    while (waitKey() != 27);

    /**** Задание 4. Результаты свёртки изображения с фильтрами ***/

    // 1) Фильтр Собеля

    // a) Горизонтальный оператор Собеля
    fft_sobelH.setImageShift(Point2i(KERNEL_COLS / 2, KERNEL_ROWS / 2));
    fft_sobelH.inverseFastFurierTransform();
    fft_sobelH.showSpectrum("Horizontal Sobel Operator Spectrum Result");
    fft_sobelH.showImage("Horizontal Sobel Operator Image Result");

    while (waitKey() != 27);

    // Сравнение горизонтального оператора Собеля со встроенным фильтром
    auto fftSobelHImage = fft_sobelH.getImage();
    auto openCVSobelHImage = Mat1f();
    Sobel(image_3, openCVSobelHImage, CV_32FC1, 0, 1, 3, 1, 0, BORDER_CONSTANT);
    imshow("Horizontal Sobel Operator Image Result [OpenCV]", openCVSobelHImage);

    auto diffSobelHFilter = Mat1f();
    absdiff(fftSobelHImage, openCVSobelHImage, diffSobelHFilter);
    imshow("Horizontal Sobel Operator Image Difference", diffSobelHFilter);

    while (waitKey() != 27);

    // б) Вертикальный оператор Собеля
    fft_sobelV.setImageShift(Point2i(KERNEL_COLS / 2, KERNEL_ROWS / 2));
    fft_sobelV.inverseFastFurierTransform();
    fft_sobelV.showSpectrum("Vertical Sobel Operator Spectrum Result");
    fft_sobelV.showImage("Vertical Sobel Operator Image Result");

    while (waitKey() != 27);

    // Сравнение вертикального оператора Собеля со встроенным фильтром
    auto fftSobelVImage = fft_sobelV.getImage();
    auto openCVSobelVImage = Mat1f();
    Sobel(image_3, openCVSobelVImage, CV_32FC1, 1, 0, 3, 1, 0, BORDER_CONSTANT);
    imshow("Vertical Sobel Operator Image Result [OpenCV]", openCVSobelVImage);

    auto diffSobelVFilter = Mat1f();
    absdiff(fftSobelVImage, openCVSobelVImage, diffSobelVFilter);
    imshow("Vertical Sobel Operator Image Difference", diffSobelVFilter);

    while (waitKey() != 27);

    // в) Свёртка горизонтального и вертикального операторов Собеля
    fft_sobel.setImageShift(Point2i(2 * (KERNEL_COLS / 2), 2 * (KERNEL_ROWS / 2)));
    fft_sobel.inverseFastFurierTransform();
    fft_sobel.showSpectrum("Sobel Filter Spectrum Result");
    fft_sobel.showImage("Sobel Filter Image Result");

    while (waitKey() != 27);

    // Сравнение свёртки операторов Собеля со встроенным фильтром
    auto fftSobelImage = fft_sobel.getImage();
    auto openCVSobelImage = Mat1f();
    Sobel(image_3, openCVSobelImage, CV_32FC1, 1, 1, 3, 1, 0, BORDER_CONSTANT);
    imshow("Sobel Filter Image Result [OpenCV]", openCVSobelImage);

    auto diffSobelFilter = Mat1f();
    absdiff(fftSobelImage, openCVSobelImage, diffSobelFilter);
    imshow("Sobel Filter Image Difference", diffSobelFilter);

    while (waitKey() != 27);

    // 2) Усредняющий фильтр (Box Filter)
    fft_box.setImageShift(Point2i(KERNEL_COLS / 2, KERNEL_ROWS / 2));
    fft_box.inverseFastFurierTransform();
    fft_box.showSpectrum("Box Filter Spectrum Result");
    fft_box.showImage("Box Filter Image Result");

    while (waitKey() != 27);

    // Сравнение усредняющего фильтра со встроенным фильтром
    auto fftBoxImage = fft_box.getImage();
    auto openCVBoxImage = Mat1f();
    boxFilter(image_3, openCVBoxImage, CV_32FC1, Size(3, 3), Point(-1, -1), true, BORDER_CONSTANT);
    imshow("Box Filter Image Result [OpenCV]", openCVBoxImage);

    auto diffBoxFilter = Mat1f();
    absdiff(fftBoxImage, openCVBoxImage, diffBoxFilter);
    imshow("Box Filter Image Difference", diffBoxFilter);

    while (waitKey() != 27);

    // 3) Фильтр Лапласа
    fft_laplace.setImageShift(Point2i(KERNEL_COLS / 2, KERNEL_ROWS / 2));
    fft_laplace.inverseFastFurierTransform();
    fft_laplace.showSpectrum("Laplace Filter Spectrum Result");
    fft_laplace.showImage("Laplace Filter Image Result");

    while (waitKey() != 27);

    // Сравнение фильтра Лапласа со встроенным фильтром
    auto fftLaplaceImage = fft_laplace.getImage();
    auto openCVLaplaceImage = Mat1f();
    Laplacian(image_3, openCVLaplaceImage, CV_32FC1, 1, 1, 0, BORDER_CONSTANT);
    imshow("Laplace Filter Image Result [OpenCV]", openCVLaplaceImage);

    auto diffLaplaceFilter = Mat1f();
    absdiff(fftLaplaceImage, openCVLaplaceImage, diffLaplaceFilter);
    imshow("Laplace Filter Difference", diffLaplaceFilter);

    while (waitKey() != 27);

    /********************** Конец задания 4 **********************/

    /************* Задание 5. Корреляция изображений *************/

    Mat1f image_5 = imread("src/images/car_number.bmp", IMREAD_GRAYSCALE);
    image_5 /= static_cast<float>(0xFF);

    // Корреляция с символом '6'
    Mat1f symbol_1 = imread("src/images/number_six.bmp", IMREAD_GRAYSCALE);
    symbol_1 /= static_cast<float>(0xFF);

    cols = getOptimalDFTSize(image_5.cols + symbol_1.cols - 1);
    rows = getOptimalDFTSize(image_5.rows + symbol_1.rows - 1);

    auto fft_carNumber_1 = FastFurierTransformer();
    fft_carNumber_1.setImage(image_5);
    fft_carNumber_1.setSpectrumSize(Size2i(cols, rows));
    fft_carNumber_1.directFastFurierTransform();

    auto fft_symbol_1 = FastFurierTransformer();
    fft_symbol_1.setImage(symbol_1);
    fft_symbol_1.setSpectrumSize(Size2i(cols, rows));
    fft_symbol_1.directFastFurierTransform();

    fft_carNumber_1.showImage("Car Number");
    fft_symbol_1.showImage("Symbol '6' Image");
    fft_symbol_1.showSpectrum("Symbol '6' Spectrum");

    while (waitKey() != 27);

    auto carNumberSpectrum_1 = fft_carNumber_1.getSpectrum();
    auto symbolSpectrum_1 = fft_symbol_1.getSpectrum();
    multiplySpectrums(carNumberSpectrum_1, symbolSpectrum_1, carNumberSpectrum_1, true);

    fft_carNumber_1.setSpectrum(carNumberSpectrum_1);
    fft_carNumber_1.inverseFastFurierTransform();

    auto carNumberImage_1 = fft_carNumber_1.getImage();
    threshold(carNumberImage_1, carNumberImage_1, 0.95, 1.0, THRESH_BINARY);
    fft_carNumber_1.setImage(carNumberImage_1);

    fft_carNumber_1.showSpectrum("Correlation with Symbol '6' Spectrum");
    fft_carNumber_1.showImage("Correlation with Symbol '6' Image");

    while (waitKey() != 27);

    // Корреляция с символом '9'
    Mat1f symbol_2 = imread("src/images/number_nine.bmp", IMREAD_GRAYSCALE);
    symbol_2 /= static_cast<float>(0xFF);

    cols = getOptimalDFTSize(image_5.cols + symbol_2.cols - 1);
    rows = getOptimalDFTSize(image_5.rows + symbol_2.rows - 1);

    auto fft_carNumber_2 = FastFurierTransformer();
    fft_carNumber_2.setImage(image_5);
    fft_carNumber_2.setSpectrumSize(Size2i(cols, rows));
    fft_carNumber_2.directFastFurierTransform();

    auto fft_symbol_2 = FastFurierTransformer();
    fft_symbol_2.setImage(symbol_2);
    fft_symbol_2.setSpectrumSize(Size2i(cols, rows));
    fft_symbol_2.directFastFurierTransform();

    fft_carNumber_2.showImage("Car Number");
    fft_symbol_2.showImage("Symbol '9' Image");
    fft_symbol_2.showSpectrum("Symbol '9' Spectrum");

    while (waitKey() != 27);

    auto carNumberSpectrum_2 = fft_carNumber_2.getSpectrum();
    auto symbolSpectrum_2 = fft_symbol_2.getSpectrum();
    multiplySpectrums(carNumberSpectrum_2, symbolSpectrum_2, carNumberSpectrum_2, true);

    fft_carNumber_2.setSpectrum(carNumberSpectrum_2);
    fft_carNumber_2.inverseFastFurierTransform();

    auto carNumberImage_2 = fft_carNumber_2.getImage();
    threshold(carNumberImage_2, carNumberImage_2, 0.95, 1.0, THRESH_BINARY);
    fft_carNumber_2.setImage(carNumberImage_2);

    fft_carNumber_2.showSpectrum("Correlation with Symbol '9' Spectrum");
    fft_carNumber_2.showImage("Correlation with Symbol '9' Image");

    while (waitKey() != 27);

    // Корреляция с символом 'O'
    Mat1f symbol_3 = imread("src/images/letter_o.bmp", IMREAD_GRAYSCALE);
    symbol_3 /= static_cast<float>(0xFF);

    cols = getOptimalDFTSize(image_5.cols + symbol_3.cols - 1);
    rows = getOptimalDFTSize(image_5.rows + symbol_3.rows - 1);

    auto fft_carNumber_3 = FastFurierTransformer();
    fft_carNumber_3.setImage(image_5);
    fft_carNumber_3.setSpectrumSize(Size2i(cols, rows));
    fft_carNumber_3.directFastFurierTransform();

    auto fft_symbol_3 = FastFurierTransformer();
    fft_symbol_3.setImage(symbol_3);
    fft_symbol_3.setSpectrumSize(Size2i(cols, rows));
    fft_symbol_3.directFastFurierTransform();

    fft_carNumber_3.showImage("Car Number");
    fft_symbol_3.showImage("Symbol 'O' Image");
    fft_symbol_3.showSpectrum("Symbol 'O' Spectrum");

    while (waitKey() != 27);

    auto carNumberSpectrum_3 = fft_carNumber_3.getSpectrum();
    auto symbolSpectrum_3 = fft_symbol_3.getSpectrum();
    multiplySpectrums(carNumberSpectrum_3, symbolSpectrum_3, carNumberSpectrum_3, true);

    fft_carNumber_3.setSpectrum(carNumberSpectrum_3);
    fft_carNumber_3.inverseFastFurierTransform();

    auto carNumberImage_3 = fft_carNumber_3.getImage();
    threshold(carNumberImage_3, carNumberImage_3, 0.95, 1.0, THRESH_BINARY);
    fft_carNumber_3.setImage(carNumberImage_3);

    fft_carNumber_3.showSpectrum("Correlation with Symbol 'O' Spectrum");
    fft_carNumber_3.showImage("Correlation with Symbol 'O' Image");

    while (waitKey() != 27);

    // Корреляция с символом 'E'
    Mat1f symbol_4 = imread("src/images/letter_e.bmp", IMREAD_GRAYSCALE);
    symbol_4 /= static_cast<float>(0xFF);

    cols = getOptimalDFTSize(image_5.cols + symbol_4.cols - 1);
    rows = getOptimalDFTSize(image_5.rows + symbol_4.rows - 1);

    auto fft_carNumber_4 = FastFurierTransformer();
    fft_carNumber_4.setImage(image_5);
    fft_carNumber_4.setSpectrumSize(Size2i(cols, rows));
    fft_carNumber_4.directFastFurierTransform();

    auto fft_symbol_4 = FastFurierTransformer();
    fft_symbol_4.setImage(symbol_4);
    fft_symbol_4.setSpectrumSize(Size2i(cols, rows));
    fft_symbol_4.directFastFurierTransform();

    fft_carNumber_4.showImage("Car Number");
    fft_symbol_4.showImage("Symbol 'E' Image");
    fft_symbol_4.showSpectrum("Symbol 'E' Spectrum");

    while (waitKey() != 27);

    auto carNumberSpectrum_4 = fft_carNumber_4.getSpectrum();
    auto symbolSpectrum_4 = fft_symbol_4.getSpectrum();
    multiplySpectrums(carNumberSpectrum_4, symbolSpectrum_4, carNumberSpectrum_4, true);

    fft_carNumber_4.setSpectrum(carNumberSpectrum_4);
    fft_carNumber_4.inverseFastFurierTransform();

    auto carNumberImage_4 = fft_carNumber_4.getImage();
    threshold(carNumberImage_4, carNumberImage_4, 0.95, 1.0, THRESH_BINARY);
    fft_carNumber_4.setImage(carNumberImage_4);

    fft_carNumber_4.showSpectrum("Correlation with Symbol 'E' Spectrum");
    fft_carNumber_4.showImage("Correlation with Symbol 'E' Image");

    while (waitKey() != 27);

    /********************** Конец задания 5 **********************/

    return 0;
}
