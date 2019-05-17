
#include "FreenectExampleModded.h"

using namespace cv;
using namespace std;

FreenectExampleModded::FreenectExampleModded(string image_path, string video_path) {
	this->image_path = image_path;
	this->video_path = video_path;
}

void FreenectExampleModded::LoadImages() {
	// Remove Image Objects
	image_objects.clear();
	for (int object_id = 1; object_id <= 20; object_id++) {
		// Make new Image Object
		ImageObject image_object = ImageObject(to_string(object_id), object_id);
		// Load in Images
		for (int angle = 0; angle < 72; angle++) {
			string filepath = this->image_path + "obj" + to_string(object_id) + "__" + to_string(angle) + ".png";
			Mat image = cv::imread(filepath);
			Mat greyscale;
			cv::cvtColor(image, greyscale, cv::COLOR_BGR2GRAY);
			image_object.add_image(greyscale);
		}
		// Add Image Object to Array
		image_objects.push_back(image_object);
	}
}

void FreenectExampleModded::LearnImages() {
	// Init SVM
	this->support_vector_machine = cv::ml::SVM::create();
	// Set Properties
	this->support_vector_machine->setType(cv::ml::SVM::C_SVC);
	this->support_vector_machine->setKernel(cv::ml::SVM::LINEAR);
	this->support_vector_machine->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER,2, 0.00001));
	// Train Data Size
	cv::Mat training_data(20 * 72, 128 * 128, CV_32FC1);
	cv::Mat training_labels(20 * 72, 1, CV_32SC1);
	// For Each Image Object
	for (int image_object_id = 0; image_object_id < image_objects.size(); image_object_id++) {
		ImageObject image_object = this->image_objects[image_object_id];
		// For Each Image
		for (int image_id = 0; image_id < this->image_objects[image_object_id].images.size(); image_id++) {
			// Get and show image
			cv::Mat image = image_object.images[image_id].clone().reshape(0, 1);
			// Training Set ID
			int training_set_id = (image_object_id * 72) + image_id;
			// Set Training Label
			training_labels.at<int>(training_set_id) = image_object_id;
			// For each Pixel
			for (int pixel_id = 0; pixel_id < image.cols; pixel_id++)
				training_data.at<float>(training_set_id, pixel_id) = (float)image.at<uchar>(pixel_id);
		}
	}
	this->support_vector_machine->train(training_data, cv::ml::ROW_SAMPLE, training_labels);
}

void FreenectExampleModded::DecorateVideo(vector<cv::Mat> video) {
	for (int image_id = 0; image_id < video.size(); image_id++) {
		string text = to_string(image_id + 1) + "/" + to_string(video.size());
		putText(video[image_id], text, cv::Point(0, 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	}
}

void FreenectExampleModded::LoadVideo() {
	// Load File
	FreenectPlaybackWrapper wrap(this->video_path);
	// Clear Arrays
	this->video_rgb.clear();
	this->video_depth.clear();
	// Load Video
	while (wrap.GetNextFrame()) {
		this->video_rgb.push_back(wrap.RGB.clone());
		this->video_depth.push_back(wrap.Depth.clone());
	}
	// Decorate
	DecorateVideo(this->video_rgb);
	DecorateVideo(this->video_depth);
}

void FreenectExampleModded::ViewVideo() {
	// Create Windows
	destroyAllWindows();
	namedWindow(_RGB, flags);
	namedWindow(_Depth, flags);
	namedWindow(_Guess, flags);
	// Set Keys and Image
	this->key = -1;
	this->current_image = 0;
	while (key != 27) {
		// Load Image
		this->frame_rgb = this->video_rgb[current_image];
		this->frame_depth = this->video_depth[current_image];
		// If the files have updated data, load data into current
		imshow(this->_RGB, frame_rgb);
		imshow(this->_Depth, frame_depth);
		// Make Prediction
		cv::resize(frame_depth, frame_depth, Size(128, 128), 0, 0, INTER_CUBIC);
		frame_depth = frame_depth.reshape(0, 1);
		frame_depth.convertTo(frame_depth, CV_32FC1);
		try {
			imshow(_Guess, this->image_objects[this->support_vector_machine->predict(frame_depth)].images[0]);
		}
		catch (Exception e) {
			cout << "\nPrediction Error";
		}

		// Get Key and Convert to Upper Case
		this->key = waitKey(0);
		if (this->key > 'Z') this->key -= ' ';
		// Act on Keys
		switch (this->key) {
		case 'A':
			current_image--; break;
		case 'D':
			current_image++; break;
		default:
			ignore;
		}
		if (this->current_image < 0) this->current_image = 0;
		if (this->current_image >= video_rgb.size()) current_image = video_rgb.size() - 1;
	}

	// Destroy Windows and Exit
	destroyAllWindows();
}

void FreenectExampleModded::Start() {
	// File Name
	cout << "\nVideo File  : " << this->video_path;
	cout << "\nImage Files : " << this->image_path;
	cout << "\nLoading Video";
	LoadVideo();
	cout << "\nLoading Images";
	LoadImages();
	// Create Windows
	cout << "\nLearning Images";
	LearnImages();
	cout << "\nStarting Freenect Modded";
	ViewVideo();
};