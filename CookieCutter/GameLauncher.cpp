#include "GameLauncher.h"
#include "ShapeDetector.h"

using namespace std;

//timer used for the "Getting ready" phase
void GameLauncher::timerFunction() {
	gettingReady = true;
	for (int i = 5; i > 0; --i)
	{
		this_thread::sleep_for(1s);
		countdown = to_string(i);
	}
	countdown = "Ready";
	this_thread::sleep_for(1s);
	gettingReady = false;
}

GameLauncher::GameLauncher(Mat cookie, int countdown)
{
	this->cookie = cookie;
	this->countdown = to_string(countdown);
}

void GameLauncher::launch(Painter painter)
{
	ShapeDetector sd(cookie);
	VideoCapture cap(0);

	getReady(painter, sd, cap);
	play(painter, sd, cap);

	std::this_thread::sleep_for(3s);
	destroyAllWindows();

	Mat finalScreen(Size(512, 512), CV_8UC3, Scalar(255, 255, 255));

	if (sd.checkShape()) {
		putText(finalScreen, "Congratulations!", Point(125, 255), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 2);
		putText(finalScreen, "You can live.", Point(157, 290), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 2);

		imshow("You won!", finalScreen);
		waitKey(0);
	}
	else {
		putText(finalScreen, "Eliminated", Point(180, 255), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
		putText(finalScreen, "Rest in peace", Point(150, 290), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);

		imshow("Eliminated", finalScreen);
		waitKey(0);
	}
}

void GameLauncher::getReady(Painter painter, ShapeDetector sd, VideoCapture cap)
{
	Mat frame;

	Mat cookieCopy = cookie;

	thread timerThread([this] { this->timerFunction(); });
	while (gettingReady) {
		cap.read(frame);

		painter.detectColor(frame);
		if (gettingReady)
			painter.drawOnCanvas(cookieCopy, sd, gettingReady);

		putText(frame, countdown, Point(150, 150), FONT_HERSHEY_PLAIN, 7, Scalar(255, 0, 0), 5);
		imshow("Video", frame);
		imshow("Get Ready", cookieCopy);
		waitKey(1);
	}
	destroyAllWindows();
	timerThread.join();

	painter.refresh(); //to remove all the points that were made while getting ready
}

void GameLauncher::play(Painter painter, ShapeDetector sd, VideoCapture cap)
{
	Mat frame;

	while (!painter.getDead() && !sd.checkShape()) {
		cap.read(frame);

		painter.detectColor(frame);
		painter.drawOnCanvas(cookie, sd, gettingReady);

		imshow("Video", frame);
		imshow("Play", cookie);
		waitKey(1);
	}
}
