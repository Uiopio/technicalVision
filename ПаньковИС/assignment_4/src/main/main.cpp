#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "functions.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    /********** Задание 1. Поиск укрытий на изображениях *********/

    vector<String> imageNamesPart_1 =
    {
        "cover_1.jpg",
        "cover_2.jpg",
        "cover_3.jpg",
    };

    for (auto imageName : imageNamesPart_1)
    {
        auto image_1 = imread("src/images/part_1/" + imageName);
        if (image_1.empty() == true)
        {
            continue;
        }
        auto result_1 = processImage(Part::PART_1, image_1);

        imshow("Image", image_1);
        imshow("Result", result_1);

        while (waitKey() != 27);

        destroyAllWindows();
    }

    /********************** Конец задания 1 **********************/

    /******* Задание 2. Поиск уязвимых мест на изображениях ******/

    vector<String> imageNamesPart_2 =
    {
        "hsv.png",
        "attack_helicopter.jpg",
        "cruiser.png",
        "fighter.jpg",
        "transport_helicopter.jpg",
        "transport_plane.jpg",
    };

    for (auto imageName : imageNamesPart_2)
    {
        auto image_2 = imread("src/images/part_2/" + imageName);
        if (image_2.empty() == true)
        {
            continue;
        }
        auto result_2 = processImage(Part::PART_2, image_2);

        imshow("Image", image_2);
        imshow("Result", result_2);

        while (waitKey() != 27);

        destroyAllWindows();
    }

    /********************** Конец задания 2 **********************/

    /*********** Задание 3. Определение команд роботов ***********/

    vector<String> imageNamesPart_3 =
    {
        "robots_1.jpg",
        "robots_2.jpg"
    };

    for (auto imageName : imageNamesPart_3)
    {
        auto image_3 = imread("src/images/part_3/" + imageName);
        if (image_3.empty() == true)
        {
            continue;
        }
        auto result_3 = processImage(Part::PART_3, image_3);

        imshow("Image", image_3);
        imshow("Result", result_3);

        while (waitKey() != 27);
    }

    /********************** Конец задания 3 **********************/

    /**************** Задание 4. Поиск по шаблону ****************/

    auto image_4 = imread("src/images/part_4/keys.jpg");
    auto imageTemplate = imread("src/images/part_4/key_template.jpg");
    auto result_4 = processImage(Part::PART_4, image_4, imageTemplate);

    imshow("Image", image_4);
    imshow("Result", result_4);

    while (waitKey() != 27);

    /********************** Конец задания 4 **********************/
}
