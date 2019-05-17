#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

#include "ImageObject/ImageObject.h"

using namespace std;

class VisualEyes {
public:
	// Constructor
	VisualEyes(string filepath, string identifier, string splitter, int count);
	// Functions
	void Clear();
	void Splash();
	void Start();
	void SetPath(string filepath);
	void LoadObject(string identifier);
	void DisplayObject(string identifier);
	void LoadObjects();
	void DisplayObjects();
	void RecognEyes();
	int MostLikely(cv::Mat image);

private:
	// Text
	string
		splash = \
		"\n                              "\
		"\n    .---------------------.   "\
		"\n    | Starting VisualEyes |   "\
		"\n    '---------------------'   "\
		"\n                              "\
		"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",
		menu = \
		"\n                              "\
		"\n (())   Visual Eyes   (())    "\
		"\n        ___________           "\
		"\n                              "\
		"\n  S - Set Path                "\
		"\n  L - Load Objects            "\
		"\n  D - Display Objects         "\
		"\n  R - Recognise               "\
		"\n                              "\
		"\n  Q - Quit                    "\
		"\n                              "\
		"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	// Keys
	static const char
		ESCAPE = 27,
		LEFT = 97,
		RIGHT = 100;
	// Control
	int timer = 5;
	string _VisualEyes = "VisualEyes";
	// Data
	string default_filepath = "";
	string filepath = "";
	string identifier = "";
	string splitter = "";
	int count = 0;
	vector<ImageObject> imageObjects;
	// CV
	int window_dimension[2] = { 600,600 };
	char key = ' ';
	// WebCam
	cv::VideoCapture web_cam;
	cv::Mat frame;
};