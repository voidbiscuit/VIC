#pragma once
// Includes
#include "../FreenectPlaybackWrapper/FreenectPlaybackWrapper.h"
#include <string>

// Namespace
using namespace std;


class FreenectExampleModded {

public:
	FreenectExampleModded(string image_path, string video_path);
	void Start();

private:
	void DecorateVideo(vector<cv::Mat> video);
	void LoadVideo();
	void ViewVideo();
	void LoadImages();
	void LearnImages();

private:
	// Image Object
	class ImageObject {
	public:
		string name;
		int id;
		vector<cv::Mat> images;
		ImageObject(string name, int id) { this->name = name, this->id = id; }
		void add_image(cv::Mat image) { this->images.push_back(image); }
	};
	vector<ImageObject> image_objects;
	// Support Vector Machine
	cv::Ptr<cv::ml::SVM> support_vector_machine;


	// Keys
	char key = -1;
	int current_image = 0;

	// Window
	const int flags = cv::WindowFlags::WINDOW_AUTOSIZE | cv::WindowFlags::WINDOW_GUI_EXPANDED;
	const string _RGB = "RGB press <A D> to switch frame", _Depth = "Depth press <A D> to switch frame", _Guess = "Guess";

	// Hardcoded Strings
	string image_path = "";
	string video_path = "";

	// Videos
	vector<cv::Mat> video_rgb, video_depth;
	cv::Mat frame_rgb, frame_depth;
};