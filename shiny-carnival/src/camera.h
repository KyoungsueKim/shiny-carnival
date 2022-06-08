#include <opencv2/opencv.hpp>

enum direction
{
    LEFT = 0,
    FRONT = 1,
    RIGHT = 2
};

class Camera {
    private:
        bool left = false;
        bool front = false;
        bool right = false;
        bool interrupt = false;

        cv::Mat img;

    public:
        Camera();
        ~Camera();
        void TurnOnGuide(int direction);
        void TurnOffGuide(int direction);
};