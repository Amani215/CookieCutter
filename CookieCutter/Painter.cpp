#include "Painter.h"

void Painter::detectColor(Mat img) {
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	//imshow("Video HSV", imgHSV);
	Scalar lower(colorToDetect[0], colorToDetect[2], colorToDetect[4]);
	Scalar upper(colorToDetect[1], colorToDetect[3], colorToDetect[5]);
	inRange(imgHSV, lower, upper, mask);

	Point myPoint = getTipOfPen(mask);
	if (myPoint.x != 0 && myPoint.y != 0)
		newPoints.push_back({ myPoint.x,myPoint.y});
}

Point Painter::getTipOfPen(Mat img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	Mat imgDilate;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));
	dilate(img, imgDilate, kernel);

	findContours(imgDilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> contourPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);
	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		if (area > 500) {
			float param = arcLength(contours[i], true);
			approxPolyDP(contours[i], contourPoly[i], 0.02 * param, true);
			//drawContours(img, contourPoly, i, Scalar(0, 0, 255), 2);

			boundRect[i] = boundingRect(contourPoly[i]);

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;
		}
	}

	return myPoint;
}

void Painter::drawOnCanvas(Mat img, ShapeDetector sd, bool gettingReady) {
	if(gettingReady)
		for (int i = 0; i < newPoints.size(); i++) {
			Point p(newPoints[i][0], newPoints[i][1]);
		

			if (!sd.pointIsValid(p)) {
				circle(img, p, 5, Scalar(0,0,255), FILLED);
			
			}else
				circle(img, p, 5, Scalar(0,255,0), FILLED);
		}
	else
		for (int i = 0; i < newPoints.size(); i++) {
			Point p(newPoints[i][0], newPoints[i][1]);


			if (!sd.pointIsValid(p)) {
				putText(img, "Dead", Point(210, 50), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
				circle(img, p, 5, Scalar(0, 0, 255), FILLED);
				dead = true;
				return;
			}
			else {
				circle(img, p, 5, Scalar(0, 255, 0), FILLED);
				sd.shapeIsValid(img);
			}
		}
}

void Painter::setColorToDetect(vector<int> colorToDetect) { this->colorToDetect = colorToDetect; }

void Painter::refresh()
{
	newPoints.clear();
}

bool Painter::getDead()
{
	return dead;
}
