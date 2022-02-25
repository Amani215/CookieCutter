#include "ShapeDetector.h"
#include <iostream>

ShapeDetector::ShapeDetector(Mat img)
{
    this->img = img;
    setContours();
}

void ShapeDetector::setContours()
{
    Mat imgHSV, mask;

    //detect the shape from its color
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    Scalar lower(shapeColor[0], shapeColor[2], shapeColor[4]);
    Scalar upper(shapeColor[1], shapeColor[3], shapeColor[5]);
    inRange(imgHSV, lower, upper, mask);

    //find the external and internal contour of the shape
    Mat maskDilate;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));
    dilate(mask, maskDilate, kernel);

    findContours(maskDilate, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
}

bool ShapeDetector::pointIsValid(Point point)
{
    //if the point is outside or inside both contours it's invalid
    if ((pointPolygonTest(contours[0], point, false) == -1 && pointPolygonTest(contours[1], point, false) == -1) ||
        (pointPolygonTest(contours[0], point, false) == 1 && pointPolygonTest(contours[1], point, false) == 1))
        return false;
    else
        return true;
}

bool ShapeDetector::shapeIsValid(Mat frame)
{
    Mat frameHSV, mask;

    //detect the green shape on the frame (green means the contour is correct)
    cvtColor(frame, frameHSV, COLOR_BGR2HSV);

    Scalar lower(120, 100, 100);
    Scalar upper(120, 100, 100);
    inRange(frameHSV, lower, upper, mask);

    //find the contour of the green shape
    Mat maskDilate;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));
    dilate(mask, maskDilate, kernel);

    vector<vector<Point>> greenContour;
    vector<Vec4i> greenHierarchy;
    findContours(maskDilate, greenContour, greenHierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if(greenContour.size()>0)
        //if the aread of the green contour is between the areas of the contours of the shape then it's probably correct
        if (((arcLength(greenContour[0], true) >= arcLength(contours[0], true)) && (arcLength(greenContour[0], true) <= arcLength(contours[1], true))) ||
            ((arcLength(greenContour[0], true) <= arcLength(contours[0], true)) && (arcLength(greenContour[0], true) >= arcLength(contours[1], true))))
            validShape = true;
    

    return validShape;
}

bool ShapeDetector::checkShape()
{
    return validShape;
}
