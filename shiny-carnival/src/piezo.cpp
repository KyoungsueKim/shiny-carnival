#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "pwm.h"
#include "piezo.h"

Melody::~Melody() {
    PWMWriteDutyCycle(0, 0);// mute
}

void* Melody::playMelody(float duration){
    // PWM 0 Initialize
    if (PWMExport(0) == 0 && PWMEnable(0) == 0) {
        // play
        for (int i = 0; i < melody.size(); i++) {
            piezo(melody[i], duration);
        }

        PWMWriteDutyCycle(0, 0); // mute
    }

    return NULL;
}

void* Melody::piezo(int scale, float duration){

    if (scale == 0){ // if mute
        PWMWriteDutyCycle(0, 0); 
    } 
    else {
        PWMWritePeriod(0, (1000000 / scale) * 1000);
        PWMWriteDutyCycle(0, ((1000000 / 2) / 261));
    }

    // maintain scale for duration 
    usleep((int)(1000000 * duration));

    // mute
    PWMWriteDutyCycle(0, 0);
    usleep(10000);

    return NULL; 
}