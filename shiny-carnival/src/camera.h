#ifndef CAMERA_H
#define CAMERA_H

struct Directions
{
	bool right;
	bool front;
	bool left;
};

#include <opencv2/opencv.hpp>

class Camera
{
private:
	Directions directions;

	cv::Mat img;

public:
	int SetGuides(Directions directions);
    void DisplayCamera();
};

#endif