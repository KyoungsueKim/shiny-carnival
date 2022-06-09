#include "camera.h"
#include <opencv2/opencv.hpp>


int Camera::SetGuides(Directions directions) {
    this->directions = directions;
    if(directions.left || directions.front || directions.right){
        return 1;
    } else {
        return 0;
    }
}

void Camera::DisplayCamera(){
    cv::VideoCapture cap(-1);

    if (!cap.isOpened()){
        printf("Camera Open Error");
        exit(1);
    }

    cap >> img;

    // 카메라 상하반전
    transpose(img, img);
    flip(img, img, 1);
    transpose(img, img);
    flip(img, img, 1);

    if(this->directions.left){
        // 왼쪽 가이드 생성
        ellipse(img, cv::Point(0, 240), cv::Size(220.0, 70.0), 90, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
    }
    if(this->directions.front){
        // 앞쪽 가이드 생성
        ellipse(img, cv::Point(320, 0),	 cv::Size(300.0, 70.0), 180, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
    }
    if(this->directions.right){
        // 오른쪽 가이드 라인 생성
        ellipse(img, cv::Point(640, 240), cv::Size(220.0, 70.0), 90, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
    }

    //cv::imshow("Camera", img);

    if(cv::waitKey(1) == 27){
        delete this;
    }

}
