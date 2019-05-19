#pragma once
// Includes
#include "../FreenectPlaybackWrapper/FreenectPlaybackWrapper.h"
#include <string>

// Namespace
using namespace std;


class FreenectExampleModded {

private:
	// Images
	string _image_path = "";
	class ImageObject {
	public:
		string name;
		int id;
		vector<cv::Mat> images;
		ImageObject(string name, int id) { this->name = name, this->id = id; }
		void add_image(cv::Mat image) { this->images.push_back(image); }
	};
	vector<ImageObject> _image_objects;
	int _image_object_count = 20,
		_image_object_images = 72;

	// Support Vector Machine
	cv::Ptr<cv::ml::SVM> _support_vector_machine;

	// Keys
	char _key = -1;

	// Window
	const int _flags = cv::WindowFlags::WINDOW_AUTOSIZE | cv::WindowFlags::WINDOW_GUI_EXPANDED;
	const string
		_Instructions = " press <A D> to switch frame",
		_RGB = "RGB" + _Instructions,
		_Depth_Raw = "Depth Raw" + _Instructions,
		_Depth = "Depth" + _Instructions,
		_Guess = "Guess";

	// Values
	// Images
	
	

	// Videos
	string _video_path = "";
	struct frame { cv::Mat rgb, depth_raw, depth; };
	int current_frame_id = 0;
	frame current_frame;
	int current_guess = 0,
		current_guess_rotation = 0;
	bool frame_switched = true;
	vector<frame> stream;

public:
	FreenectExampleModded(string image_path, string video_path);
	void Start();

private:
	void DecorateFrame(frame frame, int frame_id, int frame_count);
	void DecorateVideo(vector<frame> stream);
	void LoadVideo();
	void ViewVideo();
	void LoadImages();
	void LearnImages();
};