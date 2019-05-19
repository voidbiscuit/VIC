#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <vector>

enum State
{
	UPDATED_RGB = 0x01,
	UPDATED_DEPTH = 0x10
};

class FreenectPlaybackWrapper
{
public:
	FreenectPlaybackWrapper(std::string FreenectVideoFolder);
	std::vector<cv::Mat> GetFrame();
	uint8_t GetNextFrame();
	cv::Mat RGB;
	cv::Mat Depth;
	cv::Mat DepthRaw;

protected:
	std::ifstream reader;
	uint8_t previous_state = 0;
	bool finished = false;
	std::string freenect_video_folder = "";
	std::string previous_line = "";
	double previous_timestamp = 0;
	std::chrono::milliseconds previous_time_ran;
	double GetTimestampFromFilename(std::string Filename);
};