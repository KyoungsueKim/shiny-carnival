#include "camera.h"
#include <opencv2/opencv.hpp>

Camera::Camera (){
    cv::VideoCapture cap(-1);

    if (!cap.isOpened()){
        printf("Camera Open Error");
        exit(1);
    }

    while(!interrupt){
        cap >> img;

        // 카메라 상하반전
        transpose(img, img);
        flip(img, img, 1);
        transpose(img, img);
        flip(img, img, 1);

        if(this->left){
            // 왼쪽 가이드 생성
            ellipse(img, cv::Point(0, 240), cv::Size(220.0, 70.0), 90, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
        }
        if(this->front){
            // 앞쪽 가이드 생성
            ellipse(img, cv::Point(320, 0),	 cv::Size(300.0, 70.0), 180, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
        }
        if(this->right){
            // 오른쪽 가이드 라인 생성
            ellipse(img, cv::Point(640, 240), cv::Size(220.0, 70.0), 90, 0, 360, cv::Scalar(0, 0, 255), 5, 8);
        }

        cv::imshow("Camera", img);

        if(cv::waitKey(1) == 27){
            delete this;
        }
    }
}

Camera::~Camera() {
    this->interrupt = true;
}

void Camera::TurnOnGuide (int direction){
    switch (direction)
    {
        case LEFT:
            this->left = true;
            break;

        case RIGHT:
            this->right = true;
            break;

        case FRONT:
            this->front = true;
            break;

        default:
            break;
    }
}

void Camera::TurnOffGuide (int direction){
    switch (direction){
        case LEFT:
            this->left = false;
            break;

        case RIGHT:
            this->right = false;
            break;

        case FRONT:
            this->front = false;
            break;

        default:
            break;
    }
}
