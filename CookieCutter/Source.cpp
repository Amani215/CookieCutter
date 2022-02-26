//TODO
// adding default colours
// aesthetics
// readme

#ifndef UNICODE
#define UNICODE
#endif 

#define OK_MESSAGE 1

#include <windows.h>

#include "ColorPicker.h"
#include "Painter.h"
#include "ShapeDetector.h"

ColorPicker cp;
bool gettingReady = true;
String currentTime = "5";	//for showing the countdown while getting ready

vector<String> cookies = { "Resources/heartCookie.png",
						   "Resources/circleCookie.png", 
						   "Resources/starCookie.png", 
						   "Resources/squareCookie.png",
						   "Resources/triangleCookie.png" };

void timerFunction() {
	gettingReady = true;
	for (int i = 5; i > 0; --i)
	{
		std::this_thread::sleep_for(1s);
		currentTime = to_string(i);
	}
	currentTime = "Ready";
	std::this_thread::sleep_for(1s);
	gettingReady = false;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void createInitialScreen(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 730, 350,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	cp.setColor();

	Painter painter;
	painter.setColorToDetect({ cp.getHueMin(),cp.getHueMax(),cp.getSatMin(),cp.getSatMax(),cp.getValMin(),cp.getValMax() });

	VideoCapture cap(0);
	Mat frame;
	
	srand(time(0));
	String cookieLink = cookies[rand() % cookies.size()]; //choose a random shape
	Mat cookie = imread(cookieLink);
	Mat cookieCopy = imread(cookieLink);
	ShapeDetector sd(cookie); //the contours are the same so we use the same shape detector

	thread timerThread(timerFunction);
	while (gettingReady) {
		cap.read(frame);

		painter.detectColor(frame);
		if(gettingReady)
			painter.drawOnCanvas(cookieCopy, sd, gettingReady);

		putText(frame, currentTime, Point(150, 150), FONT_HERSHEY_PLAIN, 7, Scalar(255, 0, 0), 5);
		imshow("Video", frame);
		imshow("Get Ready", cookieCopy);
		waitKey(1);
	}
	destroyAllWindows();
	timerThread.join();

	painter.refresh(); //to remove all the points that were made while getting ready

	while (!painter.getDead() && !sd.checkShape()) {
		cap.read(frame);

		painter.detectColor(frame);
		painter.drawOnCanvas(cookie, sd, gettingReady);

		imshow("Video", frame);
		imshow("Play", cookie);
		waitKey(1);
	}

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

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		createInitialScreen(hwnd);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case OK_MESSAGE:
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			break;
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void createInitialScreen(HWND hwnd) {
	LPCWSTR instructions = TEXT("HOW TO PLAY\n\n"
		"1. Get a pen with a colored tip. Make sure you're in a well lit room.\n\n"
		"2. In the next screen keep moving the trackbars until the color of your pen appears as white.\n"
		"Make sure that the max is always higher than the min!\n\n"
		"3. Once you're done choosing the color, close the trackbars window and start playing.\n\n"
		"Have fun and good luck!");

	HWND hText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), instructions,
		WS_CHILD | WS_VISIBLE,
		30, 30, 650, 200,
		hwnd, HMENU(NULL), NULL, NULL);

	CreateWindowW(L"Button", L"OK",
		WS_VISIBLE | WS_CHILD,
		300, 250, 80, 30,
		hwnd, (HMENU)OK_MESSAGE, NULL, NULL);
}