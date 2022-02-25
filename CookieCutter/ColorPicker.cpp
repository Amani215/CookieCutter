#include "ColorPicker.h"
#include <iostream>

using namespace std;

void ColorPicker::setColor()
{
	Mat frame, hsv, mask;
	VideoCapture cap(0);

	namedWindow("Trackbars", (640, 200));
	moveWindow("Trackbars", 60, 60);
	createTrackbar("hue min", "Trackbars", &hueMin, 179);
	createTrackbar("hue max", "Trackbars", &hueMax, 179);

	createTrackbar("sat min", "Trackbars", &satMin, 255);
	createTrackbar("sat max", "Trackbars", &satMax, 255);

	createTrackbar("val min", "Trackbars", &valMin, 255);
	createTrackbar("val max", "Trackbars", &valMax, 255);

	while (getWindowProperty("Trackbars", WND_PROP_VISIBLE) >= 1) {
		cap.read(frame);
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Scalar lower(hueMin, satMin, valMin);
		Scalar upper(hueMax, satMax, valMax);
		inRange(hsv, lower, upper, mask);

		putText(mask, "Close Trackbars once done choosing color!", Point(70, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0));
		//imshow("Video", frame);
		//moveWindow("Video", 645, 50);

		imshow("Result", mask);
		moveWindow("Result", 500, 60);

		waitKey(1);
	}

	cout << hueMin << "," << hueMax << "," << satMin << "," << satMax << "," << valMin << "," << valMax << endl;

	destroyAllWindows();
}

int ColorPicker::getHueMin() { return hueMin; }
int ColorPicker::getHueMax() { return hueMax; }
int ColorPicker::getSatMin() { return satMin; }
int ColorPicker::getSatMax() { return satMax; }
int ColorPicker::getValMin() { return valMin; }
int ColorPicker::getValMax() { return valMax; }