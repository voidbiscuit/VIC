#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;

class ImageObject {
public:
	// Constructor
	ImageObject(string name);
	// Functions
	void AddImage(cv::Mat image);
	char Display(string window_name);
	char LoopDisplay(string window_name);
	double Likeness(cv::Mat image);
	// Data
	string name;
private:
	char ESCAPE_KEY = 27;
	int time = 20;
	vector<cv::Mat> objectFiles;
};