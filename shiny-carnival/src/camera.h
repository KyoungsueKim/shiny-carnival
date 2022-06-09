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
	bool interrupt = false;

	cv::Mat img;

public:
	Camera();
	~Camera();
	int SetGuides(Directions directions);
};

#endif