#include <opencv2/opencv.hpp>

struct Directions{
    bool right;
    bool front;
    bool left;
};

class Camera {
    private:
        Directions directions;
        bool interrupt = false;

        cv::Mat img;

    public:
        Camera();
        ~Camera();
        int SetGuides(Directions directions);
};