#pragma once

#include "ImageObject/ImageObject.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>


#define cls(); system("cls");

using namespace std;

class VisuII {
public:
	// Constructor
	VisuII(string filepath, string identifier, string splitter, int count);
	// Functions
	void Start();
	void SetPath(string filepath);
	void LoadObject(string identifier);
	void DisplayObject(string identifier);
	void LoadObjects();
	void DisplayObjects();
	void RecognEyes();
	int MostLikely(cv::Mat image);

private:
	// Keys
	static const char
		ESCAPE = 27,
		LEFT = 97,
		RIGHT = 100;
	// Control
	int timer = 5;
	string _VisuII = "VisUII | <A D> Press Escape to Quit";
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