#include "ImageObject.h"

using namespace std;


ImageObject::ImageObject(string name) {
	this->name = name;
}


void ImageObject::AddImage(cv::Mat image) {
	this->objectFiles.push_back(image);
}

char ImageObject::Display() {
	char key = -1;
	for (cv::Mat frame : this->objectFiles)
	{
		imshow("VisualEyes", frame);
		key = cv::waitKey(this->time);
		if (key != -1) break;
	}
	return key;
}

char ImageObject::LoopDisplay() {
	char key = -1;
	while (key == -1)
		key = Display();
	return key;
}


double ImageObject::Likeness(cv::Mat image) {
	return 0;
}