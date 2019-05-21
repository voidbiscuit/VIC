
#include "FreenectExampleModded.h"

using namespace cv;
using namespace std;

FreenectExampleModded::FreenectExampleModded(string image_path, string video_path) {
	_image_path = image_path;
	_video_path = video_path;
}

void FreenectExampleModded::LoadImages() {
	// Remove Image Objects
	_image_objects.clear();
	cout << "\nLoading Images";
	for (int object_id = 1; object_id <= _image_object_count; object_id++) {
		// Make new Image Object
		ImageObject image_object = ImageObject(to_string(object_id), object_id);
		// Load in Images
		for (int angle = 0; angle < _image_object_images; angle++) {
			string filepath = _image_path + "obj" + to_string(object_id) + "__" + to_string(angle) + ".png";
			cv::Mat image = cv::imread(filepath);
			cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
			image_object.add_image(image);
			cout << ".";
		}
		// Add Image Object to Array
		_image_objects.push_back(image_object);
	}
}

void FreenectExampleModded::LearnImages() {
	// Support Vector Machine
	// https://docs.opencv.org/3.4/d1/d2d/classcv_1_1ml_1_1SVM.html
	// Create SVM
	cout << "\nCreating SVM";
	_support_vector_machine = cv::ml::SVM::create();
	// Set Properties
	// C-Support Vector Classification :  n-class classification (n ≥ 2), allows imperfect separation of classes with penalty multiplier C for outliers.
	_support_vector_machine->setType(cv::ml::SVM::C_SVC);
	_support_vector_machine->setKernel(cv::ml::SVM::LINEAR);
	_support_vector_machine->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER,2,0));
	// Train Data Size
	cv::Mat training_data(20 * 72, 128 * 128, CV_32FC1);
	cv::Mat training_labels(20 * 72, 1, CV_32SC1);
	cout << "\nTraining";
	// For Each Image Object
	for (int image_object_id = 0; image_object_id < _image_objects.size(); image_object_id++) {
		ImageObject image_object = _image_objects[image_object_id];
		// For Each Image
		for (int image_id = 0; image_id < _image_objects[image_object_id].images.size(); image_id++) {
			// Get and show image
			cv::Mat image = image_object.images[image_id].clone().reshape(0, 1);
			// Training Set ID
			int training_set_id = (image_object_id * 72) + image_id;
			// Set Training Label
			training_labels.at<int>(training_set_id) = image_object_id;
			// For each Pixel
			for (int pixel_id = 0; pixel_id < image.cols; pixel_id++)
				training_data.at<float>(training_set_id, pixel_id) = (float)image.at<uchar>(pixel_id);
			cout << ".";
		}
	}
	_support_vector_machine->train(training_data, cv::ml::ROW_SAMPLE, training_labels);
	cout << "\nTrained";
}


void FreenectExampleModded::DecorateFrame(frame frame, int frame_id, int frame_count) {
	string text = to_string(frame_id) + "/" + to_string(frame_count);
	putText(frame.rgb, text, cv::Point(0, 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	putText(frame.depth_raw, text, cv::Point(0, 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	putText(frame.depth, text, cv::Point(0, 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

}

void FreenectExampleModded::DecorateVideo(vector<frame> stream) {
	for (int frame_id = 0; frame_id < stream.size(); frame_id++)
		DecorateFrame(stream[frame_id], frame_id + 1, stream.size());
}

void FreenectExampleModded::LoadVideo() {
	// Load Video from Directory
	// File Readers
	std::ifstream index_reader, file_reader;
	// Format Path and Open Index.txt
	std::replace(_video_path.begin(), _video_path.end(), '\\', '/');
	cout << "\n" << _video_path + "INDEX.txt";
	index_reader.open(_video_path + "INDEX.txt");
	// If reader breaks, return
	if (!index_reader.is_open()) { cout << "\nError loading Video"; return; }
	// Read All Files
	std::string file_name, file_path, format_settings;
	uint32_t
		frame_skip = 30, count = 0,
		width = 0, height = 0, max_size = 0,
		max_value = 0, shift = 5, low_pass = 0;
	frame current_frame;
	cv::Mat empty_picture = cv::Mat();
	cout << "\nLoading Video";
	while (getline(index_reader, file_name)) {
		if (count < frame_skip) {
			++count;
		}
		else {
			// Update Full File Path
			file_path = _video_path + file_name;
			// Check File Type
			switch (file_name[0]) {
			case 'r': // RGB
				current_frame.rgb = cv::imread(file_path, cv::ImreadModes::IMREAD_UNCHANGED);
				if (!(current_frame.depth_raw.size == empty_picture.size || current_frame.depth.size == empty_picture.size)) {
					// Push Current Frame
					stream.push_back(current_frame);
					// Clear current frame
					current_frame = frame();
					// Frame Skip
					cout << ".";
					count = 0;
				}
				break;
			case 'd': // Depth
				// Get File Info
				file_reader.open(file_path, ifstream::binary);
				getline(file_reader, format_settings);
				sscanf(format_settings.c_str(), "P5 %d %d %d", &width, &height, &max_size);
				// Clear Mat
				current_frame.depth_raw = cv::Mat(cv::Size(width, height), CV_16UC1);
				current_frame.depth = cv::Mat(cv::Size(width, height), CV_8UC1);
				// Read File
				file_reader.read((char*)current_frame.depth_raw.data, width*height * sizeof(uint16_t));
				file_reader.close();
				// Swap Endianness
				for (unsigned int pixel = 0; pixel < width*height; pixel += sizeof(uint16_t))current_frame.depth_raw.data[pixel] = _byteswap_ushort(current_frame.depth_raw.data[pixel]);

				// Read Depth Raw into Depth
				for (unsigned int pixel = 0; pixel < width*height; pixel++)
					current_frame.depth.data[pixel] = ((uint8_t)current_frame.depth_raw.data[(pixel * sizeof(uint16_t))-1]) >> low_pass << low_pass << shift;
				break;
			default:
				cout << "\nUnrecognised File";
			}
		}
	}
	DecorateVideo(stream);
	return;
}

void FreenectExampleModded::ViewVideo() {
	system("cls");
	// Destroy all windows, then create windows
	destroyAllWindows();
	namedWindow(_RGB, _flags);
	namedWindow(_Depth, _flags);
	namedWindow(_Depth_Raw, _flags);
	namedWindow(_Guess, _flags);
	// Set Keys and Image
	_key = -1;
	current_frame_id = 0;
	frame_switched = true;
	while (_key != 27) {
		// If the Frame hasn't been loaded
		if (frame_switched)
		{
			// Update frame loading status
			frame_switched = false;
			// Load Frame
			current_frame = stream[current_frame_id];
			// Test RGB
			current_frame.depth = current_frame.rgb;
			cv::cvtColor(current_frame.depth, current_frame.depth, cv::COLOR_RGB2GRAY);
			// Resize
			current_frame.depth = current_frame.depth(cv::Rect(640 / 2 - 64 * 2, 480 / 2 - 48 * 3, 64 * 5, 48 * 4));
			cv::resize(current_frame.depth, current_frame.depth, Size(640, 480), 0, 0, INTER_CUBIC);
			// Load Images from Frame
			imshow(_RGB, current_frame.rgb);
			imshow(_Depth_Raw, current_frame.depth_raw);
			imshow(_Depth, current_frame.depth);
			// Preprare frame for prediction
			cv::resize(current_frame.depth, current_frame.depth, Size(128, 128), 0, 0, INTER_CUBIC);
			current_frame.depth = current_frame.depth.reshape(0, 1);
			current_frame.depth.convertTo(current_frame.depth, CV_32FC1);
			// Make Prediction
			current_guess = _support_vector_machine->predict(current_frame.depth);

			cout << "\nGuess : " << int(current_guess);
		}
		// Update Current Rotation
		current_guess_rotation = (current_guess_rotation + 1) % _image_objects[current_guess].images.size();
		// Show image based on prediction
		imshow(_Guess, _image_objects[current_guess].images[current_guess_rotation]);
		// Get Key and Convert to Upper Case
		_key = waitKey(40);
		if (_key > 'Z') _key -= ' ';
		// Act on Keys
		switch (_key) {
		case 'A': current_frame_id--; frame_switched = true; break;
		case 'D': current_frame_id++; frame_switched = true; break;
		default: ignore; break;
		}
		// Set within bounds
		if (current_frame_id < 0) current_frame_id = 0;
		if (current_frame_id >= stream.size()) current_frame_id = stream.size() - 1;
	}

	// Destroy Windows and Exit
	destroyAllWindows();
}

void FreenectExampleModded::Start() {
	// File Name
	cout << "\nVideo File  : " << _video_path;
	cout << "\nImage Files : " << _image_path;
	LoadVideo();
	LoadImages();
	// Create Windows
	LearnImages();
	ViewVideo();
};