#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "camera.h"
#include "piezo.h"
#include "sensor.h"
#include "socket.h"

#define LED 0 // Physical 11

int main(void){
    if (wiringPiSetup() == -1) return 1; // wiringPi 셋업.

    std::cout << "Hello World" << std::endl;

    int a = 0;

    return 0;
}