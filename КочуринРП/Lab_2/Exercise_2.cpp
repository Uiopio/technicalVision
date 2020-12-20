
#include "Otsu_method/Otsu_method.h"

int main()
{
    //Mat image1 = imread("D:/Учёба/ВУЗ/7 семестр/Техническое зрение/Lab/Exercise_2/What.bmp");
    //OtsuMethod(image1);
    //Mat image2 = imread("D:/Учёба/ВУЗ/7 семестр/Техническое зрение/Lab/Exercise_2/Mount1.bmp");
    //OtsuMethod(image2);
    //Mat image3 = imread("D:/Учёба/ВУЗ/7 семестр/Техническое зрение/Lab/Exercise_2/Mount2.bmp");
    //OtsuMethod(image3);
    //Mat image4 = imread("D:/Учёба/ВУЗ/7 семестр/Техническое зрение/Lab/Exercise_2/fkf.bmp");
    //OtsuMethod(image4);
    Mat image3 = imread("D:/Учёба/ВУЗ/7 семестр/Техническое зрение/Lab/Exercise_2/Mount2.bmp");
    Comparision(image3);
    return 0;
}
