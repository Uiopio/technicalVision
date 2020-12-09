# 4 лаба СТЗ
import cv2
import numpy as np
import math


def nothing(x):
    pass


# ищет ближайшего робота к лампе
def findNearestRobot(centres, lamp_centre):
    dst_min = math.sqrt((centres[0][0] - lamp_centre[0])**2 + (centres[0][1] - lamp_centre[1])**2 )
    near_center = centres[0]
    for cntr in centres:
        dst = math.sqrt((cntr[0]-lamp_centre[0])**2 + (cntr[1]-lamp_centre[1])**2)
        if dst < dst_min:
            dst_min = dst
            near_center = cntr
    return near_center


# находит все объекты в 2х цветовых диапозонах, совмещает их, рисует контуры и центр
def findColorAndDrow2Diaposons(image, hsv_min, hsv_max, hsv_min2, hsv_max2, counter_BGR):
    # +close lamp
    image_copy = image.copy()
    cv2.ellipse(image_copy, (632, 218), (22, 40), 85, 0, 360, (127, 127, 127), -1)
    cv2.imshow("original + closed lamp", image_copy)

    image_hsv = cv2.cvtColor(image_copy, cv2.COLOR_BGR2HSV)
    image_bin1 = cv2.inRange(image_hsv, hsv_min, hsv_max)
    image_bin2 = cv2.inRange(image_hsv, hsv_min2, hsv_max2)
    image_bin = image_bin1 + image_bin2

    # cv2.imshow("founded areas without opening", image_bin)
    kernel = np.ones((5, 5), 'uint8')
    image_bin = cv2.erode(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)
    image_bin = cv2.erode(image_bin, kernel)
    # cv2.imshow("finded areas", image_bin)

    counters, _ = cv2.findContours(image_bin.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    print("num of counter = " + str(len(counters)))

    centers = []
    # loop over the contours
    for c in counters:
        # compute the center of the contour
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            centers.append((cX, cY))
            # draw the contour and center of the shape on the image
            cv2.drawContours(image, [c], -1, counter_BGR, 2)
            cv2.circle(image, (cX, cY), 3, (0, 0, 0), -1)

    return image, centers


# находит все объекты в цветовом диапозонах, рисует контуры и центр
def findColorAndDrow(image, hsv_min, hsv_max, counter_BGR):
    image_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    image_bin = cv2.inRange(image_hsv, hsv_min, hsv_max)

    # cv2.imshow("finded areas without opening", image_bin)
    kernel = np.ones((5, 5), 'uint8')
    # open+close filters
    image_bin = cv2.erode(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)
    image_bin = cv2.erode(image_bin, kernel)

    # cv2.imshow("finded areas", image_bin)

    counters, _ = cv2.findContours(image_bin.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    # print("num of counter = " + str(len(counters)))
    centers = []
    # loop over the contours
    for c in counters:
        # compute the center of the contour
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            centers.append((cX, cY))
            # draw the contour and center of the shape on the image
            cv2.drawContours(image, [c], -1, counter_BGR, 2)
            cv2.circle(image, (cX, cY), 3, (0, 0, 0), -1)

    return image, centers


# 1 часть - белые домики
def part1(path):
    image = cv2.imread(path)
    cv2.imshow("original", image)
    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image_bin = cv2.threshold(image_gray, 210, 255, cv2.THRESH_BINARY)[1]

    kernel = np.ones((3, 3), 'uint8')
    image_bin = cv2.erode(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)

    counters, hiearchy = cv2.findContours(image_bin.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    print("num of houses = " + str(len(counters)))
    # loop over the contours
    for c in counters:
        # compute the center of the contour
        M = cv2.moments(c)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        # draw the contour and center of the shape on the image
        cv2.drawContours(image, [c], -1, (0, 255, 0), 2)
        cv2.circle(image, (cX, cY), 4, (0, 0, 255), -1)

    cv2.imshow("bin", image_bin)
    cv2.imshow("original + center", image)

    while cv2.waitKey(10) != 27:
        nothing(0)
    cv2.destroyAllWindows()


# 2 часть - моторные отделения в тепловизоре
def part2(path):
    image = cv2.imread(path)
    cv2.imshow("original", image)
    cv2.createTrackbar("hsv max", "original", 10, 100, nothing)

    while cv2.waitKey(10) != 100:
        nothing(0)

    image_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    hsv_min = np.array((0, 0, 0), 'uint8')
    hsv_max = np.array((cv2.getTrackbarPos("hsv max", "original"), 255, 255), 'uint8')
    image_bin = cv2.inRange(image_hsv, hsv_min, hsv_max)

    kernel = np.ones((7, 7), 'uint8')
    image_bin = cv2.dilate(image_bin, kernel)
    image_bin = cv2.erode(image_bin, kernel)
    cv2.imshow("color diaposon", image_bin)

    counters, _ = cv2.findContours(image_bin.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    print("num of counter = " + str(len(counters)))
    image_copy = image.copy()
    # loop over the contours
    for c in counters:
        # compute the center of the contour
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            # draw the contour and center of the shape on the image
            cv2.circle(image_copy, (cX, cY), 7, (0, 0, 0), -1)

    cv2.imshow("result", image_copy)

    while cv2.waitKey(5) != 27:
        key = cv2.waitKey(5)
        if key == 100:
            cv2.destroyAllWindows()
            part2(path)
            break
    cv2.destroyAllWindows()


# 3 часть - роботы и поиск ближайшего к лампе
def part3(path):
    image = cv2.imread(path)

    hsv_min_red = np.array((160, 80, 120), 'uint8')
    hsv_max_red = np.array((179, 255, 255), 'uint8')
    hsv_min_red2 = np.array((0, 80, 120), 'uint8')
    hsv_max_red2 = np.array((12, 255, 255), 'uint8')
    red_BGR = (0, 0, 255)

    hsv_min_green = np.array((45, 40, 130), 'uint8')
    hsv_max_green = np.array((80, 255, 255), 'uint8')
    green_BGR = (0, 255, 0)

    hsv_min_blue = np.array((90, 50, 50), 'uint8')
    hsv_max_blue = np.array((130, 255, 255), 'uint8')
    blue_BGR = (255, 0, 0)

    hsv_min_yellow = np.array((0, 0, 250), 'uint8')
    hsv_max_yellow = np.array((179, 10, 255), 'uint8')
    yellow_BGR = (0, 255, 255)

    image, red_centres = findColorAndDrow2Diaposons(image, hsv_min_red, hsv_max_red, hsv_min_red2, hsv_max_red2, red_BGR)
    image, green_centres = findColorAndDrow(image, hsv_min_green, hsv_max_green, green_BGR)
    image, blue_centres = findColorAndDrow(image, hsv_min_blue, hsv_max_blue, blue_BGR)
    image, lamp_centres = findColorAndDrow(image, hsv_min_yellow, hsv_max_yellow, yellow_BGR)

    red_near_center = findNearestRobot(red_centres, lamp_centres[0])
    cv2.circle(image, red_near_center, 5, (255, 0, 0), -1)
    cv2.line(image, red_near_center, lamp_centres[0], (0,0,0), 1)
    green_near_center = findNearestRobot(green_centres, lamp_centres[0])
    cv2.circle(image, green_near_center, 5, (255, 0, 0), -1)
    cv2.line(image, green_near_center, lamp_centres[0], (0, 0, 0), 1)
    blue_near_center = findNearestRobot(blue_centres, lamp_centres[0])
    cv2.circle(image, blue_near_center, 5, (255, 0, 0), -1)
    cv2.line(image, blue_near_center, lamp_centres[0], (0, 0, 0), 1)

    cv2.imshow("orig + contours", image)

    while cv2.waitKey(5) != 27:
        nothing(0)
    cv2.destroyAllWindows()


# 4 часть - поиск бракованных ключей
def part4():
    image = cv2.imread("C:/lab4Pictures/Spanners/spanners.jpg")
    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image_bin = cv2.threshold(image_gray, 210, 255, cv2.THRESH_BINARY_INV)[1]

    template = cv2.imread("C:/lab4Pictures/Spanners/spanner_tmplt.jpg")
    template_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    template_bin = cv2.threshold(template_gray, 210, 255, cv2.THRESH_BINARY)[1]

    kernel = np.ones((3, 3), 'uint8')
    template_bin = cv2.erode(template_bin, kernel)
    template_bin = cv2.dilate(template_bin, kernel)

    image_bin = cv2.erode(image_bin, kernel)
    image_bin = cv2.dilate(image_bin, kernel)

    contour_template, _ = cv2.findContours(template_bin.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    print("num of counter = " + str(len(contour_template)))

    contours_image, _ = cv2.findContours(image_bin.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    print("num of counter on image = " + str(len(contours_image)))

    M = cv2.moments(contour_template[0])
    cntsX = int(M["m10"] / M["m00"])
    cntsY = int(M["m01"] / M["m00"])
    # draw the contour and center of the shape on the image
    cv2.drawContours(template, contour_template[0], -1, (0, 255, 0), 2)
    cv2.circle(template, (cntsX, cntsY), 4, (0, 0, 255), -1)

    for cntr_img in contours_image:
        sim = cv2.matchShapes(contour_template[0], cntr_img, 1, 0)
        print(str(sim))
        if sim < 1.0:
            cv2.drawContours(image, cntr_img, -1, (255, 0, 0), 2)
        else:
            cv2.drawContours(image, cntr_img, -1, (0, 0, 255), 2)

    cv2.putText(image, "correct spanner", (10, 40), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 0, 0), 2)
    cv2.putText(image, "defect spanner", (10, 70), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 2)
    cv2.imshow("template", template)
    cv2.imshow("Spanners", image)
    # cv2.imshow("Spanners bin + open", image_bin)

    while cv2.waitKey(10) != 27:
        nothing(0)
    cv2.destroyAllWindows()


# ============================== main ==============================
while True:
    # Esc - перелистывать (d во второй части)

    print("Enter the number of part 4 of the lab: \n")
    data = input()
    if data == "1":
        print("1 part starting")
        for num in range(3):
            part1("C:/lab4Pictures/Allah/ig_" + str(num) + ".jpg")

    elif data == "2":
        print("2 part starting")
        for num in range(5):
            print(str(num+1) + " / 5 image in 2 part")
            part2("C:/lab4Pictures/Thermal imaging/" + str(num) + ".jpg")

    elif data == "3":
        for num in [1, 2]:
            print("3 part starting")
            part3("C:/lab4Pictures/Robots/" + str(num) + ".jpg")
    elif data == "4":
        print("4 part starting")
        part4()
    else:
        print("wrong number, try again")

# ==================================================================
