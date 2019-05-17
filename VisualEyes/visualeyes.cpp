

#include "VisualEyes.h"

using namespace std;


VisualEyes::VisualEyes(string filepath, string identifier, string splitter, int count) {
	this->default_filepath = filepath;
	this->filepath = filepath;
	this->identifier = identifier;
	this->splitter = splitter;
	this->count = count;
}

VisualEyes::~VisualEyes() {
	cv::destroyAllWindows();
}


void VisualEyes::Clear() { for (int _ = 0; _ < 100; _++) printf("\n"); }

void VisualEyes::Splash() {
	Clear();
	cout << splash;
	_sleep(1000);
}

void VisualEyes::Start() {
	Splash();
	// Set Window
	cv::startWindowThread();
	while (this->key != 'Q') {
		// Reset Window
		cv::destroyAllWindows();



		// Clear Screen
		Clear();
		// Menu
		cout << menu;
		// Data
		cout << \
			"\n Path       : " << this->filepath << \
			"\n Identifier : " << this->identifier << \
			"\n Count      : " << imageObjects.size() << "/" << this->count << \
			"";
		// Input
		this->key = _getche();
		this->key = this->key > 'Z' ? this->key - ' ' : this->key;
		Clear();
		// Handling
		switch (key) {
		case 'S':
			cout
				<< "\"reset\" to original filepath."
				<< "\n\nPath : ";
			cin >> this->filepath;
			if (this->filepath == "reset") this->filepath = this->default_filepath;
			break;
		case 'L':
			LoadObjects();
			break;
		case 'D':
			// Create Window
			cv::namedWindow(_VisualEyes, cv::WINDOW_NORMAL);
			cv::resizeWindow(_VisualEyes, this->window_dimension[0], this->window_dimension[1]);
			cv::moveWindow(_VisualEyes, 1920 - this->window_dimension[0], 1080 - this->window_dimension[1]);
			DisplayObjects();
			break;
		case 'R':
			cv::namedWindow(_VisualEyes, cv::WINDOW_FULLSCREEN);
			RecognEyes();
			break;
		default:
			ignore;
		}
	}
}



void VisualEyes::SetPath(string filepath) {
	this->filepath = filepath;
}

void VisualEyes::LoadObject(string image_name)
{
	ImageObject imageObject = ImageObject(image_name);
	for (int frame_id = 0; frame_id < 72; frame_id++) {
		string file_name = image_name + this->splitter + to_string(frame_id) + ".png";
		imageObject.AddImage(cv::imread(this->filepath + file_name));
	}
	this->imageObjects.push_back(imageObject);
}

void VisualEyes::LoadObjects() {
	imageObjects.clear();
	cout << "\nLoading Objects";
	for (int imageObjectID = 0; imageObjectID < count; imageObjectID++) {
		LoadObject(this->identifier + to_string(imageObjectID + 1));
		cout << ".";
	}
}

void VisualEyes::DisplayObject(string identifier) {
	for (ImageObject imageObject : this->imageObjects)
		if (identifier == imageObject.name)
			imageObject.LoopDisplay();
}


void VisualEyes::DisplayObjects() {
	cout
		<< "\n"
		<< "\nDisplaying Objects <A D>";
	if (imageObjects.size() > 0) {
		this->key = -1;
		int imageObjectID = 0;
		while (this->key != 27) {
			// Keep Image Object ID within bounds and loopback
			while (imageObjectID < 0) imageObjectID += imageObjects.size();
			imageObjectID %= imageObjects.size();
			// Get Key from Looped Display
			this->key = imageObjects[imageObjectID].LoopDisplay();
			// Act according to key
			switch (this->key) {
			case this->LEFT:
				imageObjectID--;
				break;
			case this->RIGHT:
				imageObjectID++;
				break;
			}
		}
	}
}

void VisualEyes::RecognEyes() {
	// Display till Esc pressed
	this->key = -1;
	bool webcam_status = true;
	while (this->key != 27) {
		try {
			// If there's no WebCam, update the WebCam
			if (!this->web_cam.read(this->frame)) {
				if (webcam_status) {
					cout << "\nWebcam Unplugged";
					webcam_status = false;
				}
				this->web_cam.~VideoCapture();
				this->web_cam = cv::VideoCapture::VideoCapture(0);
			}
			else {
				if (!webcam_status) {
					cout << "\nWebcam Plugged In";
					webcam_status = true;
				}
				// Analyse Frame
				//cout << "\nMost Likely " << MostLikely(this->frame);
				// Display Frame
				cv::imshow(this->_VisualEyes, this->frame);
				// Get Key
			}
			this->key = cv::waitKey(1);
		}
		catch (exception e) {
			cout << "\nWebcam Unplugged";
		}
	}
	// Kill WebCam
}


int VisualEyes::MostLikely(cv::Mat image) {
	int mostLikely = 0;
	double maxLikely = 0;
	for (int imageID = 0; imageID < this->imageObjects.size(); imageID++)
	{
		int likeliness = imageObjects[imageID].Likeness(image);
		if (likeliness > maxLikely) {
			mostLikely = imageID;
			maxLikely = likeliness;
		}
	}
	return mostLikely;

}