#include "FreenectPlaybackWrapper/freenect-playback-wrapper.h"

int main(int argc, char * argv[])
{
	FreenectPlaybackWrapper wrap("F:/Work/Year 3/Visual Intelligence/coil-20-proc/");

	cv::Mat currentRGB;
	cv::Mat currentDepth;

	// Create the RGB and Depth Windows
	cv::namedWindow("RGB", cv::WindowFlags::WINDOW_AUTOSIZE | cv::WindowFlags::WINDOW_GUI_EXPANDED);
	cv::namedWindow("Depth", cv::WindowFlags::WINDOW_AUTOSIZE | cv::WindowFlags::WINDOW_GUI_EXPANDED);

	// The key value represents a key pressed on the keyboard,
	// where 27 is the ESCAPE key
	char key = '0';

	// The status represents the current status of the Playback
	// wrapper. 
	//
	// A value of 0 represents that it has finished
	// playback.
	//
	// The status can by bitwise AND to determine if the RGB or
	// Depth image has been updated using the State enum.
	uint8_t status = 255;

	while (key != 27 && status != 0)
	{
		// Loads in the next frame of Kinect data into the
		// wrapper. Also pauses between the frames depending
		// on the time between frames.
		status = wrap.GetNextFrame();

		// Determine if RGB is updated, and grabs the image
		// if it has been updated
		if (status & State::UPDATED_RGB)
			currentRGB = wrap.RGB;

		// Determine if Depth is updated, and grabs the image
		// if it has been updated
		if (status & State::UPDATED_DEPTH)
			currentDepth = wrap.Depth;

		// Show the images in the windows
		cv::imshow("RGB", currentRGB);
		cv::imshow("Depth", currentDepth);

		// Check for keyboard input
		key = cv::waitKey(10);
	}

	return 0;
}